#ifndef _LED_TASK_H_
#define _LED_TASK_H_

extern void LedTask(void *argument);

typedef enum {
	LED_RED = 0,
	LED_GREEN  = 1,
	LED_COLOR_DEFAULT,
}LedColor_e;

typedef enum {
	LED_OFF = 0,
	LED_ON = 1,
	LED_BLINK = 2,
	LED_GRADIENT = 3,
	LED_STATUS_DEFAULT,
}LedStatus_e;

typedef struct {
	LedColor_e ledColor;
	LedStatus_e ledStatus;
}LedManage_s;

extern LedManage_s g_LedInfo; 

#endif
