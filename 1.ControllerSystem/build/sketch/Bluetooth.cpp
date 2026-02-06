#line 1 "D:\\BaiduSyncdisk\\esp32yk\\yk2\\1.ControllerSystem\\Bluetooth.cpp"
// #include "Bluetooth.h"
// #include "controller_keys.h"
// #include "Keys.h"
// #include "Screen.h"  // 添加屏幕支持
// #include <WiFi.h>     // 添加WiFi支持以获取MAC地址

// Bluetooth::Bluetooth() 
//     : pServer(nullptr)
//     , pReportChar(nullptr)
//     , isConnectedFlag(false)
//     , _screen(nullptr)
// {
//     // 初始化报告数据结构
//     memset(&reportData, 0, sizeof(reportData));
//     reportData.reportId = 0x01; // 设置报告ID
// }

// void Bluetooth::begin(const char* deviceName, Screen* screen) {
//     // 生成唯一设备名称
//     String uniqueName;
//     if (deviceName == nullptr) {
//         // 如果没有传入设备名称，使用默认名称+MAC地址
//         uniqueName = "LilyGo_S3_Gamepad_" + String(ESP.getEfuseMac() & 0xFFFF, HEX);
//     } else {
//         // 如果传入了设备名称，添加MAC地址确保唯一性
//         uniqueName = String(deviceName) + "_" + String(ESP.getEfuseMac() & 0xFFFF, HEX);
//     }
    
//     // 保存设备信息用于屏幕显示
//     _deviceName = uniqueName;
//     _macAddress = WiFi.macAddress();
//     _lastConnectionState = false;
//     _screen = screen;  // 保存屏幕对象指针
    
//     // 初始化BLE设备
//     BLEDevice::init(uniqueName.c_str());
    
//     // 创建BLE服务器
//     pServer = BLEDevice::createServer();
//     pServer->setCallbacks(new ServerCallbacks(this));

//     // 创建HID标准服务
//     BLEService* pService = pServer->createService(BLEUUID(HID_SERVICE_UUID));

//     // 1. 创建报告描述符特征值
//     BLECharacteristic* pReportDescChar = pService->createCharacteristic(
//         BLEUUID(REPORT_DESC_UUID),
//         BLECharacteristic::PROPERTY_READ
//     );
//     pReportDescChar->setValue((uint8_t*)hidReportDesc, sizeof(hidReportDesc));

//     // 2. 创建数据报告特征值
//     pReportChar = pService->createCharacteristic(
//         BLEUUID(REPORT_CHAR_UUID),
//         BLECharacteristic::PROPERTY_READ |
//         BLECharacteristic::PROPERTY_WRITE |
//         BLECharacteristic::PROPERTY_WRITE_NR |
//         BLECharacteristic::PROPERTY_NOTIFY |
//         BLECharacteristic::PROPERTY_INDICATE
//     );

//     // 3. 创建CCCD描述符（客户端特征配置描述符）
//     BLEDescriptor* pCccd = new BLEDescriptor(BLEUUID(CCCD_DESCRIPTOR_UUID));
//     uint8_t cccdValue[2] = {0x01, 0x00}; // 小端格式，0x0001开启通知
//     pCccd->setValue(cccdValue, 2);
//     pReportChar->addDescriptor(pCccd);

//     // 启动服务
//     pService->start();

//     // 配置广播参数（基于ESP32-BLE-Gamepad优化）
//     BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
//     pAdvertising->addServiceUUID(BLEUUID(HID_SERVICE_UUID));
//     pAdvertising->setAppearance(0x03C4); // 游戏手柄外观编码
    
//     // 优化广播参数
//     pAdvertising->setScanResponse(true); // 启用扫描响应
//     pAdvertising->setMinPreferred(0x06); // 最小广播间隔（7.5ms）
//     pAdvertising->setMaxPreferred(0x0C); // 最大广播间隔（15ms）
    
//     // 设置制造商数据（可选，增强兼容性）
//     uint8_t manufacturerData[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
//     pAdvertising->setManufacturerData(std::string((char*)manufacturerData, 5));
    
