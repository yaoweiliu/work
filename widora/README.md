#### 项目A

1. 移植`BoardService`
2. 自己写一个简易的`BoardService`，用`C++`语言来写，主要实现进程的管理、驱动的加载；可看情况是否要分为`Top half`和`Bottom halt`
3. 将自己实现的简易`BoardService`设置为开机自启(*`init`进程的`shell`脚本*)

#### 项目B

1. 编写外设驱动，包括但不限于`DHT22`、蜂鸣器、`OLED`、`GPS`等；
2. 学会使用`DTS`；
3. 把`OLED`驱动基于内核`framebuffer`框架再实现一遍
