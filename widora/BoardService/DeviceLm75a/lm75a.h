#ifndef __LM75A_H__
#define __LM75A_H__

#include <linux/i2c.h>
#include <linux/timer.h>
#include <linux/input.h>
#include <linux/workqueue.h>

#define DEVNAME			"lm75a"
#define SLAVEADDR		0x48

#define	TEMP_MAX		100
#define	TEMP_MIN		0
#define	INPUT_PERIOD	1000 //1000ms

//register form datasheet
#define	LM75A_TEMP_REG	0x00
#define LM75A_CONF_REG	0x01
#define	LM75A_THYST_REG	0x02
#define	LM75A_TOS_REG	0x03

//mode with configure register form datasheet
#define	LM75A_SHUTDOWN			0x01
#define LM75A_NORMAL			0x00
#define LM75A_OS_INTR			0x02
#define	LM75A_OS_COMP			0x00
#define LM75A_OS_ACTIVE_HIGH	0x04
#define LM75A_OS_ACTIVE_LOW		0x00

#define	__unused				__attribute__((unused))

struct lm75a_pdata
{
	struct input_dev *input;
	struct timer_list timer;
	struct i2c_client *client;
	struct work_struct wq;
};

#endif
