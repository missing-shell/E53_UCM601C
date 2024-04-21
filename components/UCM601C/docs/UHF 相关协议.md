# 超高频(UHF)RFID协议

## G2协议

全称为ISO/IEC 18000-6C，也被称为EPC Class1 Gen2。这是一个国际标准，用于定义在860MHz至960MHz频段下电子标签与读写器之间的空气接口和数据通信规范。G2协议允许标签在全球任何地方被对应协议的读写器读取。

### 特点

    **跳频发射**：超高频读写器采用跳频发射方式，不受不同区域无线电频段变化的影响。
    **防冲突性能**：标签具备防冲突性能，能在读卡器密集的环境中运行。
    **读/写现场可编程性**：标签存储器可现场编程，提高了灵活性。
    **更快的标签读/写速度**：与其他协议相比，G2协议提供了更快的数据传输速率。

## 标签

### UHF RFID 标签存储器逻辑上分为四个区域

    保留存储区(Reserved memory) 包括杀死和访问密码(kill and access passwords)，用于控制标签的灭活和数据访问权限。
    EPC存储区(EPC memory) 包括一个CRC-16，协议控制位（ProtocolControl（PC） bits），以及一个用于识 别标签所附着的或即将附着的物品的编码( 如EPC，以后都称为EPC)，
    TID存储区(TID memory) 包括一个8-bit ISO/IEC 15963分配类别标识 符(EPCglobal的为111000102)，还可以 包括标签数据和厂商数据。
    用户存储区(User memory) 允许存储用户专用的数据信息。存储器的结构由用户自己定义。

### 标签操作：管理标签群

    询问机采用选择、盘存及访问三个基本操作来管理标签群。每个操作均由一个或一个以上的命令组成。这三个基本的定义如下：
    1、选择：询问机选择标签群以便于盘存和访问的过程。询问机可以一个或一个以上的Select命令在盘存之前选择特定的标签群。
    2、盘存（清点）：询问机识别标签的过程。询问机在四个通话的其中一个通话中传输Query命令，开始一个盘存周期。一个或一个以上的标签可以应答。询问机检查某个标签应答，请求该标签发出PC、EPC和CRC-16。同时只在一个通话中进行一个盘存周期。
    3、访问：询问机与各标签交易(读取或写入标签)的过程。访问前必须要对标签进行识别。访问由多个命令组成，其中有些命令执行R=＞T链的一次活页加密。

### 标签状态

    - 当标签收到连续波(CW)照射上电(Power-up)以后, 标签可处于下列七种状态之一。
    - Ready 状态：在进入到一个射频激活区域之后，标签如果没有被killed，则将进入readdy状态。
    - Arbitrate 状态：表示那些参与到当前inventory round中,但是时隙计数器值非零的那部分标签。
    - Reply状态：当时隙计数器到达0000h的时候，标签转入reply状态 ,标签进入到reply状态后会反射一个RN16。
    - Acknowledge状态：如果reply状态下的标签收到一个有效的ACK，它将转入acknowledged状态，返回他的PC，EPC以及CRC–16。
    - Open 状态：标签处于open状态可以执行除了lock以外的所有访问命令。
    - secured 状态：处于ssecured状态的标签可以执行所有的访问命令。
    - Killed状态：处于open或者secured状态的标签，在接收到一个带有有效的非零kill密码以及有效的handle的Kill指令后会转入到killed状态。

## 命令分类

- 从命令体系架构和扩展性角度，分为Mandatory(必备的)，Optional(可选的)，Proprietary(专有的)和Custom(定制的)四类。

- 从使用功能上看，分为标签Select(选取)，Inventory(盘点)和Access(存取)命令三类，此外还为了以后命令扩展，预留了长短不同的编码待用。

### 从命令体系架构和扩展性角度

#### 必备的(Mandatory)命令

    符合G2协议的标签和读写器，应该支持必备的命令有十一条：

        Select(选择)
        Query(查询)
        QueryAdjust(调节查询)
        QueryRep(重复查询)
        ACK(EPC答复)
        NAK(转向裁断)
        Req_RN(随机数请求)
        Read(读)
        Write(写)
        Kill(灭活)
        Lock(锁定)

