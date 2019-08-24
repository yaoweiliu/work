dts见dts/WIDORA32128.dts
Enable framebuffer:

make menuconfig
    ->Device Drivers
      ->Graphics support
        ->Frame buffer Devices
        widora 3.18.29 需要enable Direct Rendering Manager,参考openwrt_widora/build_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/linux-ramips_mt7688/linux-3.18.29/drivers/video/Kconfig
          -\*-Support for frame buffer devices--->
          ......
          <\*>Solomon SSD1307 framebuffer support

