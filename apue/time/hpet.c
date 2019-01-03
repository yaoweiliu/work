#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <linux/hpet.h>

extern void hpet_open_close(int, const char **);
extern void hpet_info(int, const char **);
extern void hpet_poll(int, const char **);
extern void hpet_fasync(int, const char **);
extern void hpet_read(int, const char **);

#include <sys/poll.h>
#include <sys/ioctl.h>

struct hpet_command {
	char *command;
	void (*func)(int argc, const char **argv);
} hpet_command[] = {
	{
		"open-close",
		hpet_open_close
	},
	{
		"info",
		hpet_info
	},
	{
		"poll",
		hpet_poll
	},
	{
		"fasync",
		hpet_fasync
	},
};

int main(int argc, char const *argv[])
{
	unsigned int i;

	argc--;
	argv++;

	if(!argc) {
		fprintf(stderr, "-hpet: requires command\n");
		return -1;
	}

	for(i = 0; i < (sizeof(hpet_command)/sizeof(hpet_command[0])); i++) {
		if(!strcmp(argv[0], hpet_command[i].command)) {
			argc--;
			argv++;
			fprintf(stderr, "-hpet: executing %s\n", hpet_command[i].command);
			hpet_command[i].func(argc, argv);
			return 0;
		}
	}

	fprintf(stderr, "do_hpet: command %s not implemented\n", argv[0]);

	return -1;
}

void hpet_open_close(int argc, const char **argv)
{
	int fd;

	if(argc != 1) {
		fprintf(stderr, "%s: device-name\n", __func__);
		return ;
	}

	fd = open(argv[0], O_RDONLY);
	if(fd < 0)
		fprintf(stderr, "%s: open failed\n", __func__);
	else
		close(fd);

	return ;
}
