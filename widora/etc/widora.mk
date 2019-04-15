LINUXPATH :=/home/lyw/project/openwrt_widora/build_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/linux-ramips_mt7688/linux-3.18.29

CFLAGS += -c -g #-Wall -Werror

CPPFLAGS += -c -g #-Wall -Werror

LDFLAGS += -g -lrt -pthread
