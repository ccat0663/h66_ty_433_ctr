#### 简介

- 使用17h66 ble芯片和涂鸦wbr3模块配合完成的433远程控制器，支持ble+wifi或者纯ble使用，具体工程可配置
- 需要另外购买433遥控器并且对码完成
- 由于是通过导致433遥控器按键的形式进行控制，所以支持滚码等所有按键控制的遥控器，最多支持4个按键

#### 开源连接

- [硬件开源连接](https://oshwhub.com/ccat0663/ty_433_wifi_ble)

#### 代码工程配置

- 在[ty_dev_config.h](https://github.com/ccat0663/h66_ty_433_ctr/blob/master/software/ST17H66_SDK_3.1.1.2-ty/example/Tuya_IOT/simpleBlePeripheral/source/ty_dev_config.h)文件中配置涂鸦平台生成的参数，选择使能的功能

#### 涂鸦平台配置

- 数据点配置

​	![数据点配置](https://github.com/ccat0663/h66_ty_433_ctr/blob/master/file/%E6%B6%82%E9%B8%A6%E6%95%B0%E6%8D%AE%E7%82%B9%E9%85%8D%E7%BD%AE.png)

- studio面板配置

![studio面板配置](https://github.com/ccat0663/h66_ty_433_ctr/blob/master/file/%E6%B6%82%E9%B8%A6studio%E9%9D%A2%E6%9D%BF.png)

#### 硬件

![PCB](https://github.com/ccat0663/h66_ty_433_ctr/blob/master/file/3d.png)

![433小板](https://github.com/ccat0663/h66_ty_433_ctr/blob/master/file/433%E5%B0%8F%E6%9D%BF.jpg)

#### 功耗

- ble+wifi模式：平均电流60ma左右
- 纯ble模式：平均电流60ua左右(暂未实际优化)