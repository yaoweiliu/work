#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>

static void set_ds(void)
{
	s3c_gpio_cfgpin(EXYNOS4_GPX1(0), S3C_GPIO_OUTPUT);//config gpio with samsung machine. just like gpio_direction_input()
	gpio_set_value(EXYNOS4_GPX1(0), 1);
}

static void clear_ds(void)
{
	s3c_gpio_cfgpin(EXYNOS4_GPX1(0), S3C_GPIO_OUTPUT);
	gpio_set_value(EXYNOS4_GPX1(0), 0);
}

static int get_ds(void)
{
	s3c_gpio_cfgpin((0), S3C_GPIO_INPUT);

	return gpio_get_value(EXYNOS4_GPX1(0));
}

static void read_dht22(char *buf)
{
	int count = 0, s, i, j;

	memset(buf, 0, 5);

	set_ds();
	udelay(10);
	clear_ds();
	udelay(600);
	set_ds();

	while(get_ds());
	while(!get_ds());

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 8; j++) {
			do {
				s = get_ds();
				count++;
				udelay(2);
			}while(s && (count < 40));

			if(s != 0) {
				printk("time out: %d\n", __LINE__);
				return ;
			}

			count = 0;
			do {
				s = get_ds();
				count++;
				udelay(2);
			}while(!s && (count < 25));

			if(s == 0) {
				printk("time out: %d\n", __LINE__);
				return ;
			}

			udelay(40);
			if(get_ds()) {
				buf[i] |= 1 << (7 - j);
			}
		}
	}

	set_ds();
}

static __init int dht22_init(void)
{
	int conut = 100;
	char buf[5];
	int ret = 0;

	ret = gpio_request(EXYNOS4_GPX1(0), "dht22");
	if(ret)
		return -EBUSY;

	while(count--) {
		read_dht22(buf);
		if(buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) {
			printk("shidu: %d%%\n", (buf[0] << 8 | buf[1]));
			printk("wendu: %dC\n", (buf[2] << 8 | buf[3]));
		}
		ssleep(1);
	}

	return 0;
}

static __exit void dht22_exit(void)
{
	gpio_free(EXYNOS4_GPX1(0));
}

module_init(dht22_init);
module_exit(dht22_exit);

MODULE_LICENSE("GPL");
