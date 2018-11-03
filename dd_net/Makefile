#ccflags-y+=-Wfatal-errors
ccflags-y+=-DDEBUG
ccflags-y+=-DUSE_PLATFORM_DEVICE
ccflags-y+=-fmax-errors=5
#ccflags-y+=-DCTRACER_ON -include $M/ctracer.h
#ccflags-y+=-D USE_MISCDEV # uncomment to use single misc device instead char devices region

obj-m+= virnet.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all:	modules

modules:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules_install

app:app.o
	gcc -o $@ $^

clean:
	$(MAKE) -C $(KERNELDIR) M=$$PWD clean
	@#rm -rf *.o app *~ core  *.ko *.mod.c modules.order Module.symvers

_src = virnet.c

checkpatch:
	checkpatch.pl --no-tree --show-types --ignore LINE_CONTINUATIONS --terse -f $(_src) Makefile

checkpatch2:
	checkpatch.pl --no-tree --show-types --ignore LONG_LINE,LINE_CONTINUATIONS --terse -f $(_src) Makefile
