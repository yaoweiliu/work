LINUXPATH :=/home/lyw/project/openwrt_widora/build_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/linux-ramips_mt7688/linux-3.18.29

CFLAGS += -c -fmessage-length=0 -mlittle-endian -march=armv7-a \
		  -mtune=cortex-a8 -msoft-float -g -Wall -Werror -MD -MP \
		  -std=gnu99 -D_GNU_SOURCE -I/usr/local/include/libxml2

CPPFLAGS += -c -fmessage-length=0 -mlittle-endian -march=armv7-a \
			-mtune=cortex-a8 -msoft-float -g -Wall -Werror -MD -MP \
			-D__STDC_FORMAT_MACROS

LDFLAGS += -g -lrt -pthread -march=armv7-a -mtune=cortex-a8, -Wl,--hash-style=gnu -Wl,-O1
