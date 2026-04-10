#include <M5Stack.h>
#include <BLEDevice.h>
#include <BLE2902.h>

#define PIN 36          // ピン番号


#define SERVICE_UUID "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
#define CHARACTERISTIC_UUID "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"

BLEServer *pServer;

int t_start = 60;
int t_end = 0.5;
int ANA = 0;       // アナログ値 

// ===================================================
// サービスを開始する
// ===================================================
void startService(BLEService *pService, std::string str)
{
  BLECharacteristic *pCharacteristic  = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setValue(str);
  pService->start();
}

// ===================================================
// アドバタイズを開始する
// ===================================================
void startAdvertising()
{
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
}



// ANAを表示する
void printCntAndFlg(){
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("ANA: %d", ANA);
}

// ===================================================
// ===================================================
void setup()
{

  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println("M5BLE");

  // ====================
  // BLEの初期化
  // ====================
  BLEDevice::init("M5BLE");
  pServer = BLEDevice::createServer();

  // ====================
  // センサの初期化
  // ====================
  pinMode(PIN, INPUT);

}

// ===================================================
// ===================================================
void loop()
{

  M5.update();
  
  ANA = analogRead(PIN);

  std::string str = "";
  str += std::to_string(ANA);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("ANA: %d", ANA);

  // ====================
  // 開始
  // ====================
  BLEService *pService = pServer->createService(SERVICE_UUID);  // サービスの作成
  startService(pService, str);  // サービスの開始
  startAdvertising();           // アドバタイズの開始 
  delay(t_start * 1000);

  // ====================
  // 停止
  // ====================
  BLEDevice::stopAdvertising();       // アドバタイズの停止
  pService->stop();                   // サービスの停止
  pServer->removeService(pService);   // サービスの削除
  delay(t_end * 1000);

}
