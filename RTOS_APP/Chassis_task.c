/**
  ******************************************************************************
  * @author  PINK SAKURA / Modified for ZDT 42 stepper CAN
  * @version V1.0
  * @date    2024-08-08
  * @brief   底盘控制 C 文件
  ******************************************************************************
  * @attention
  * 底盘电机驱动已从 UART4 串口协议改为 ZDT X_V2 CAN 速度控制
  * 电机地址：左前=0x01，右前=0x02，左后=0x03，右后=0x04
  * 速度单位：ZDT X_V2 vel 参数单位为 RPM（转/分）
  * 原始 SpeedTarget 单位换算：除以 0.238 得到整数速度值，
  * 再换算为 RPM：整数速度值 / SPEED_TO_RPM_RATIO
  ******************************************************************************
  */

#include "chassis_control.h"
#include "cmsis_os2.h"
#include "visual_identity.h"
#include "X_V2.h"

/* ZDT电机CAN地址 */
#define MOTOR_ADDR_LF   0x01    // 左前
#define MOTOR_ADDR_RF   0x02    // 右前
#define MOTOR_ADDR_LB   0x03    // 左后
#define MOTOR_ADDR_RB   0x04    // 右后

/* 加速度设置（单位：RPM/s，根据实际调试修改） */
#define MOTOR_ACC       200

float rcKpx  = 8,  rcKpy = 8,  rcKpz = 6;
float mKpx   = 2.3, mKpy = 2.3, mKpz = 9;
float vKpx   = 1.2, vKpy = 1.2, vKpz = 2.4;
float cvKpz  = 0.08;
int   last_Speed[4] = {0};

float XYVmax;
float ZVmax;
float XYVmin;
float ZVmin;

uint8_t in_pos     = 1;
uint8_t v_in_pos   = 1;
uint8_t near_pos   = 1;
uint8_t delay_pos  = 0;
uint8_t delay_turn = 0;
uint8_t delay_visual = 0;

float devx = 0;
float devy = 0;
float devz = 0;