#### 可选的(Optional)命令

    符合G2协议的标签和读写器，可选的命令有三条：Access(访问)，BlockWrite(块写)，BlockErase(块擦除)。

#### 专有的(Proprietary)命令

    专有的命令一般用于制造目的，如标签内部测试等，标签出厂后这样的命令应该永久失效。

#### 定制的(Custom)命令

    可以是制造商自己定义而开放给用户使用的命令，如Philips公司提供有:BlockLock(块锁定)，ChangeEAS(改EAS状态)，EASAlarm(EAS报警)等命令(EAS是商品电子防盗窃系统Electronic Article Surveillance的缩写)。

### 从功能角度

#### 选取(Select)类命令

    仅有一条：Select，是必备的。标签有多种属性，基于用户设定的标准和策略，使用Select命令，改变某些属性和标志人为选择或圈定了一个特定的标签群，可以只对它们进行盘点识别或存取操作，这样有利于减少冲突和重复识别，加快识别速度。

#### 盘点(Inventory)类命令

有五条：Query，QueryAdjust，QueryRep，ACK，NAK，都是必备的。

    1、标签收到有效Query命令后，符合设定标准被选择的每个标签产生一个随机数(类似掷骰子)，而随机数为零的每个标签，都将产生回响(发回临时口令RN16, 一个16-bit随机数)，并转移到Reply状态;符合另一些条件的标签会改变某些属性和标志，从而退出上述标签群，有利于减少重复识别。

    2、标签收到有效QueryAdjust命令后，各标签分别新产生一个随机数(象重掷骰子)，其他同Query。

    3、标签收到有效QueryRep命令后，只对标签群中的每个标签原有的随机数减一，其他同Query。

    4、仅单一化的标签才能收到有效ACK命令(使用上述RN16，或句柄Handle，一个临时代表标签身份的16-bit随机数，此为一种安全机制)，收到后发回EPC区中的内容，EPC协议最基本的功能。

    5、标签收到有效NAK命令后，除了处于Ready、Killed的保持原状态外, 其它情况都转到Arbitrate状态。

#### 存取(Access)类命令

有五条必备的：Req_RN，Read，Write，Kill，Lock，和三条可选的: Access，BlockWrite，BlockErase。

    1、标签收到有效Req_RN(with RN16 or Handle)命令后，发回句柄，或新的RN16，视状态而不同。

    2、标签收到有效Read(with Handle)命令后，发回出错类型代码，或所要求区块的内容和句柄。

    3、标签收到有效Write(with RN16 & Handle)命令后，发回出错类型代码，或写成功就发回句柄。

    4、标签收到有效Kill(with Kill Password, RN16 & Handle)命令后，发回出错类型代码，或灭活成功就发回句柄。

    5、标签收到有效Lock(with Handle)命令后，发回出错类型代码，或锁定成功就发回句柄。

    6、标签收到有效Access(with Access Password，RN16 & Handle)命令后，发回句柄。

    7、标签收到有效BlockWrite(with Handle)命令后，发回出错类型代码，或块写成功就发回句柄。

    8、标签收到有效BlockErase(with Handle)命令后，发回出错类型代码，或块擦除成功就发回句柄。

## 参考

- [ISO/IEC 18000-63：2021](https://www.iso.org/obp/ui/en/#iso:std:iso-iec:18000:-63:ed-3:v1:en)
- [射频识别](https://zh.wikipedia.org/wiki/%E5%B0%84%E9%A2%91%E8%AF%86%E5%88%AB)
- [史上最全的RFID超高频知识梳理](https://zhuanlan.zhihu.com/p/216010023)
- [关于RFID EPC Class1 Gen2电子标签的命令](https://www.msrfid.com/Service/RFID_EPC_Class1_Gen2.html)
- [科普：你所不知道的那些关于超高频RFID标签常识](https://www.msrfid.com/Service/About_UHF_RFID_labeling.html)
- [ISO18000-6 IOT小分队](http://article.iotxfd.cn/RFID/ISO18000-6C)
