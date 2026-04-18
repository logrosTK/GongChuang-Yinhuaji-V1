#include "tower.h"
#include "cmsis_os2.h"
#include "dm_motor_drv.h"

struct Finish_Flag flag =
{
		.MOTOR35_flag      = 0,
		.MOTOR28_flag      = 0,
		.G6220_flag        = 0
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      			������ʼ��
//  @return     			/
//  @Sample usage:    ����can��ʼ�������������ʼ����G6220��ʼ��
//-------------------------------------------------------------------------------------------------------------------
void Tower_init(void)
{
	CAN_Start(CAN_NUM);
	osDelay(1000);
	TowerMotor_Init();
	G6220_Init();	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      			����λ�ÿ���
//  @return     			/
//  @param					 yaw   λ��,������õ��ǽǶ���,��Ȧ��ΧΪ0-3600�����ֱ���Ϊ0.1��,�������ö�Ȧ�Ƕ�
//  @param					 yaw_speed			�ٶ�,���õ�Ҳ�ǽǶ��ƣ��ٶ����Ϊ2300��ÿ��
//  @param					 z      �߶ȣ���λΪ0.1mm����ʾz��߶�
//  @param					 z_speed	�ٶȣ���λΪmm����ʾz���ƶ����ٶ�
//  @param					 y      ���ȣ���λΪ0.1mm����ʾ�����쳤�ĳ���
//  @param					 y_speed	�ٶȣ���λΪmm����ʾ�����ƶ����ٶ�
//  @Sample usage:    /̫���ˣ��Լ�ȥ���濴�ڲ���������
//-------------------------------------------------------------------------------------------------------------------
void Tower_Control(uint32_t yaw,uint16_t yaw_speed,uint32_t z,uint16_t z_speed,uint32_t y,uint16_t y_speed)
{
	
	G6220_Position(yaw,yaw_speed);
	HAL_Delay(200);
	Motor35_AbsPosition(z,z_speed);
	HAL_Delay(200);
	Motor28_AbsPosition(y,y_speed);
}

//-------------------------------------------------------------------------------------------------------------------
//����Ϊ����G6220�п���̨������ƺ���

//-------------------------------------------------------------------------------------------------------------------
//  @brief      			G6220ʹ��
//  @return     			/
//  @Sample usage:    /
//-------------------------------------------------------------------------------------------------------------------
void G6220_Init(void)
{
	 G6220_Command(1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 G6220����
//  @return     		 /
//  @param					 CMD  �������� ʹ��:1,ʧ��:2,�������:3,�������:4
//  @Sample usage:   G6220_Command(&hcan1,0x01,1);//���ʹ��
//-------------------------------------------------------------------------------------------------------------------
void G6220_Command(uint8_t CMD)
{
	switch(CMD)
	{
		case G6220_Enable:
			enable_motor_mode(&hcan1, J4310_CAN_ID, POS_MODE);
			break;
		case G6220_Disable:
			disable_motor_mode(&hcan1, J4310_CAN_ID, POS_MODE);
			break;
		case G6220_Save_Zero_Position:
			save_pos_zero(&hcan1, J4310_CAN_ID, POS_MODE);
			break;
		case G6220_Clear_Error:
			clear_err(&hcan1, J4310_CAN_ID, POS_MODE);
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 G6220λ���ٶ�ģʽ
//  @return     		 /
//  @param					 position   λ��,������õ��ǽǶ���,��Ȧ��ΧΪ0-3600�����ֱ���Ϊ0.1��,�������ö�Ȧ�Ƕ�
//  @param					 speed			�ٶ�,���õ�Ҳ�ǽǶ��ƣ��ٶ����Ϊ2300��ÿ��
//  @Sample usage:   G6220_Position(1800,1800);//��180��ÿ����ٶ�ת��180�ȵ�λ��
//-------------------------------------------------------------------------------------------------------------------
void G6220_Position(int32_t position, uint16_t speed)
{
	float pos_rad = position * 0.00174533f;  /* 0.1° → rad */
	float vel_rad = speed   * 0.0174533f;   /* °/s  → rad/s */
	pos_ctrl(&hcan1, J4310_CAN_ID, pos_rad, vel_rad);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 G6220�ٶ�ģʽ
//  @return     		 /
//  @param					 speed			�ٶ�,���õ�Ҳ�ǽǶ��ƣ��ٶ����Ϊ2300��ÿ��
//  @Sample usage:   G6220_Speed(1800);//��180��ÿ����ٶȳ���ת��
//-------------------------------------------------------------------------------------------------------------------
void G6220_Speed(uint16_t speed)
{
	float velocity;
	velocity = speed*0.0174533;
	uint8_t *Velocity_Tmp;
	uint8_t Data[4];
	Velocity_Tmp = (uint8_t *)&velocity;
	Data[0] = *(Velocity_Tmp);
	Data[1] = *(Velocity_Tmp + 1);
	Data[2] = *(Velocity_Tmp + 2);
	Data[3] = *(Velocity_Tmp + 3);
	CAN_SendData(CAN_NUM,G6220_SPEED+G6220_CAN_ID,(uint8_t*)Data,4);//�����Ϊλ���ٶ�ģʽʱ,idҪ��0x200
}



//-------------------------------------------------------------------------------------------------------------------
//����Ϊ�Ŵ�ͷ����������ƺ���

//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 ���������ʼ��
//  @return     		 /
//  @Sample usage:   
//-------------------------------------------------------------------------------------------------------------------
void TowerMotor_Init(void)
{
	Motor_Homing(MOTOR35_CAN_ID);
  Motor_Homing(MOTOR28_CAN_ID);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 ���������Ȧ����λ����
//  @return     		 /
//  @param					 ID   ���������canID
//  @Sample usage:   Motor_Homing(0x300)//canIDΪ0x300�Ĳ���������ж�Ȧ����λ����
//-------------------------------------------------------------------------------------------------------------------
void Motor_Homing(uint16_t ID)
{
	uint8_t Data[4]={0x9A,0x02,0x00,0x6B};
	CAN_SendEXData(CAN_NUM,ID,(uint8_t*)Data,4);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 �����������λ��ģʽ
//  @return     		 /
//  @param					 dir    ���򣬱�ʾ��������λ�õ�����
//  @param					 ID			���������canID
//  @param					 step   ����������Χ��0-uint32��һ������Ϊ1.8��
//  @param					 speed	�ٶȣ���λΪrpm����ΧΪ0-uint16
//  @Sample usage:   Motor_AbsPosition(0,0x300,1000,1000);//��1000תÿ���ӵ��ٶȿ���canIDΪ0x300�Ĳ��������0����ת��1800��
//-------------------------------------------------------------------------------------------------------------------
void Motor_AbsPosition(uint8_t dir,uint16_t ID,uint32_t step,uint16_t speed)
{
	uint8_t Data[16]={0xFD,0x00,0xAF,0xFF,0xAF,0xFF,0x00,0x00,0xFD,0x00,0x00,0x00,0x00,0x01,0x00,0x6B};
	
	Data[1]=dir >> 0;
	
	Data[6]=(speed >> 8); 
	Data[7]=(speed >> 0); 
	
	Data[9]=(step >> 24); 
	Data[10]=(step >> 16); 
	Data[11]=(step >> 8); 
	Data[12]=(step >> 0);
	
	Can_SendCmd(ID,(uint8_t*)Data,16);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 35�����������λ��ģʽ������Z��߶ȣ�
//  @return     		 /
//  @param					 h      �߶ȣ���λΪ0.1mm����ʾz��߶�
//  @param					 speed	�ٶȣ���λΪmm����ʾz���ƶ����ٶ�
//  @Sample usage:   Motor35_AbsPosition(1000,50);//��50mmÿ����ٶ��ƶ���100.0�߶ȴ�
//-------------------------------------------------------------------------------------------------------------------
void Motor35_AbsPosition(uint32_t h,uint16_t speed)
{
	uint32_t z = 0;
	if(h>= claw_h_max)
		z = 0;
	else
	  z = claw_h_max - h;
	z = z > 1600 ? 1600 : z;
	Motor_AbsPosition(MOTOR35_Dir,MOTOR35_CAN_ID,z*MOTOR35_Reduction,speed*30);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 28�����������λ��ģʽ���������۳��ȣ�
//  @return     		 /
//  @param					 y      ���ȣ���λΪ0.1mm����ʾ�����쳤�ĳ���
//  @param					 speed	�ٶȣ���λΪmm����ʾ�����ƶ����ٶ�
//  @Sample usage:   Motor28_AbsPosition(1000,50);//��50mmÿ����ٶȽ�����ͷ�ƶ���100.0mmλ��
//-------------------------------------------------------------------------------------------------------------------
void Motor28_AbsPosition(uint32_t r,uint16_t speed)
{
	r = r < claw_r_min ? claw_r_min : r;
	uint32_t y = r - claw_r_min;
	y = y > 1660 ? 1660 : y;
	Motor_AbsPosition(MOTOR28_Dir,MOTOR28_CAN_ID,y*MOTOR28_Reduction,speed*0.53);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      		 can����֡����
//  @return     		 /
//  @param					 ExtId  ��չ֡
//  @param					 StdId	��׼֡
//  @param					 rxBuf	can����֡
//  @Sample usage:   Response_Handle(ExtId,StdId,rxBuf);
//-------------------------------------------------------------------------------------------------------------------
void Response_Handle(uint32_t ExtId,uint32_t StdId, uint8_t *rxBuf)
{
	if(ExtId==0x300)
	{
		flag.MOTOR35_flag=1;
	}
	else if(ExtId==0x400)
	{
		flag.MOTOR28_flag=1;
	}
	else if(StdId==0)
	{
		
		flag.G6220_flag=1;
	}
	
}

//static float uint_to_float(int X_int, float X_min, float X_max,int Bits){
//float span = X_max - X_min;
//float offset = X_min;
//return ((float)X_int)*span/((float)((1<<Bits)-1)) + offset;
//}

//typedef struct
//{
//uint16_t ID;
//Motor_CANFrameInfo_typedef CANFrameInfo;
//DM_Motor_Data_Typedef Data;
//}DM_Motor_Info_Typedef;


//void DM_Motor_Info_Update(uint8_t *Data,DM_Motor_Info_Typedef *DM_Motor)
//{
//DM_Motor->ID = Data[0] & 0x0F;
//DM_Motor->Data.State = Data[0]>>4;
//DM_Motor->Data.P_int = (uint16_t)(((Data[1]) <<8) | (Data[2]));
//DM_Motor->Data.V_int = (uint16_t)((Data[3]) <<4) | ((Data[4])>>4);
//DM_Motor->Data.T_int = (uint16_t)((Data[4]&0xF) <<8) | ((uint16_t)(Data[5]));
//DM_Motor->Data.Position=uint_to_float(DM_Motor->Data.P_int,-P_MAX,P_MAX,16);
//DM_Motor->Data.Velocity=uint_to_float(DM_Motor->Data.V_int,-V_MAX,V_MAX,12);
//DM_Motor->Data.Torque= uint_to_float(DM_Motor->Data.T_int,-T_MAX,T_MAX,12);
//DM_Motor->Data.Temperature_MOS = (float)(Data[6]);
//DM_Motor->Data.Temperature_Rotor = (float)(Data[7]);
//}
