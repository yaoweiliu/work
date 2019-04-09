KERNDIR:= /home/lyw/project/openwrt_widora/build_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/linux-ramips_mt7688/linux-3.18.29
CROSS_COMPILE := mipsel-openwrt-linux-

build::
	make -C $(KERNDIR) M=$(CWD) ARCH=mips CROSS_COMPILE=$(CROSS_COMPILE) modules

clean::
	make -C $(KERNDIR) M=$(CWD) ARCH=mips CROSS_COMPILE=$(CROSS_COMPILE) modules clean
