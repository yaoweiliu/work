#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include <string.h>

#define TICKS_INTERVAL	5 //ms
#define	DEBOUNCE_TICKS	3
#define SHORT_TICKS		(300 / TICKS_INTERVAL)
#define LONG_TICKS		(1000 / TICKS_INTERVAL)

typedef void (*BtnCallback)(void *);

typedef enum {
	PRESS_DOWN = 0,
	PRESS_UP,
	PRESS_REPEAT,
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_PRESS_START,
	LONG_PRESS_HOLD,
	NUMBER_OF_EVENT,
	NONE_PRESS
}PressEvent;

typedef struct Button {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3;
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  (*hal_button_level)(void);
	BtnCallback cb[NUMBER_OF_EVENT];
	struct Button *next;
}Button;

#ifdef __cplusplus
extern "C" {
#endif

void button_init(struct Button *handle, uint8_t (*pin_level)(),uint8_t active_level);
void button_attach(struct Button *handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button *handle);
int button_start(struct Button *handle);
void button_stop(struct Button *handle);
void button_ticks(void);

#ifdef __cplusplus
}
#endif

#endif
