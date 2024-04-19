# MFRC522

| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- |

### 内部寄存器

```txt
-- CommandReg 启动和停止命令的执行 
-- ComIrqReg 包含中断请求标志 
-- ErrorReg 错误标志，指示执行的上个命令的错误状态 
-- Status2Reg 包含接收器和发送器的状态标志 
-- FIFODtataReg 64 字节 FIFO 缓冲区的输入和输出 
-- FIFOLevelReg 指示 FIFO 中存储的字节数 
-- ControlReg 不同的控制寄存器 
-- BitFramingReg 面向位的帧的调节 
-- CollReg RF 接口上检测到的第一个位冲突的位的位置 
-- ModeReg 定义发送和接收的常用模式 
-- TxModeReg 定义发送过程的数据传输速率
-- TxControlReg 控制天线驱动器管脚 TX1 和 TX2 的逻辑特性 
-- TModeRegTPrescalerReg定义内部定时器的设置

```

## 使用 SPI 通信

MFRC522 引脚连接:
|      |    |ESP32c6 |
| ---- |----| --------|
| Host |    | SPI2    |
|**RST**|复位引脚，高电平有效|3.3V |
|IRQ|中断引脚，悬空不使用|  |
| VCC |              | 3.3V |
| GND |              | GND |
| MOSI |             | 5 |
| MISO |             |22 |
| SDA |SPI 片选(nss)  | 21 |
| SCK |SPI 时钟线     | 4  |

使用引脚前需查看手册，该引脚是否被推荐使用

连续读取时，需要由远到近移动卡片

## M1卡

常用的IC卡一般是M1卡，也称为S50卡，购买RC522刷卡模块送的白卡，蓝色钥匙扣、公交卡、地铁卡都是S50卡。S50卡内部有16个分区，每分区有AB两组密码，总容量为8Kbit。

- 其中第0扇区的块0是用于存放厂商代码的，已经固化，不可更改，为32位（4Bytes）；
- 每个扇区的块0、块1和块2位数据块，可用于存储数据，每块**16个字节**（只有S50卡是这样）；
- 每个扇区的块3位控制块，包含了密码A、存取控制、密码B，具体结构如下图所示；
- 空卡时的密码A 和密码B 均为“OxFFFFFF”，由于A 密码不可读，读出的数据**显示为“Ox000000”。**
- 在空卡默认读写权限下可以利用密码A 对所有块进行读写操作，以及更改各块的读写权限
- 在验证命令期间，MF RC500 是从其内部密码缓冲区（key buffer）读取密码。因此，用户必须保证在执行Authen1 命令前就已经将密码放到key buffer 中了。

|密码A(6Bytes) |存储控制(4Bytes)|密码B(6Bytes)|
|--------------|------------|----------------|

**M1卡分为16个扇区，每个扇区由4块（0、1、2、3）组成。在实际操作时，将16个扇区分为64个块，按绝对地址编号为0-63进行访问，也就是程序里需要填块的位置时，范围是0~63**

**每个块的大小是16字节，每个扇区里有3个数据块，数据块可以存放自己的自定义数据。**

对 MF RC500的控制，实现 RFID 卡的读写操作。
（1）设置 MF RC500 的状态。
（2）发送命令，要求 MF RC500 执行相应的动作。
（3）通过读 MF RC500 的状态标志来监测 MF RC500 的工作情况。
无论上述的哪一种方式，都是通过读/写 MF RC500 的寄存器来实现的：配置 MF
RC500 就是设置寄存器的某些位；执行命令要向命令寄存器写入命令代码以及通过
FIFO 缓冲区寄存器向缓冲区写入命令参数；监测 MF RC500即读状态寄存器的标志位。
因此，读写寄存器是所有操作的基础。

### 工作原理

读写器向M1卡发一组固定频率的电磁波，卡片内有一个 LC串联谐振电路，其频率与读写器发射的频率相同，在电磁波的激励下，LC谐振电路产生共振，从而使电容内有了电荷，在这个电容的另一端，接有一个单向导通的电子泵，将电容内的电荷送到另一个电容内储存，当所积累的电荷达到2V时，此电容可做为电源为其它电路提供工作电压，将卡内数据发射出去或接取读写器的数据。

### M1卡控制字

```markdwon
-- 每个字节的bit7控制block3，Bit6控制block2，Bit5控制block1    -- Bit4控制block0，Bit3 ~ 0其实就是bit7 ~ 4取反的结果
-- Block3出厂默认 ff ff ff ff ff ff    ff 07 80 69    ff ff ff ff ff ff
```

## 相关知识

### ISO 144443-A协议

- 物理特性：规定了接近式卡（PICC）的物理特性
- 频谱功率和信号接口：规定了再接近式耦合设备（PCD）和接近式卡（PICC）之间提供功率和双向通信的场的性质与特征
- 初始化和防冲突算法：描述了PICC进入PCD工作场的轮询；在PCD和PICC之间通信的初始阶段期间所使用的字节格式、帧和定时；初始REQ和ATQ命令内容；探测方法和与几个卡（防冲突）中的某一个通信的方法；初始化PICC和PCD之间的通信所需要的其他参数；容易和加速选择在应用准则基础上的几个卡中的一个（即最需要处理的一个）的任选方法
- 通讯协议：规定了以无触点环境中的特殊需要为特色的半双工传输协议，并定义了协议的激活和停活序列

### ISO 144443术语及缩写

```
-- 接近式卡 Proximity card（PICC）
-- 接近式耦合设备 Proximity coupling device（PCD）
-- 防冲突环 anticollision loop -- 比特冲突检测协议 bit collision detection protocol
-- 冲突 collision
-- 帧 frame
-- REQA：请求命令，类型A（Request To Command, Type A）
-- REQB：请求命令，类型B（Request To Command, Type B）
-- ATQA：请求应答，类型A（Answer To Request, Type A）
-- ATQB：请求应答，类型B（Answer To Request, Type B）
-- NVB：有效位的数目（Number of Valid Bits）
```

**规定了非接触的半双工的块传输协议并定义了激活和停止协议的步骤。这部分传输协议同时适用于A型卡和B型卡**

基于14443-A的操作帧格式：

    -- 请求卡 ：0x26 
    -- 唤醒所有卡 ：0x52
    -- 防冲突 ：0x93，0x20 得到卡ID
    -- 选择卡片 ：0x93，0x70， ID1,ID2,ID3,ID4， checksum， CRC16 

## Notes

If you meet timeout issues, please check your connections.