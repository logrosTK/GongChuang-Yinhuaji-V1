/**
  ******************************************************************************
  * @author  PINK SAKURA
  * @version V1.0
  * @date    2024-08-08
  * @brief   ������
  ******************************************************************************
  * @attention
  * ���䣺sakura.mail@qq.com
  ******************************************************************************
  */

#include "Main_task.h"
#include "position.h"
#include "visual_identity.h"
#include "cmsis_os2.h"

uint8_t GET_TASK_FLAGE = 0;
uint8_t GET_TASK_TIME_OUT = 0;

void Main_init(void)
{
	// SoftSPI_OLED_Init();           // ���Ƴ�����SoftSPI OLED
	// SoftSPI_OLED_Clear();          // ���Ƴ�
	// SoftSPI_OLED_DisplayTurn(0);   // ���Ƴ�
	visual_idle();
	pos_reset();
}

void Get_Task_Code(void)
{
	for(uint8_t i=0; i<6; i++)
	  color_task[i]=0;
	GET_TASK_FLAGE = 1;
	while(color_task[0] + color_task[5] == 0)
	{
		identify_QR_code();    //ʶ���ά��
		osDelay(50);
		GET_TASK_TIME_OUT++;
		if (GET_TASK_TIME_OUT >= 80)  //��ʱ����
		{
			if (d_X == 0 || d_X == -30)
			  d_X = 30;
			else
				d_X = -30;
			GET_TASK_TIME_OUT = 0;
		}
	}
	GET_TASK_FLAGE = 0;
	visual_idle();
	Chassis_d_zero();
}

void Show_Task_Code(void)
{
	// SoftSPI_OLED_Clear();                            // ���Ƴ�
	// for(uint8_t i=0;i<3;i++)
	// {
	//     SoftSPI_OLED_ShowBN(8+i*16,16, color_task[i], 1);  // ���Ƴ�
	// }
	// SoftSPI_OLED_ShowBN(56,16, 0, 1);               // ���Ƴ�
	// for(uint8_t i=3;i<6;i++)
	// {
	//     SoftSPI_OLED_ShowBN(24+i*16,16, color_task[i], 1); // ���Ƴ�
	// }
	// SoftSPI_OLED_Refresh();                          // ���Ƴ�
}

void CRing_Dual_Calibration()
{
	Chassis_Visual_Pos(4, 320, 222, 4, 0);

		identify_posture(1500, 1200);  //ʶ����̬
		osDelay(100);

		Chassis_Visual_Pos(2, 322, 228, 4, 0);

		identify_posture(1500, 1700);  //ʶ����̬
		osDelay(200);
}

