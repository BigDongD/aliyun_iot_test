#include "cmsis_os.h"
#include "led_task.h"
#include "gpio.h"
#include "main.h"

extern osMutexId_t ledCtrlMutex;

LedManage_s g_LedInfo = {
	.ledColor = LED_COLOR_DEFAULT,
	.ledStatus = LED_STATUS_DEFAULT,
};

void LedTask(void *argument)
{
	while (1) {
		if (osMutexAcquire(ledCtrlMutex, portMAX_DELAY) == osOK) {
			if (g_LedInfo.ledColor == LED_RED) {
				if (g_LedInfo.ledStatus == LED_OFF) {
					RED_LED_OFF;
				} else if (g_LedInfo.ledStatus == LED_ON) {
					RED_LED_ON;
				} else if (g_LedInfo.ledStatus == LED_BLINK) {
					RED_LED_ON;
					osDelay(200);
					RED_LED_OFF;
					osDelay(200);
				} else if (g_LedInfo.ledStatus == LED_GRADIENT) {

				} else {
				}
			} else if (g_LedInfo.ledColor == LED_GREEN) {
				if (g_LedInfo.ledStatus == LED_OFF) {
					GREEN_LED_OFF;
				} else if (g_LedInfo.ledStatus == LED_ON) {
					GREEN_LED_ON;
				} else if (g_LedInfo.ledStatus == LED_BLINK) {
					GREEN_LED_ON;
					osDelay(200);
					GREEN_LED_OFF;
					osDelay(200);
				} else if (g_LedInfo.ledStatus == LED_GRADIENT) {

				} else {

				}
			}
			osMutexRelease(ledCtrlMutex);
	    }
		vTaskDelay(10);
	}
}
