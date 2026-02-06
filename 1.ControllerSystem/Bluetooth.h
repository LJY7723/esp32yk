// #pragma once
// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLEDescriptor.h>

// // 包含Keys.h以获取KVS结构体定义
// #include "Keys.h"

// // 标准BLE HID服务UUID（兼容旧版库）
// #define HID_SERVICE_UUID        "1812"
// #define REPORT_CHAR_UUID        "2A4D"
// #define REPORT_DESC_UUID        "2A4B"
// // CCCD描述符UUID（客户端特征配置描述符）
// #define CCCD_DESCRIPTOR_UUID    "2902"

// // HID报告描述符（基于ESP32-BLE-Gamepad项目优化）
// const uint8_t hidReportDesc[] = {
//   0x05, 0x01,        // Usage Page (Generic Desktop)
//   0x09, 0x05,        // Usage (Game Pad)
//   0xA1, 0x01,        // Collection (Application)
  
//   // 报告ID（Windows兼容性）
//   0x85, 0x01,        // Report ID (1)
  
//   // 左摇杆（X/Y）
//   0x05, 0x01,        // Usage Page (Generic Desktop)
//   0x09, 0x30,        // Usage (X)
//   0x09, 0x31,        // Usage (Y)
//   0x15, 0x81,        // Logical Minimum (-127)
//   0x25, 0x7F,        // Logical Maximum (127)
//   0x75, 0x08,        // Report Size (8)
//   0x95, 0x02,        // Report Count (2)
//   0x81, 0x02,        // Input (Data,Var,Abs)
  
//   // 右摇杆（Z/Rz）
//   0x05, 0x01,        // Usage Page (Generic Desktop)
//   0x09, 0x32,        // Usage (Z)
//   0x09, 0x35,        // Usage (Rz)
//   0x15, 0x81,        // Logical Minimum (-127)
//   0x25, 0x7F,        // Logical Maximum (127)
//   0x75, 0x08,        // Report Size (8)
//   0x95, 0x02,        // Report Count (2)
//   0x81, 0x02,        // Input (Data,Var,Abs)
  
//   // 16个按钮（2字节）
//   0x05, 0x09,        // Usage Page (Button)
//   0x19, 0x01,        // Usage Minimum (1)
//   0x29, 0x10,        // Usage Maximum (16)
//   0x15, 0x00,        // Logical Minimum (0)
//   0x25, 0x01,        // Logical Maximum (1)
//   0x75, 0x01,        // Report Size (1)
//   0x95, 0x10,        // Report Count (16)
//   0x81, 0x02,        // Input (Data,Var,Abs)
  
//   // 旋钮/滑块（2个8位值）
//   0x05, 0x02,        // Usage Page (Sim Controls)
//   0x09, 0xBB,        // Usage (Slider)
//   0x09, 0xBC,        // Usage (Dial)
//   0x15, 0x00,        // Logical Minimum (0)
//   0x26, 0xFF, 0x00,  // Logical Maximum (255)
//   0x75, 0x08,        // Report Size (8)
//   0x95, 0x02,        // Report Count (2)
//   0x81, 0x02,        // Input (Data,Var,Abs)
  
//   0xC0               // End Collection
// };

// class Bluetooth {
// private:
//     BLEServer* pServer;
//     BLECharacteristic* pReportChar;
//     bool isConnectedFlag;
//     unsigned long _lastSendTime = 0;
//     const int _sendInterval = 20; // 20ms发送一次（游戏标准帧率）
//     unsigned long _lastReconnectAttempt = 0;
//     const unsigned long _reconnectInterval = 5000; // 5秒重连尝试间隔（更积极的重连）
//     bool _isAdvertising = false;
//     String _deviceName;           // 存储设备名称
//     String _macAddress;           // 存储MAC地址
//     bool _lastConnectionState;    // 上次连接状态（用于检测状态变化）
//     class Screen* _screen;        // 屏幕对象指针（用于状态显示）
    
//     // 手柄数据结构体（基于ESP32-BLE-Gamepad优化）
//     typedef struct __attribute__((packed)) {
//         uint8_t reportId;  // 报告ID
//         int8_t  lx;        // 左摇杆X轴
//         int8_t  ly;        // 左摇杆Y轴
//         int8_t  rx;        // 右摇杆X轴
//         int8_t  ry;        // 右摇杆Y轴
//         uint16_t buttons;  // 16个按钮（2字节）
//         uint8_t knobL;     // 左旋钮
//         uint8_t knobR;     // 右旋钮
//     } GamepadData;

//     GamepadData reportData;

//     // 连接回调（兼容旧版库）
//     class ServerCallbacks : public BLEServerCallbacks {
//     public:
//         Bluetooth* parent;
//         ServerCallbacks(Bluetooth* parent) : parent(parent) {}
//         void onConnect(BLEServer* pServer) override {
//             parent->isConnectedFlag = true;
//             parent->_isAdvertising = false;
//             parent->updateConnectionStatus(true); // 更新屏幕显示
//             Serial.println("[蓝牙] 设备已连接");
//         }
//         void onDisconnect(BLEServer* pServer) override {
//             parent->isConnectedFlag = false;
//             parent->_isAdvertising = true;
//             parent->_lastReconnectAttempt = millis();
//             parent->updateConnectionStatus(false); // 更新屏幕显示
//             Serial.println("[蓝牙] 连接断开，尝试重新连接...");
//             // 断开后重启广播
//             pServer->startAdvertising();
//         }
//     };

// public:
//     Bluetooth();
//     void begin(const char* deviceName = nullptr, class Screen* screen = nullptr); // 修改为nullptr，在函数内部生成唯一名称
//     void sendReport(uint16_t buttons, int8_t lx, int8_t ly, int8_t rx, int8_t ry, uint8_t knobL, uint8_t knobR);
//     bool isConnected();
//     void checkReconnect(); // 新增：检查并尝试重新连接
    
//     // 新增：屏幕显示相关函数
//     void drawStatusScreen(class Screen& screen);                    // 绘制蓝牙状态屏幕
//     void updateConnectionStatus(bool connected); // 更新连接状态显示
//     String getDeviceName();                      // 获取设备名称
//     String getMacAddress();                      // 获取MAC地址
    
//     // 新增：统一的按键映射函数
//     static uint16_t mapButtons(const KVS& kvs);
//     static uint8_t mapKnob(int16_t knobValue);
// };