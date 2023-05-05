#ifndef _TY_DEV_CONFIG_H 
#define _TY_DEV_CONFIG_H 
#ifdef __cplusplus
extern "C" {
#endif

//编译选项，1:使用BLE sdk，纯蓝牙模式 oth:使用mcu sdk
#define TY_BLE_SDK 0

#error "配置完成后删除"

//涂鸦纯BLE三元组和PID配置
#define TY_DEVICE_PID_CONFIG         "xxxx" 
#define TY_DEVICE_MAC_CONFIG          "xxxx"
#define TY_DEVICE_DID_CONFIG          "xxxx" //16Bytes
#define TY_DEVICE_AUTH_KEY_CONFIG     "xxxx" //32Bytes

//wbr3 mcu sdk PID配置
#define TY_PRODUCT_KEY_CONFIG "xxxx"    //开发平台创建产品后生成的16位字符产品唯一标识

#ifdef __cplusplus
}
#endif
#endif	// _TY_DEV_CONFIG_H

