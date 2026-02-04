#include "Bluetooth.h"
#include "controller_keys.h"

Bluetooth::Bluetooth() 
    : pServer(nullptr), pReportChar(nullptr), isConnectedFlag(false) {
    memset(&reportData, 0, sizeof(GamepadData));
}

void Bluetooth::begin(const char* deviceName) {
    // 初始化BLE设备
    BLEDevice::init(deviceName);
    
    // 创建BLE服务器
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));

    // 创建HID服务
    BLEService* pService = pServer->createService(BLEUUID(HID_SERVICE_UUID));

    // 创建报告特征值（旧版API）
    pReportChar = pService->createCharacteristic(
        BLEUUID(REPORT_CHAR_UUID),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_WRITE
    );

    // 添加描述符（HID标准配置）
    BLE2904* p2904 = new BLE2904();
    p2904->setFormat(BLE2904::FORMAT_UINT8);
    pReportChar->addDescriptor(p2904);

    // 启动服务
    pService->start();

    // 配置广播
    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(BLEUUID(HID_SERVICE_UUID));
    // 设置为游戏手柄设备类型
    pAdvertising->setAppearance(0x03C4);
    pAdvertising->setScanResponse(true);
    
    // 启动广播
    BLEDevice::startAdvertising();
    Serial.println("[蓝牙] 初始化完成，等待连接...");
}

void Bluetooth::sendReport(uint16_t buttons, int8_t lx, int8_t ly, int8_t rx, int8_t ry, uint8_t knobL, uint8_t knobR) {
    if (!isConnectedFlag || !pReportChar) return;

    // 填充数据
    reportData.lx = constrain(lx, -128, 127);
    reportData.ly = constrain(ly, -128, 127);
    reportData.rx = constrain(rx, -128, 127);
    reportData.ry = constrain(ry, -128, 127);
    reportData.btn[0] = buttons & 0xFF;
    reportData.btn[1] = (buttons >> 8) & 0xFF;
    reportData.knobL = knobL;
    reportData.knobR = knobR;

    // 发送数据（旧版API）
    pReportChar->setValue((uint8_t*)&reportData, sizeof(GamepadData));
    pReportChar->notify();
}

bool Bluetooth::isConnected() {
    return isConnectedFlag;
}