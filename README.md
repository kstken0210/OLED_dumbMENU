# Simple Menu Base on OLED & STM32
**Introduction**
A sophisticated OLED menu interface with smooth animations and Chinese character support, developed by a Year 2 Microelectronic Engineering student.

**✨ Features**
**🌀 Smooth Scrolling Effects**
Reverse lighting transition between menu lines
Seamless scrolling without abrupt jumps
Visually pleasing line-to-line transitions

**🎬 Animated Interface**
Opening animation: Clip from 非人哉 (Chinese anime)
Closing animation: Progress bar visualization
Pet mode: Animated drunk fox demonstration

**🎵 Music & Karaoke System**
Music app mode with lyrics display
Karaoke mode with gradient effect (漸變)
Real-time lyrics representation

**Here is license for the OLED Module I base on**  
~~~~~~~
  * 本程序由江协科技创建并免费开源共享
  * 你可以任意查看、使用和修改，并应用到自己的项目之中
  * 程序版权归江协科技所有，任何人或组织不得将其据为己有
  * 程序名称：				0.96寸OLED显示屏驱动程序（4针脚I2C接口）
  * 程序创建时间：			2023.10.24
  * 当前程序版本：			V2.0
  * 当前版本发布时间：		2024.10.20
~~~~~~~~
**OLED Module Modifications**
I modified a bit on the OLED module that can fit 12*12 Chinese, can take a look OLED.c line 697-709 and OLED_Data.c line633 and OLED_Data.h line 26-38

**Key Components**
Microcontroller: STM32F103c8t6
Display: OLED module with custom firmware SSD1306
Input: Rotary encoder/button interface
Storage: W25Q64 flash memory for animations

**Coming Soon**
Video demonstration of the complete menu system

**Other**
Gimbal control system(doing)

**Good picture**
![](https://i.pinimg.com/1200x/08/fd/b3/08fdb3b3a2a3d9e231460791c74f079e.jpg)
