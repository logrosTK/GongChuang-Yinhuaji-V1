#ifndef __IO_DEVICES__
#define __IO_DEVICES__

#include "main.h"
#include "cmsis_os2.h"

// LED_1 °˙ ∞Â‘ÿLED0 (PF9, µÕµÁ∆Ω¡¡)
#define LED_1_ON()   HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED_1_OFF()  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
#define LED_1_TOG()  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)

// LED_2 °˙ ∞Â‘ÿLED1 (PF10, µÕµÁ∆Ω¡¡)
#define LED_2_ON()   HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED_2_OFF()  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED_2_TOG()  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)

void LED_1_flashing(uint16_t delay_on, uint16_t delay_off, uint8_t times);
void LED_2_flashing(uint16_t delay_on, uint16_t delay_off, uint8_t times);

#endif  /* __IO_DEVICES__ */