/**
  ******************************************************************************
  * @author  PINK SAKURA
  * @version V1.0
  * @date    2024-08-08
  * @brief   输出任务
  ******************************************************************************
  * @attention
  * 邮箱：sakura.mail@qq.com
  ******************************************************************************
  */

#include "Output_task.h"
#include "io_devices.h"
#include "cmsis_os2.h"

uint8_t ready = 0;

void Output_init(void)
{
	LED_1_OFF();
	LED_2_OFF();
	// LED_3_OFF();        // 已移除，无LED3
	// Buzzer_OFF();       // 已移除，无蜂鸣器
	// SPI_OLED_Init();    // 已移除，无SPI OLED
	// SPI_OLED_Clear();   // 已移除，无SPI OLED
	// CAM_Light_OFF();    // 已移除，无补光灯
}

void Output_task(void)
{
	Output_init();
	// SPI_OLED_ShowString(16,0,(char *)"SW Ver 2.7.4",16,0);  // 已移除
	// SPI_OLED_ShowString(8,2,(char *)"System init ",16,0);   // 已移除
	// buzzer_rings(20, 50, 2);                                 // 已移除

	for(uint16_t i=1400; i>0 && ready==0; i--)
	{
		// if(i%100==0)
		//     SPI_OLED_ShowNum(104,2,i/100 - 1,2,16,0);       // 已移除
		osDelay(10);
		if (HAL_GPIO_ReadPin(Start_GPIO_Port, Start_Pin))
			break;
	}
	ready = 1;
	// SPI_OLED_ShowString(0,2,(char *)"      Reday     ",16,0); // 已移除

	while(1)
	{
		// SPI_OLED_Showdecimal(16,4,pos_x,6,1,16,0);  // 已移除
		// SPI_OLED_Showdecimal(16,6,pos_y,6,1,16,0);  // 已移除

		// if(control_mode==1)
		//     SPI_OLED_ShowString(0,2,(char *)"     RC Mode    ",16,1); // 已移除
		// else
		//     SPI_OLED_ShowString(0,2,(char *)"    AUTO Mode   ",16,1); // 已移除

		LED_1_TOG();   // 心跳闪烁，表示系统正常运行
		osDelay(500);
	}
}