void SpeedTarget_stop(void)
{
    SpeedTarget[0] = 0;
    SpeedTarget[1] = 0;
    SpeedTarget[2] = 0;
    SpeedTarget[3] = 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ZDT 42步进电机 CAN速度控制
//              原函数通过 UART4 发送串口协议，现改为调用 X_V2_Vel_Control()
//              speed > 0: 正转(dir=0)，speed < 0: 反转(dir=1)
//              速度幅值直接作为 RPM 传入，最大值与 XYVmax 对应
//              如实际转速不对请调整 SPEED_TO_RPM 比例系数
//-------------------------------------------------------------------------------------------------------------------
void SetMotorVoltageAndDirection(int MotorSpeed1, int MotorSpeed2, int MotorSpeed3, int MotorSpeed4)
{
    uint8_t addr[4]  = {MOTOR_ADDR_LF, MOTOR_ADDR_RF, MOTOR_ADDR_LB, MOTOR_ADDR_RB};
    int     speed[4] = {MotorSpeed1, MotorSpeed2, MotorSpeed3, MotorSpeed4};

    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t dir = 0;
        int     vel = speed[i];

        if (vel < 0)
        {
            dir = 1;
            vel = -vel;
        }

        /* X_V2_Vel_Control(addr, dir, acc, vel_rpm, sync_flag)
         * vel 单位：RPM，根据实际机械结构调试该值
         * snF=false 表示立即执行，不等待同步指令 */
        X_V2_Vel_Control(addr[i], dir, MOTOR_ACC, (float)vel, false);
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度限幅
//-------------------------------------------------------------------------------------------------------------------
void numerical_limit(float* value, float max, float min, float dead_zone)
{
    if (*value > dead_zone)
        *value += min;
    else if (*value < -dead_zone)
        *value -= min;
    if (*value > max)
        *value = max;
    else if (*value < -max)
        *value = -max;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      底盘遥控函数
//-------------------------------------------------------------------------------------------------------------------
void chassis_RC(short x, short y, short z)
{
    XYVmax = 800;
    ZVmax  = 600;

    int   Speed[4] = {0};
    devx = x;
    devy = y;
    devz = z;

    float vx1 = 0, vy1 = 0, vz = 0;

    vx1 = 1 * rcKpx * devx;
    numerical_limit(&vx1, XYVmax, 0, 5);

    vy1 = 1 * rcKpy * devy;
    numerical_limit(&vy1, XYVmax, 0, 5);

    vz = rcKpz * devz;
    numerical_limit(&vz, ZVmax, 0, 5);

    float v_ratio = 1;

    Speed[0] =   (int)(( vy1 + vx1 ) * v_ratio + vz);
    Speed[1] = - (int)(( vy1 - vx1 ) * v_ratio - vz);
    Speed[2] =   (int)(( vy1 - vx1 ) * v_ratio + vz);
    Speed[3] = - (int)(( vy1 + vx1 ) * v_ratio - vz);

    for (uint8_t i = 0; i < 4; i++)
    {
        SpeedTarget[i] = (int)(Speed[i] * 0.238f);
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      底盘定位系统位置环
//-------------------------------------------------------------------------------------------------------------------
void chassis_move(int x, int y, int z)
{
    XYVmax = 1600;
    ZVmax  = 750;
    XYVmin = 5;
    ZVmin  = 5;

    int Speed[4] = {0};
    devx = pos_x - x;
    devy = pos_y - y;
    if (z * zangle < 0 && abs(z) + fabs(zangle) > 180)
        devz = -(360 - abs(z) - fabs(zangle));
    else
        devz = z - zangle;

    float vx1 = 0, vy1 = 0, vx2 = 0, vy2 = 0, vz = 0;

    vy1 = cos(zangle * 3.141f / 180) * mKpy * devy;
    vx2 = sin(zangle * 3.141f / 180) * mKpy * devy;
    numerical_limit(&vy1, XYVmax, XYVmin, 5);
    numerical_limit(&vx2, XYVmax, XYVmin, 5);

    vy2 = sin(zangle * 3.141f / 180) * mKpx * devx;
    vx1 = cos(zangle * 3.141f / 180) * mKpx * devx;
    numerical_limit(&vy2, XYVmax, XYVmin, 5);
    numerical_limit(&vx1, XYVmax, XYVmin, 5);

    vz = mKpz * devz;
    numerical_limit(&vz, ZVmax, 0, 5);

    Speed[0] = - (int)(  vy1 - vy2 +  vx1 + vx2 +  vz);
    Speed[1] =   (int)(  vy1 - vy2 -  vx1 - vx2 -  vz);
    Speed[2] = - (int)(  vy1 - vy2 -  vx1 - vx2 +  vz);
    Speed[3] =   (int)(  vy1 - vy2 +  vx1 + vx2 -  vz);

    for (uint8_t i = 0; i < 4; i++)
    {
        if (Speed[i] > 0 && Speed[i] > last_Speed[i])
            Speed[i] = last_Speed[i] + 20;
        if (Speed[i] < 0 && Speed[i] < last_Speed[i])
            Speed[i] = last_Speed[i] - 20;
        SpeedTarget[i] = (int)(Speed[i] * 0.238f);
        last_Speed[i]  = Speed[i];
    }

    if ((devx < 100 && devx > -100) && (devy < 100 && devy > -100) && (devz < 30 && devz > -30))
        near_pos = 1;
    if ((devx < 60 && devx > -60) && (devy < 60 && devy > -60) && (devz < 15 && devz > -15))
        delay_pos++;
    else
        delay_pos = 0;

    if (delay_pos > 10)
        in_pos = 1;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      底盘视觉微调函数
//-------------------------------------------------------------------------------------------------------------------
void visual_pos_adj(uint8_t target, int x, int y, int z)
{
    XYVmax = 80;
    ZVmax  = 35;
    XYVmin = 3;
    ZVmin  = 2;

    int Speed[4] = {0};

    switch (target)
    {
        case 1:  // 物料盘
            devy = VIS_RX.tray_center_x - x;
            devx = VIS_RX.tray_center_y - y;
            break;
        case 2:  // 色环
            devy = VIS_RX.rgb_circle_x - x;
            devx = VIS_RX.rgb_circle_y - y;
            break;
        case 3:  // 码垛时色环
            devy = VIS_RX.rgb_circle_x - x;
            devx = VIS_RX.rgb_circle_y - y;
            break;
        case 4:  // 色环粗校准
            devy = VIS_RX.rgb_circle_x - x;
            devx = VIS_RX.rgb_circle_y - y;
            break;
    }

    if (target >= 2)
        devz = VIS_RX.rgb_circle_z - z;
    else
    {
        if (z * zangle < 0 && abs(z) + fabs(zangle) > 180)
            devz = -(360 - abs(z) - fabs(zangle));
        else
            devz = z - zangle;
    }

    float vx1 = 0, vy1 = 0, vx2 = 0, vy2 = 0, vz = 0;

    vy1 = vKpy * devy;
    numerical_limit(&vy1, XYVmax, XYVmin, 0);
    numerical_limit(&vx2, XYVmax, XYVmin, 0);

    vx1 = vKpx * devx;
    numerical_limit(&vy2, XYVmax, XYVmin, 0);
    numerical_limit(&vx1, XYVmax, XYVmin, 0);

    vz = vKpz * devz;
    numerical_limit(&vz, ZVmax, 0, 0);

    Speed[0] = - (int)(  vy1 - vy2 +  vx1 + vx2 +  vz);
    Speed[1] =   (int)(  vy1 - vy2 -  vx1 - vx2 -  vz);
    Speed[2] = - (int)(  vy1 - vy2 -  vx1 - vx2 +  vz);
    Speed[3] =   (int)(  vy1 - vy2 +  vx1 + vx2 -  vz);

    for (uint8_t i = 0; i < 4; i++)
    {
        if (Speed[i] > 0 && Speed[i] > last_Speed[i])
            Speed[i] = last_Speed[i] + 20;
        if (Speed[i] < 0 && Speed[i] < last_Speed[i])
            Speed[i] = last_Speed[i] - 20;
        SpeedTarget[i] = (int)(Speed[i] * 0.238f);
        last_Speed[i]  = Speed[i];
    }

    switch (target)
    {
        case 1:
            if ((devx < 4 && devx > -4) && (devy < 4 && devy > -4) && (devz < 2 && devz > -2))
                delay_visual += 2;
            else
                delay_visual = 0;
            break;
        case 2:
            if ((devx < 2 && devx > -2) && (devy < 2 && devy > -2) && (devz < 2 && devz > -2))
                delay_visual++;
            else
                delay_visual = 0;
            break;
        case 3:
            if ((devx < 4 && devx > -4) && (devy < 4 && devy > -4) && (devz < 3 && devz > -3))
                delay_visual += 2;
            else
                delay_visual = 0;
            break;
        case 4:
            if ((devx < 10 && devx > -10) && (devy < 10 && devy > -10) && (devz < 6 && devz > -6))
                delay_visual += 20;
            else
                delay_visual = 0;
            break;
    }

    if (delay_visual > 200)
        v_in_pos = 1;
}