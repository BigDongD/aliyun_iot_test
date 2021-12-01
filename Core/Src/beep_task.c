#include "cmsis_os.h"
#include "gpio.h"
#include "beep_task.h"
#include "main.h"

extern osMutexId_t beepCtrlMutex;

BeepStatus_e g_BeepStatus = BEEP_DEFAULT;

void BeepTask(void *argument)
{
	while (1) {
		if (osMutexAcquire(beepCtrlMutex, portMAX_DELAY) == osOK) {
			switch ((int)g_BeepStatus) {	
				case BEEP_SHORT_SOUND:
					BEEP_ON;
					osDelay(100);
					BEEP_OFF;
					g_BeepStatus = BEEP_DEFAULT;
					break;
				
				case BEEP_LONG_SOUND:
					BEEP_ON;
					osDelay(500);
					BEEP_OFF;
					g_BeepStatus = BEEP_DEFAULT;
					break;
				
				case BEEP_DOUBLE_SOUND:
					BEEP_ON;
					osDelay(100);
					BEEP_OFF;
					osDelay(100);
					BEEP_ON;
					osDelay(100);
					BEEP_OFF;
					g_BeepStatus = BEEP_DEFAULT;
					break;
				
				case BEEP_DEFAULT:
				default:
					BEEP_OFF;
					break;
			}
			osMutexRelease(beepCtrlMutex);
	    }
		vTaskDelay(10);
	}
}
