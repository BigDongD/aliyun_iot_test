#ifndef _BEEP_TASK_H_
#define _BEEP_TASK_H_

extern void BeepTask(void *argument);

typedef enum {
	BEEP_SHORT_SOUND = 0,
	BEEP_LONG_SOUND,
	BEEP_DOUBLE_SOUND,
	BEEP_DEFAULT,
}BeepStatus_e;

extern BeepStatus_e g_BeepStatus;

#endif
