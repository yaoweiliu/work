#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include "multi_button.h"

static struct Button btn1;
static timer_t timerid;
//struct Button btn2;
typedef void (*cb_function)(void);
static cb_function func = NULL;

static uint8_t read_button1_GPIO() 
{
	//return HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
	return 0;
}

/*
uint8_t read_button2_GPIO() 
{
	return HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin);
}
*/

static void BTN1_PRESS_DOWN_Handler(void* btn)
{
	//do something...
}

static void BTN1_PRESS_UP_Handler(void* btn)
{
	//do something...
}

static void timer_notify_cb(union sigval val)
{
	func();
}

static void sighandler(int id)
{
	printf("%s: id = %d\n", __func__, id);
	timer_delete(timerid);
	exit(0);
}

static void __timer_start(void (*function)(void), int level, int interval)
{
	struct sigevent sevp;
	struct itimerspec value;
	struct timespec spec;

	signal(SIGINT, sighandler);

	func = function;

	memset(&sevp, 0x0, sizeof(struct sigevent));
	memset(&value, 0x0, sizeof(struct itimerspec));

	sevp.sigev_notify = SIGEV_THREAD;
	sevp.sigev_notify_function = timer_notify_cb;

	timer_create(CLOCK_REALTIME, &sevp, &timerid);

	clock_gettime(CLOCK_REALTIME, &spec);
	value.it_value.tv_sec = spec.tv_sec + 2;
	value.it_value.tv_nsec = spec.tv_nsec + 0;
	value.it_interval.tv_sec = interval / 5;
	value.it_interval.tv_nsec = 0;
	timer_settime(timerid, TIMER_ABSTIME, &value, NULL);

	//sleep(6);
	//timer_delete(timerid);
}

int main(int argc, char const *argv[])
{
	button_init(&btn1, read_button1_GPIO, 0);
	//button_init(&btn2, read_button2_GPIO, 0);
	
	button_attach(&btn1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
	button_attach(&btn1, PRESS_UP,         BTN1_PRESS_UP_Handler);
#if 0
	button_attach(&btn1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler);
	button_attach(&btn1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);
	button_attach(&btn1, LONG_RRESS_START, BTN1_LONG_RRESS_START_Handler);
	button_attach(&btn1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler);

	button_attach(&btn2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
	button_attach(&btn2, PRESS_UP,         BTN2_PRESS_UP_Handler);
	button_attach(&btn2, PRESS_REPEAT,     BTN2_PRESS_REPEAT_Handler);
	button_attach(&btn2, SINGLE_CLICK,     BTN2_SINGLE_Click_Handler);
	button_attach(&btn2, DOUBLE_CLICK,     BTN2_DOUBLE_Click_Handler);
	button_attach(&btn2, LONG_RRESS_START, BTN2_LONG_RRESS_START_Handler);
	button_attach(&btn2, LONG_PRESS_HOLD,  BTN2_LONG_PRESS_HOLD_Handler);
#endif
	
	button_start(&btn1);
	//button_start(&btn2);
	
	//make the timer invoking the button_ticks() interval 5ms.
	//This function is implemented by yourself.
	__timer_start(button_ticks, 0, 5); 
	
	while(1) 
	{}

	return 0;
}
