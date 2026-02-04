#pragma once
// 第一步：优先引入引脚定义头文件，让PIN_BUZZER宏生效
#include "controller_keys.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2904.h>

// HID 服务UUID + 游戏手柄配置
#define HID_SERVICE_UUID        "1812"
#define REPORT_CHAR_UUID        "2A4D"
// 旧版API固定HID报告描述符（手柄标准格式）
const uint8_t hidReportDesc[] = {
  0x05, 0x01,        // Usage Page (Generic Desktop)
  0x09, 0x05,        // Usage (Game Pad)
  0xA1, 0x01,        // Collection (Application)
  0x05, 0x01,        //   Usage Page (Generic Desktop)
  0x09, 0x30,        //   Usage (X)
  0x09, 0x31,        //   Usage (Y)
  0x09, 0x32,        //   Usage (Rx)
  0x09, 0x35,        //   Usage (Ry)
  0x15, 0x80,        //   Logical Minimum (-128)
  0x25, 0x7F,        //   Logical Maximum (127)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x04,        //   Report Count (4)
  0x81, 0x02,        //   Input (Data,Var,Abs)
  0x05, 0x09,        //   Usage Page (Buttons)
  0x19, 0x01,        //   Usage Minimum (1)
  0x29, 0x10,        //   Usage Maximum (16)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x10,        //   Report Count (16)
  0x81, 0x02,        //   Input (Data,Var,Abs)
  0x05, 0x02,        //   Usage Page (Sim Controls)
  0x09, 0xBB,        //   Usage (Slider)
  0x09, 0xBC,        //   Usage (Dial)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0xFF,        //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x02,        //   Report Count (2)
  0x81, 0x02,        //   Input (Data,Var,Abs)
  0xC0               // End Collection
};

class Bluetooth {
private:
    BLEServer* pServer;
    BLECharacteristic* pReportChar;
    bool isConnectedFlag;

    // 手柄数据结构体（内存对齐，防止错位）
    typedef struct __attribute__((packed)) {
        int8_t  lx;        // 左摇杆X
        int8_t  ly;        // 左摇杆Y
        int8_t  rx;        // 右摇杆X
        int8_t  ry;        // 右摇杆Y
        uint8_t btn[2];    // 16个按键
        uint8_t knobL;     // 左旋钮
        uint8_t knobR;     // 右旋钮
    } GamepadData;

    GamepadData reportData;

    // 旧版BLE连接回调
    class ServerCallbacks : public BLEServerCallbacks {
    public:
        Bluetooth* parent;
        ServerCallbacks(Bluetooth* parent) : parent(parent) {}
        void onConnect(BLEServer* pServer) override {
            parent->isConnectedFlag = true;
            Serial.println("[蓝牙] 设备已连接");
            // 连接提示音
            tone(PIN_BUZZER, 1600, 150);
            delay(150);
            noTone(PIN_BUZZER);
        }
        void onDisconnect(BLEServer* pServer) override {
            parent->isConnectedFlag = false;
            Serial.println("[蓝牙] 连接断开");
            tone(PIN_BUZZER, 800, 200);
            delay(200);
            noTone(PIN_BUZZER);
            // 断开后重启广播
            pServer->startAdvertising();
        }
    };

public:
    Bluetooth();
    // 初始化蓝牙
    void begin(const char* deviceName = "LilyGo_S3_Controller");
    // 发送手柄数据
    void sendReport(uint16_t buttons, int8_t lx, int8_t ly, int8_t rx, int8_t ry, uint8_t knobL, uint8_t knobR);
    // 获取连接状态
    bool isConnected();
    // 重连管理（旧版无需手动处理，回调自动重启广播）
    void handleReconnect() {}
};