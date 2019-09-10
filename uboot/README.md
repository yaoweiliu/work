uboot调试技巧之点灯：

platform: ti, TMDSSK3358, am335x

sdk: PROCESSOR-SDK-LINUX-AM335X, v06.00

func: 首先查阅原理图，搜索关键字`led`，确定要点亮的是`d4`，接到主芯片的AM335X_GPIO_LED2上，对应gpio为gpio1_6(第一组的第6个引脚)；其次查阅datasheet的memory map章节，确定gpio1的基地址为`0x4804C000`，事先要知道每一组gpio都有一组相同的寄存器(地址不一样)，在ti的手册中表现为GPIO Registers章节。读手册知道寄存器GPIO_OE(`0x4804C000+0x134`)是控制gpio的输入输出配置(写0输出，写1输入)，寄存器GPIO_DATAOUT(`0x4804C000+0x13c`)是设置用户想要给定的值，比如某一bit置1或清零。因为我们要操作gpio1_6，所以将GPIO_OE的bit6清0，配置为输出；之后将GPIO_DATAOUT的bit6清0再置1，此时即可点亮led。在uboot的合适位置处添加代码，即可确定代码的执行位置。

uboot启动：

一般uboot的启动分为三部分，第一层内嵌到soc内的rom[片内内存]中(rom code)，被称为一级引导程序；第二层为spl image(MLO)，被称为二级引导程序；第三层为uboot.image，被称为三级引导程序，负责kernel的加载等工作。
使用spl的uboot工作流程: a. ROM code加载SPL并运行；b. SPL进行必要的初始化之后（sdram，ddr等系统存储的配置），加载u-boot并运行；c. u-boot进行后续的操作。
入口：uboot/arch/arm/cpu/armv7/start.S/reset()--->uboot/arch/arm/lib/crt0.S/_main();这些都是汇编层的代码，在_main中，主要先调用uboot/common/init/board_init.c/board_init_f_alloc_reserve()/board_init_f_init_reserve()函数为调用uboot/common/board_f.c/board_init_f()做准备工作，完成board_init_f()的调用之后最后调用uboot/common/board_r.c/board_init_r()。

cpu--->arch--->board--->machine