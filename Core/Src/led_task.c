#include "cmsis_os.h"
#include "led_task.h"
#include "gpio.h"
#include "main.h"

LedManage_s ledInfo = {
	.ledColor = LED_COLOR_MAX,
	.ledStatus = LED_STATUS_MAX,
};

void LedTask(void *argument)
{
	while (1) {
		if (ledInfo.ledColor == LED_RED) {
			if (ledInfo.ledStatus == LED_OFF) {
				RED_LED_OFF;
			} else if (ledInfo.ledStatus == LED_ON) {
				RED_LED_ON;
			} else if (ledInfo.ledStatus == LED_BLINK) {
				
			} else if (ledInfo.ledStatus == LED_GRADIENT) {
				
			} else {
				
			}
				
		} else if (ledInfo.ledColor == LED_GREEN) {
			if (ledInfo.ledStatus == LED_OFF) {
				GREEN_LED_OFF;
			} else if (ledInfo.ledStatus == LED_ON) {
				GREEN_LED_ON;
			} else if (ledInfo.ledStatus == LED_BLINK) {
				
			} else if (ledInfo.ledStatus == LED_GRADIENT) {
				
			} else {
				
			}			
		}
		vTaskDelay(10);
	}
}
