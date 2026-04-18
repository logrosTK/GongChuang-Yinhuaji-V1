#ifndef __TOWER_H__
#define __TOWER_H__

#include "stm32f4xx_hal.h"
#include "hcan.h"
#include "LobotServoController.h"

//�����߶�define����λ0.1mm
#define vehicle_h 1120  //��������߶�+�������̵׺�
#define table_h 730  //����̨�߶�
#define material_h_1 550  //���ϵ׶˵��Ͼ����߶�
#define material_h_2 150  //���ϵ׶˵��¾����߶�
#define claw_h_max 2030  //��צ�г�װ�õ��������߶�
#define claw_h_min 550  //��צ�г�װ�õ�������С�߶�
#define claw_r_max 2800  //��צ���ĵ�ת��������
#define claw_r_min 1200  //��צ���ĵ�ת����С����

#define G6220_Enable 1
#define G6220_Disable 2
#define G6220_Save_Zero_Position 3
#define G6220_Clear_Error 4
#define G6220_MIT 0
#define G6220_POZ 0x100
#define G6220_SPEED 0x200
#define G6220_Modo G6220_POZ
#define	G6220_CAN_ID 0X01
#define J4310_CAN_ID 0x01   /* J4310旋转轴CAN ID */
#define P_MAX 12.5f
#define V_MAX 45.f
#define T_MAX 10.f

#define	MOTOR35_CAN_ID 0X0300
#define	MOTOR35_Dir 0x00
#define	MOTOR35_Reduction 44.94

#define	MOTOR28_CAN_ID 0X0400
#define	MOTOR28_Dir 0x00
#define	MOTOR28_Reduction 3.189

/* 串口总线舵机 LFD-01M，ID=1，位置范围0-1000，时间单位ms（待实机标定） */
#define CLAW_SERVO_ID   1
#define CLAW_CL         200   /* 夹紧 */
#define CLAW_OP_C       500   /* 复位/传送带开张角 */
#define CLAW_OP_G       600   /* 绿色物料开张角 */
#define CLAW_OP_T       700   /* 转台物料开张角 */
#define CLAW_OP_M       800   /* 暂存台物料开张角 */
#define CLAW_MOVE_TIME  500   /* 默认运动时间ms */

#define PWM1_INTI       ((void)0)
#define LFD_01M_CL      moveServo(CLAW_SERVO_ID, CLAW_CL,   CLAW_MOVE_TIME)
#define LFD_01M_OP_C    moveServo(CLAW_SERVO_ID, CLAW_OP_C, CLAW_MOVE_TIME)
#define LFD_01M_OP_G    moveServo(CLAW_SERVO_ID, CLAW_OP_G, CLAW_MOVE_TIME)
#define LFD_01M_OP_T    moveServo(CLAW_SERVO_ID, CLAW_OP_T, CLAW_MOVE_TIME)
#define LFD_01M_OP_M    moveServo(CLAW_SERVO_ID, CLAW_OP_M, CLAW_MOVE_TIME)

struct Finish_Flag
{
	uint8_t MOTOR35_flag;
	uint8_t MOTOR28_flag;
	uint8_t G6220_flag;
};

extern struct Finish_Flag flag;

void G6220_Command(uint8_t CMD);
void Motor_Homing(uint16_t ID);
void TowerMotor_Init(void);
void G6220_Init(void);
void Tower_init(void);
void G6220_Position(int32_t position,uint16_t speed);
void Motor35_AbsPosition(uint32_t z,uint16_t speed);
void Motor28_AbsPosition(uint32_t y,uint16_t speed);
void Response_Handle(uint32_t ExtId,uint32_t StdId, uint8_t *rxBuf);
void Tower_Control(uint32_t yaw,uint16_t yaw_speed,uint32_t z,uint16_t z_speed,uint32_t y,uint16_t y_speed);






#endif
