#include "../oled12864_define.h"
#include "code_ch.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define DEV_PATH "/dev/OLED12864"

void write_16X16(int fd, char x, char y, char c)
{
	char addr[2];
	unsigned char wm=0;
    unsigned int  adder=32*c;

	printf("write %c\n", c);

	addr[0] = x;
	addr[1] = y;
	ioctl(fd, IOCTL_CMD_SET_ADDR, addr);
	for(wm = 0; wm < 16; wm++) {
		write(fd, &C16x16[adder], 1);
		adder += 1;
	}
	
	addr[1]+=1;
	ioctl(fd, IOCTL_CMD_SET_ADDR, addr);
	for(wm = 0; wm < 16; wm++) {
		write(fd, &C16x16[adder], 1);
		adder += 1;
	}
}

void draw_picture(int fd, char c)
{
	write(fd, &c, 1);
}

void OLED_DrawBMP(int fd,unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP16x16[])
{
	unsigned int j=0;
	unsigned char x,y;
	char addr[2];

	if(y1%8==0)
		y = y1/8;
  	else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		addr[0] = x0;
		addr[1] = y;
		ioctl(fd, IOCTL_CMD_SET_ADDR, addr);

    	for(x=x0;x<x1;x++)
		{
			draw_picture(fd, BMP16x16[j++]);
		}
	}
}

int main(int argc, char **argv)
{
	int fd, i;
	char fill_data = 0x00;
	
	printf("oled12864 test\n");

	fd = open(DEV_PATH, O_RDWR);
	ioctl(fd, IOCTL_CMD_FILL_SCREEN, &fill_data);
	while(1) {
		//display chinese
		for(i = 0; i < 7; i++)
			write_16X16(fd, 10+i*16, 0, i);
		
		//OLED_DrawBMP(fd, 0,0,128,8,(unsigned char *)BMP16x16);
		break;
	}

	close(fd);

	return 0;
}






