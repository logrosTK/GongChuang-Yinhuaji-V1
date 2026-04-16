#include "io_devices.h"

void LED_1_flashing(uint16_t delay_on, uint16_t delay_off, uint8_t times)
{
    for(uint8_t i = times; i > 0; i--)
    {
        LED_1_ON();
        osDelay(delay_on);
        LED_1_OFF();
        if (i > 1)
            osDelay(delay_off);
    }
}

void LED_2_flashing(uint16_t delay_on, uint16_t delay_off, uint8_t times)
{
    for(uint8_t i = times; i > 0; i--)
    {
        LED_2_ON();
        osDelay(delay_on);
        LED_2_OFF();
        if (i > 1)
            osDelay(delay_off);
    }
}