void Main_task(void)
{
	osDelay(100);
	Main_init();

//	while(1)  //���Լ�צ
//	{
//	  while(!(HAL_GPIO_ReadPin(Start_GPIO_Port,Start_Pin))) {osDelay(50);}
//	  LFD_01M_OP_C;    //��צ��λ
//		osDelay(200);
//		while(!(HAL_GPIO_ReadPin(Start_GPIO_Port,Start_Pin))) {osDelay(50);}
//		LFD_01M_CL;    //��צ�ر�
//		osDelay(200);
//	}
//	while(1){osDelay(50);};

	// SoftSPI_OLED_ShowString(16,0,(uint8_t *)"SW Ver 2.7.4",16,1);   // ���Ƴ�
	// SoftSPI_OLED_ShowString(8,16,(uint8_t *)"OLED INTI PASS",16,1); // ���Ƴ�
	// SoftSPI_OLED_Refresh();                                          // ���Ƴ�

	while(!ready) { osDelay(10);}
	osDelay(100);
	// buzzer_rings(100, 0, 1);  // ���Ƴ����޷�����

	// SoftSPI_OLED_Clear();  // ���Ƴ�

	pos_reset();

	while(!(HAL_GPIO_ReadPin(Start_GPIO_Port,Start_Pin))) {osDelay(50);}

	chassis_mode=1;    //����ģʽ

	#ifdef __PRELIMINARY_ROUND__

	while(1)
	{
//		color_task[0]=1;
//		color_task[1]=3;
//		color_task[2]=2;

//		identify_posture(1500, 1700);  //ʶ����̬
//		osDelay(500);
//
//		CRing_Dual_Calibration();  //ɫ��˫��У׼

//			grab_materials_car(1, 0);     //����ץȡ����  ����һ��Ϊ��ţ�
//			put_materials_ground(color_task[0],0);
//			grab_materials_car(2, 0);     //����ץȡ����
//			put_materials_ground(color_task[1],0);
//			grab_materials_car(3, 0);     //����ץȡ����
//			put_materials_ground(color_task[2],0);
//			reset_posture();
//		while(1){osDelay(10);};

		Chassis_Go_Pos(150, 165, -45, 0, 300);  //����

		reset_posture();

		Chassis_Go_Pos(730, 130, -90, 0, 10);  //ɨ��

		Get_Task_Code();
		Show_Task_Code();

		/* ��һ�� */

		Chassis_Go_Pos(1450, 110, -90, 0, 100);  //����̨

			grab_turntable_B(color_task[0],1);
			grab_turntable_B(color_task[1],2);
			grab_turntable_B(color_task[2],3);

		Chassis_Go_Pos(1050, 165, 0, 1, 300);  //T1·��

		Chassis_Go_Pos(1050, 1900, 0, 1, 300);  //�ּӹ���1

		Chassis_Go_Pos(1050, 1900, 90, 1, 50);  //�ּӹ���2

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1000, 1950, 90, 0, 300);

		CRing_Dual_Calibration();  //ɫ��˫��У׼

			grab_materials_car(1, 0);     //����ץȡ����  ����һ��Ϊ��ţ�
			put_materials_ground(color_task[0],0);
			grab_materials_car(2, 0);     //����ץȡ����
			put_materials_ground(color_task[1],0);
			grab_materials_car(3, 0);     //����ץȡ����
			put_materials_ground(color_task[2],0);
			reset_posture();

			grab_materials_ground(color_task[0],1);     //����ץȡ����
			put_materials_car(1);
			grab_materials_ground(color_task[1],0);     //����ץȡ����
			put_materials_car(2);
			grab_materials_ground(color_task[2],0);     //����ץȡ����
			put_materials_car(3);

		Chassis_Go_Pos(1000, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 0, 1, 300);

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1900, 1020, 0, 0, 100);

		CRing_Dual_Calibration();  //ɫ��˫��У׼
		Update_OPS(1940, 1050, 0);

			grab_materials_car(1, 0);     //����ץȡ����  ����һ��Ϊ��ţ�
			put_materials_ground(color_task[0],0);
			grab_materials_car(2, 0);     //����ץȡ����
			put_materials_ground(color_task[1],0);
			grab_materials_car(3, 0);     //����ץȡ����
			put_materials_ground(color_task[2],0);
			reset_posture();

		Chassis_Go_Pos(1900, 165, -90, 1, 300);

		/* �ڶ��� */

		Chassis_Go_Pos(1450, 120, -90, 0, 100);  //����̨

			grab_turntable_B(color_task[3],1);
			grab_turntable_B(color_task[4],2);
			grab_turntable_B(color_task[5],3);

		Chassis_Go_Pos(1050, 165, 0, 1, 300);  //T1·��

		Chassis_Go_Pos(1050, 1900, 0, 1, 300);  //�ּӹ���1

		Chassis_Go_Pos(1050, 1900, 90, 1, 200);  //�ּӹ���2

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1000, 1950, 90, 0, 300);

		CRing_Dual_Calibration();  //ɫ��˫��У׼

			grab_materials_car(1, 0);     //����ץȡ����  ����һ��Ϊ��ţ�
			put_materials_ground(color_task[3],0);
			grab_materials_car(2, 0);     //����ץȡ����
			put_materials_ground(color_task[4],0);
			grab_materials_car(3, 0);     //����ץȡ����
			put_materials_ground(color_task[5],0);

			grab_materials_ground(color_task[3],1);     //����ץȡ����
			put_materials_car(1);
			grab_materials_ground(color_task[4],0);     //����ץȡ����
			put_materials_car(2);
			grab_materials_ground(color_task[5],0);     //����ץȡ����
			put_materials_car(3);

		Chassis_Go_Pos(1000, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 0, 1, 300);

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1900, 1040, 0, 0, 100);

		Chassis_Visual_Pos(3, 328, 230, 6, 0);
		Update_OPS(1950, 1050, 0);

			grab_materials_car(1, 0);     //����ץȡ����  ����һ��Ϊ��ţ�
			put_materials_ground(color_task[3],1);
			grab_materials_car(2, 0);     //����ץȡ����
			put_materials_ground(color_task[4],1);
			grab_materials_car(3, 0);     //����ץȡ����
			put_materials_ground(color_task[5],1);
			reset_posture();

		Chassis_Go_Pos(1900, 165, -90, 1, 300);

		Chassis_Go_Pos(200, 165, -90, 0, 300);

		Chassis_Go_Pos(0, 0, 0, 0, 3000);

		chassis_mode=2;    //ֹͣģʽ

		while(1){osDelay(100);}

	}

	#else

	while(1)
	{
	  uint8_t block_pos[3] = {0};

		Chassis_Go_Pos(150, 165, -45, 0, 300);  //����

		reset_posture();

		Chassis_Go_Pos(730, 135, -90, 0, 10);  //ɨ��

		Get_Task_Code();
		Show_Task_Code();

		/* ��һ�� */

		Chassis_Go_Pos(1050, 165, 0, 1, 300);  //T1·��

		Chassis_Go_Pos(1050, 1900, 0, 1, 300);  //�ּӹ���1

		Chassis_Go_Pos(1050, 1900, 90, 1, 200);  //�ּӹ���2

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1000, 1950, 90, 0, 300);

		Chassis_Visual_Pos(3, 328, 230, 6, 0);

		switch(VIS_RX.block_order)  //��ɫ��Ӧɫ��
		{
			case 1:  block_pos[0] = 3; block_pos[1] = 2; block_pos[2] = 1; break;
			case 2:  block_pos[0] = 3; block_pos[1] = 1; block_pos[2] = 2; break;
			case 3:  block_pos[0] = 2; block_pos[1] = 3; block_pos[2] = 1; break;
			case 4:  block_pos[0] = 1; block_pos[1] = 3; block_pos[2] = 2; break;
			case 5:  block_pos[0] = 2; block_pos[1] = 1; block_pos[2] = 3; break;
			case 6:  block_pos[0] = 1; block_pos[1] = 2; block_pos[2] = 3; break;
		}

			grab_materials_ground(block_pos[color_task[0]-1],1);
			put_materials_car(1);
			grab_materials_ground(block_pos[color_task[1]-1],0);
			put_materials_car(2);
			grab_materials_ground(block_pos[color_task[2]-1],0);
			put_materials_car(3);

		Chassis_Go_Pos(1000, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 0, 1, 300);

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1900, 1040, 0, 0, 100);

		CRing_Dual_Calibration();  //ɫ��˫��У׼
		Update_OPS(1940, 1050, 0);

			grab_materials_car(1, 0);
			put_materials_ground(color_task[0],0);
			grab_materials_car(2, 0);
			put_materials_ground(color_task[1],0);
			grab_materials_car(3, 0);
			put_materials_ground(color_task[2],0);
			reset_posture();

			grab_materials_ground(color_task[0],1);
			put_materials_car(1);
			grab_materials_ground(color_task[1],0);
			put_materials_car(2);
			grab_materials_ground(color_task[2],0);
			put_materials_car(3);

		Chassis_Go_Pos(1900, 165, -90, 1, 300);

		identify_posture(2100, 2000);

		Chassis_Go_Pos(1450, 120, -90, 0, 100);  //����̨

		Chassis_Visual_Pos(1, 313, 190, -90, 0);

		put_block_turntable(1, color_task[0]);
		put_block_turntable(2, color_task[1]);
		put_block_turntable(3, color_task[2]);
		reset_posture();

		/* �ڶ��� */

		Chassis_Go_Pos(1050, 165, 0, 1, 300);  //T1·��

		Chassis_Go_Pos(1050, 1900, 0, 1, 300);  //�ּӹ���1

		Chassis_Go_Pos(1050, 1900, 90, 1, 200);  //�ּӹ���2

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1000, 1950, 90, 0, 300);

		Chassis_Visual_Pos(3, 328, 230, 6, 0);

		switch(VIS_RX.block_order)  //��ɫ��Ӧɫ��
		{
			case 1:  block_pos[0] = 3; block_pos[1] = 2; block_pos[2] = 1; break;
			case 2:  block_pos[0] = 3; block_pos[1] = 1; block_pos[2] = 2; break;
			case 3:  block_pos[0] = 2; block_pos[1] = 3; block_pos[2] = 1; break;
			case 4:  block_pos[0] = 1; block_pos[1] = 3; block_pos[2] = 2; break;
			case 5:  block_pos[0] = 2; block_pos[1] = 1; block_pos[2] = 3; break;
			case 6:  block_pos[0] = 1; block_pos[1] = 2; block_pos[2] = 3; break;
		}

			grab_materials_ground(block_pos[color_task[3]-1],1);
			put_materials_car(1);
			grab_materials_ground(block_pos[color_task[4]-1],0);
			put_materials_car(2);
			grab_materials_ground(block_pos[color_task[5]-1],0);
			put_materials_car(3);

		Chassis_Go_Pos(1000, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 90, 1, 300);

		Chassis_Go_Pos(1860, 1880, 0, 1, 300);

		identify_posture(1500, 1700);  //ʶ����̬

		Chassis_Go_Pos(1900, 1040, 0, 0, 100);

		CRing_Dual_Calibration();  //ɫ��˫��У׼
		Update_OPS(1950, 1050, 0);

			grab_materials_car(1, 0);
			put_materials_ground(color_task[3],0);
			grab_materials_car(2, 0);
			put_materials_ground(color_task[4],0);
			grab_materials_car(3, 0);
			put_materials_ground(color_task[5],0);
			reset_posture();

			grab_materials_ground(color_task[3],1);
			put_materials_car(1);
			grab_materials_ground(color_task[4],0);
			put_materials_car(2);
			grab_materials_ground(color_task[5],0);
			put_materials_car(3);

		Chassis_Go_Pos(1900, 165, -90, 1, 300);

		identify_posture(2100, 2000);

		Chassis_Go_Pos(1450, 120, -90, 0, 100);  //����̨

		Chassis_Visual_Pos(1, 310, 190, -90, 0);

		put_block_turntable(1, color_task[3]);
		put_block_turntable(2, color_task[4]);
		put_block_turntable(3, color_task[5]);
		reset_posture();

		Chassis_Go_Pos(200, 165, -90, 0, 300);

		Chassis_Go_Pos(0, 0, 0, 0, 3000);

		chassis_mode=2;    //ֹͣģʽ

		while(1){osDelay(100);}

	}
	#endif
}