//     // 启动广播
//     BLEDevice::startAdvertising();
//     _isAdvertising = true;
//     _lastReconnectAttempt = millis();
    
//     Serial.println("[蓝牙] 初始化完成，等待连接...");
//     Serial.print("[蓝牙] 设备名称: ");
//     Serial.println(uniqueName);
//     Serial.print("[蓝牙] MAC地址: ");
//     Serial.println(_macAddress);
// }

// void Bluetooth::sendReport(uint16_t buttons, int8_t lx, int8_t ly, int8_t rx, int8_t ry, uint8_t knobL, uint8_t knobR) {
//     // 控制发送频率（20ms一次，游戏标准帧率）
//     unsigned long now = millis();
//     if (now - _lastSendTime < _sendInterval) {
//         return;
//     }
//     _lastSendTime = now;
    
//     if (pReportChar && isConnectedFlag) {
//         // 更新报告数据
//         reportData.lx = constrain(lx, -127, 127);
//         reportData.ly = constrain(ly, -127, 127);
//         reportData.rx = constrain(rx, -127, 127);
//         reportData.ry = constrain(ry, -127, 127);
//         reportData.buttons = buttons;
//         reportData.knobL = knobL;
//         reportData.knobR = knobR;
        
//         // 发送数据（使用indicate确保数据送达）
//         pReportChar->setValue((uint8_t*)&reportData, sizeof(reportData));
//         pReportChar->indicate();
        
//         // 调试输出（可选）
//         if (now % 1000 < 20) { // 每秒输出一次调试信息
//             Serial.printf("[蓝牙] 发送数据: LX=%d, LY=%d, RX=%d, RY=%d, BTN=0x%04X, KnobL=%d, KnobR=%d\n", 
//                          reportData.lx, reportData.ly, reportData.rx, reportData.ry, 
//                          reportData.buttons, reportData.knobL, reportData.knobR);
//         }
//     }
// }

// bool Bluetooth::isConnected() {
//     return isConnectedFlag;
// }

// // 检查并尝试重新连接
// void Bluetooth::checkReconnect() {
//     if (!isConnectedFlag && _isAdvertising) {
//         unsigned long now = millis();
//         if (now - _lastReconnectAttempt >= _reconnectInterval) {
//             Serial.println("[蓝牙] 尝试重新连接...");
//             BLEDevice::startAdvertising();
//             _lastReconnectAttempt = now;
//         }
//     }
// }

// // 获取设备名称
// String Bluetooth::getDeviceName() {
//     return _deviceName;
// }

// // 获取MAC地址
// String Bluetooth::getMacAddress() {
//     return _macAddress;
// }

// // 绘制蓝牙状态屏幕
// void Bluetooth::drawStatusScreen(Screen& screenObj) {
//     // 完全清屏
//     screenObj.spr.fillSprite(TFT_BLACK);
    
//     // 设置字体和颜色
//     screenObj.spr.setTextColor(TFT_WHITE);
//     screenObj.spr.setTextDatum(TC_DATUM); // 顶部居中
    
//     // 绘制标题
//     screenObj.spr.drawString("蓝牙手柄状态", 120, 20, 4);
    
//     // 绘制分隔线
//     screenObj.spr.drawLine(20, 60, 220, 60, TFT_CYAN);
    
//     // 绘制设备信息
//     screenObj.spr.setTextDatum(TL_DATUM); // 左上对齐
//     screenObj.spr.drawString("设备名称:", 20, 80, 2);
//     screenObj.spr.drawString(_deviceName, 100, 80, 2);
    
//     screenObj.spr.drawString("MAC地址:", 20, 110, 2);
//     screenObj.spr.drawString(_macAddress, 100, 110, 2);
    
//     // 绘制连接状态
//     screenObj.spr.drawString("连接状态:", 20, 140, 2);
//     if (isConnectedFlag) {
//         screenObj.spr.setTextColor(TFT_GREEN);
//         screenObj.spr.drawString("已连接", 100, 140, 2);
//         // 绘制连接成功图标
//         screenObj.spr.fillSmoothCircle(200, 150, 10, TFT_GREEN, TFT_BLACK);
//     } else {
//         screenObj.spr.setTextColor(TFT_RED);
//         screenObj.spr.drawString("未连接", 100, 140, 2);
//         // 绘制断开图标
//         screenObj.spr.fillSmoothCircle(200, 150, 10, TFT_RED, TFT_BLACK);
//     }
    
