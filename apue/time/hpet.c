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

	fprintf(stderr, "%s: before argc = %d\n", __func__, argc);
	for(i = 0; i < argc; i++) {
		fprintf(stderr, "%s: before argv[%d] = %s\n", __func__, i, argv[i]);
	}

	argc--;
	argv++;

	fprintf(stderr, "%s: after argc = %d\n", __func__, argc);
	for(i = 0; i < argc; i++) {
		fprintf(stderr, "%s: after argv[%d] = %s\n", __func__, i, argv[i]);
	}

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


void hpet_info(int argc, const char **argv)
{
	struct hpet_info info;
	int fd;

	if(argc != 1) {
		fprintf(stderr, "%s: device-name, argc = %d\n", __func__, argc);
		return ;
	}

	fd = open(argv[0], O_RDONLY);
	if(fd < 0) {
		fprintf(stderr, "%s: open of %s failed\n", __func__, argv[0]);
		return ;
	}

	if(ioctl(fd, HPET_INFO, &info) < 0) {
		fprintf(stderr, "%s: failed to get info\n", __func__);
		goto out;
	}

	fprintf(stderr, "%s: hi_irqfreq 0x%lx hi_flags 0x%lx ", __func__, info.hi_ireqfreq, info.hi_flags);
	fprintf(stderr, "%s: %d hi_timer %d\n", __func__, info.hi_hpet, info.hi_timer);

out:
	close(fd);
	return ;
}

void hpet_poll(int argc, const char **argv)
{
	unsigned long freq;
	int iterations, i, fd;
	struct pollfd pfd;
	struct hpet_info info;
	struct timeval stv, etv;
	struct timezone tz;
	long usec;

	if(argc != 3) {
		fprintf(stderr, "%s: device-name freq iterations\n", __func__);
		return ;
	}

	freq = atoi(argv[1]);
	iterations = atoi(argv[2]);

	fd = open(argv[0], O_RDONLY);
	if(fd < 0) {
		fprintf(stderr, "%s: open of %s failed\n", __func__, argv[0]);
		return ;
	}

	if(ioctl(fd, HPET_IRQFREQ, freq) < 0) {
		fprintf(stderr, "%s: HPET_IRQFREQ failed\n", __func__);
		goto out;
	}

	if(ioctl(fd, HPET_INFO, &info) < 0) {
		fprintf(stderr, "%s: failed get info\n", __func__);
		goto out;
	}

	fprintf(stderr, "%s: info.hi_flags 0x%lx\n", __func__, info.hi_flags);

	if(info.hi_flags && (ioctl(fd, HPET_EPI, 0) < 0)) {
		fprintf(stderr, "%s: HPET_EPI failed\n", __func__);
		goto out;
	}

	if(ioctl(fd, HPET_IE_ON, 0) < 0) {
		fprintf(stderr, "%s: HPET_IE_ON failed\n", __func__);
		goto out;
	}

	pfd.fd = fd;
	pfd.events = POLLIN;

	for(i = 0; i < iterations; i++) {
		pfd.revents = 0;
		gettimeofday(&stv, &tz);
		if(poll(&pfd, 1, -1) < 0)
			fprintf(stderr, "%s: poll failed\n", __func__);
		else {
			long data;

			gettimeofday(&etv, &tz);
			usec = stv.tv_sec * 1000000 + stv.tv_usec;
			usec = (etv.tv_sec * 1000000 + etv.tv_usec) - usec;

			fprintf(stderr, "%s: expired time = 0x%lx\n", __func__, usec);

			fprintf(stderr, "%s: revents = 0x%x\n", __func__, pfd.revents);

			if(read(fd, &data, sizeof(data)) != sizeof(data)) {
				fprintf(stderr, "%s: read failed\n", __func__);
			}
			else
				fprintf(stderr, "%s: data 0x%lx\n", __func__, data);
		}
	}

out:
	close(fd);
	return ;
}

static int hpet_sigio_count;

static void hpet_sigio(int val)
{
	fprintf(stderr, "%s: called\n", __func__);
	hpet_sigio_count++;
}

void hpet_fasync(int argc, const char **argv)
{
	unsigned long freq;
	int iterations, i, fd, value;
	sig_t oldsig;
	struct hpet_info info;

	hpet_sigio_count = 0;
	fd = -1;

	if((oldsig = signal(SIGIO, hpet_sigio)) == SIG_ERR) {
		fprintf(stderr, "%s: failed to set signal handler\n", __func__);
		return ;
	}

	if(argc != 3) {
		fprintf(stderr, "%s: device-name freq iterations\n", __func__);
		goto out;
	}

	fd = open(argv[0], O_RDONLY);
	if(fd < 0) {
		fprintf(stderr, "%s: failed to open\n", __func__);
		return ;
	}

	if((fcntl(fd, F_SETOWN, getpid()) == 1) || ((value = fcntl(fd, F_GETFL)) == 1) || (fcntl(fd, F_SETFL, value | O_ASYNC) == 1)) {
		fprintf(stderr, "%s: fcntl failed\n", __func__);
		goto out;
	}

	freq = atoi(argv[1]);
	iterations = atoi(argv[2]);

	if(ioctl(fd, HPET_IRQFREQ, freq) < 0) {
		fprintf(stderr, "%s: HPET_IRQFREQ failed\n", __func__);
		goto out;
	}

	if(ioctl(fd, HPET_INFO, &info) < 0) {
		fprintf(stderr, "%s: HPET_INFO failed\n", __func__);
		goto out;
	}

	fprintf(stderr, "%s: info.hi_flags 0x%lx\n", __func__, info.hi_flags);

	if(info.hi_flags && (ioctl(fd, HPET_EPI, 0) < 0)) {
		fprintf(stderr, "%s: HPET_EPI faild\n", __func__);
		goto out;
	}

	if(ioctl(fd, HPET_IE_ON, 0) < 0) {
		fprintf(stderr, "%s: HPET_IE_ON failed\n", __func__);
		goto out;
	}

	for(i = 0; i < iterations; i++) {
		(void)pause();
		fprintf(stderr, "%s: count = %d\n", __func__, hpet_sigio_count);
	}

out:
	signal(SIGIO, oldsig);
	if(fd >= 0)
		close(fd);
	return ;
}
