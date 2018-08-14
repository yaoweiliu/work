obj-m+= uart.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all:	modules

modules:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules_install

clean:
	$(MAKE) -C $(KERNELDIR) M=$$PWD clean