//     // 绘制广播状态
//     screenObj.spr.setTextColor(TFT_WHITE);
//     screenObj.spr.drawString("广播状态:", 20, 170, 2);
//     if (_isAdvertising) {
//         screenObj.spr.setTextColor(TFT_YELLOW);
//         screenObj.spr.drawString("广播中", 100, 170, 2);
//         // 绘制广播动画效果
//         static uint8_t animCounter = 0;
//         animCounter++;
//         uint8_t radius = (animCounter % 20 < 10) ? 8 : 6;
//         screenObj.spr.fillSmoothCircle(200, 180, radius, TFT_YELLOW, TFT_BLACK);
//     } else {
//         screenObj.spr.setTextColor(0x8410); // 灰色
//         screenObj.spr.drawString("已停止", 100, 170, 2);
//         screenObj.spr.fillSmoothCircle(200, 180, 6, 0x8410, TFT_BLACK);
//     }
    
//     // 绘制操作提示
//     screenObj.spr.setTextColor(TFT_CYAN);
//     screenObj.spr.setTextDatum(BC_DATUM); // 底部居中
//     screenObj.spr.drawString("按X键返回主菜单", 120, 460, 2);
    
//     // 推送到屏幕
//     lcd_PushColors(0, 0, 240, 536, (uint16_t*)screenObj.spr.getPointer());
// }

// // 更新连接状态显示
// void Bluetooth::updateConnectionStatus(bool connected) {
//     // 如果连接状态发生变化，并且有屏幕对象，重新绘制屏幕
//     if (connected != _lastConnectionState && _screen != nullptr) {
//         _lastConnectionState = connected;
//         drawStatusScreen(*_screen);
        
//         // 串口输出状态变化
//         if (connected) {
//             Serial.println("[蓝牙] 连接状态：已连接");
//         } else {
//             Serial.println("[蓝牙] 连接状态：已断开");
//         }
//     }
// }

// // 统一的按键映射函数
// uint16_t Bluetooth::mapButtons(const KVS& kvs) {
//     uint16_t btnMask = 0;
    
//     // 前端4个按键
//     if (kvs.L_up == 0)    btnMask |= 0x0001;  // 第1位
//     if (kvs.L_down == 0)  btnMask |= 0x0002;  // 第2位
//     if (kvs.R_up == 0)    btnMask |= 0x0004;  // 第3位
//     if (kvs.R_down == 0)  btnMask |= 0x0008;  // 第4位
    
//     // 板载2个按键
//     if (kvs.board_L == 0) btnMask |= 0x0010;  // 第5位
//     if (kvs.board_R == 0) btnMask |= 0x0020;  // 第6位
    
//     // 8个功能键
//     if (kvs.up == 0)      btnMask |= 0x0040;  // 第7位
//     if (kvs.down == 0)    btnMask |= 0x0080;  // 第8位
//     if (kvs.left == 0)    btnMask |= 0x0100;  // 第9位
//     if (kvs.right == 0)   btnMask |= 0x0200;  // 第10位
//     if (kvs.o == 0)       btnMask |= 0x0400;  // 第11位
//     if (kvs.x == 0)       btnMask |= 0x0800;  // 第12位
//     if (kvs.a == 0)       btnMask |= 0x1000;  // 第13位
//     if (kvs.b == 0)       btnMask |= 0x2000;  // 第14位
    
//     // 拨杆开关（可选映射）
//     if (kvs.switch_L1 == 0) btnMask |= 0x4000; // 第15位
//     if (kvs.switch_L2 == 0) btnMask |= 0x8000; // 第16位
    
//     return btnMask;
// }

// // 旋钮映射函数
// uint8_t Bluetooth::mapKnob(int16_t knobValue) {
//     // 将-127~127的摇杆值映射到0~255的旋钮值
//     return (knobValue + 127) / 2;
// }