# 适用于mbed os 6和MAX7219驱动的8*8点阵类

## 观前须知

- 有问题请在github页面提issue，这样可以让后来的人得到有效的参考
- 如果不是在issue里提出问题但是问题最终被以某种方式解决了，我还是建议去补一下issue

## 简单介绍

这是一个适用于mbed os 6的库，用于驱动MAX7219控制的8*8的点阵屏，使用的是SPI协议

这个库同时提供了硬件SPI和软件SPI两种选择，在我个人的使用中感觉软件SPI的适用范围更加广泛，所以放上来的库文件默认使用了软件SPI实现，如果要改为硬件SPI请修改头文件里对应的宏，具体方法会在后文指出

## 怎么使用这个库？

> 作者自己的编程环境是vscode上的platform IO，并使用其自带的本地编译器进行编译，没有在mbed studio及keil studio cloud上尝试，如果有问题还请提issue

1. 导入到你的项目目录
2. 记得include
3. 没了

### 一个最基本的helloworld程序

```cpp
#include "Matrix.h"
// 假定你使用D11作为MOSI，D13作为CLK，A3作为CS
matrix Mtrx(D11,D13,A3);
int main(void) {
    // 作者人工取模的笑脸
    // HelloWorld方法里自动进行了一次SendBuffer
    Mtrx.HelloWorld();
}
```

### 一个没那么基本的示例程序

```cpp
#include "Matrix.h"
matrix Mtrx(D11,D13,A3);
// 这个实际上是array<uint8_t,8>的别名
// 可以理解为uint8_t Frm[8];
matrix::frame Frm = {
    0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00,
};
int main(void) {
    // Mtrx.buffer其实就是一个长度为8的uint8_t数组，1代表对应位置亮，0代表对应位置灭
    // 数组序号对应行号，MSB到LSB分别是从左到右的灯珠
    Mtrx.buffer = Frm;
    Mtrx.SendBuffer();
}
```

### 需要特别提示的一点

这个库默认使用软件模拟SPI，也可以通过修改头文件使用硬件SPI

具体修改方法是：把Matirx.h文件的第23行的`#define _Matrix_8_Using_Simulated_SPI_`语句删除或注释掉

需要注意的是，软件SPI没有对引脚的要求，只要支持GPIO即可；硬件SPI需要对应上对应的SPI引脚组合

### 类的各个方法的介绍

#### `matrix(PinName Matrix_IO_MOSI, PinName Matrix_IO_CLK,  PinName Matrix_IO_CS)`

类的构造函数

你一般不会直接调用这个函数，如果不知道这是什么，可以当他不存在

- `Matrix_IO_MOSI` MOSI针脚
- `Matrix_IO_CLK` CLK针脚
- `Matrix_IO_CS` CS针脚

#### `buffer`

类中的缓冲区，用于暂时存储将要让点阵屏显示的图案

数据类型是`matrix::frame`，也就是`array<uint8_t,8>`

这个库没有提供修改buffer内容的函数，而是选择了让用户可以直接访问buffer，是因为我认为用户才知道他们想要什么

在修改后需要`SendBuffer`才能更改显示内容

#### `operator<<(const frame &data)`

返回值：对自身的引用（看不懂可以当不存在）

给个示例：**一个没那么基本的示例程序**里main里的内容其实可以改写成：

```cpp
int main(void) {
    Mtrx << Frm;
}
```

也就是，用data覆盖现有的buffer的值，然后自动SendBuffer

其实去.cpp里看一眼实现就清楚了

#### `SendBuffer(void)`

返回值：对自身的引用（看不懂可以当不存在）

将现在的buffer推送到点阵屏上

#### `ClearBuffer(void)`

清除掉buffer的值，但是不会自动SendBuffer

~~那不就是`buffer.fill(0)`吗~~

#### `Init_Setting(void)`

返回值：对自身的引用（看不懂可以当不存在）

将所有的设置调整到推荐配置，或者重置所有设置

一般不会调用这个函数，因为在构造函数里已经帮你调用过一次了

#### `Set_Intensity(float intensity = 1.f)`

通过设置占空比来改变矩阵灯珠的亮度

默认值是1.0

数据的允许范围是0.0到1.0，分别对应你所期望的亮度百分比

#### `Set_MatrixTest(bool mode)`

返回值：对自身的引用（看不懂可以当不存在）

开关MAX7219的DisplayTest模式

在InitSetting里默认关闭，如果需要使用请配合数据手册

#### `is_In_MatrixTest(void)`

返回值：是否在DisplayTest模式，如果是则返回true

#### `Set_Shutdown(bool mode)`

返回值：对自身的引用（看不懂可以当不存在）

开关MAX7219的Shutdown模式

在InitSetting里默认关闭，如果需要使用请配合数据手册

#### `is_In_Shutdown(void)`

返回值：是否在Shutdown模式，如果是则返回true

#### `HelloWorld(void)`

一个用于测试是否正常运行的样例，封装成了一个类方法

理论上会显示一个笑脸

会自动Sendbuffer，并且笑脸图案会覆盖现有的buffer的值

### 最后

关注DT5EDU公众号谢谢喵

By CATEDS

关注真弓快车！
