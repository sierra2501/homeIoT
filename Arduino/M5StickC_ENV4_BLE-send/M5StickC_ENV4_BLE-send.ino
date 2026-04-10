//#include <M5StickC.h>
#include <M5StickCPlus.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include <SensirionI2cSht4x.h>
#include <Adafruit_BMP280.h>


#define SERVICE_UUID "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
#define CHARACTERISTIC_UUID "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

SensirionI2cSht4x sensor;

BLEServer *pServer;
Adafruit_BMP280 bmp;

int t_start = 60;
int t_end = 0.5;

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
// ===================================================
// ===================================================
void deliveryMeasument(float tmp, float hum, float pes){
  
  std::string str_tmp = std::to_string(tmp);  // 気温
  std::string str_hum = std::to_string(hum);  // 湿度
  std::string str_pes = std::to_string(pes);  // 気圧

  // 気圧が10や99になることがある
  if (pes < 99 ){
    delay(t_end * 1000);
    return;
  }
  if (str_pes == "10" || str_pes == "99"){
    delay(t_end * 1000);
    return;
  }

  // 連結
  std::string str = "";
  str += str_tmp;
  str += ",";
  str += str_hum;
  str += ",";
  str += str_pes;

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

  return;
}
// ===================================================
// ===================================================
void displayMeasument(float tmp, float hum, float pes){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.println("Temp: ");
  M5.Lcd.print(tmp);
  M5.Lcd.println(" dC");

  M5.Lcd.setCursor(0, 55);
  M5.Lcd.println("Hum: ");
  M5.Lcd.print(hum);
  M5.Lcd.println("% rH");

  M5.Lcd.setCursor(0, 85);
  M5.Lcd.println(F("Pes: "));
  M5.Lcd.print(pes);
  M5.Lcd.println(" hPa");
}
// ===================================================
// ===================================================
void setup()
{
  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.setTextSize(1);
  M5.Lcd.fillScreen(BLACK);
  //M5.Lcd.println("M5CBLE");
  M5.Lcd.println("M5CPlusBLE");

  // ====================
  // BLEの初期化
  // ====================
  //BLEDevice::init("M5CBLE");
  BLEDevice::init("M5CPlusBLE");
  pServer = BLEDevice::createServer();

  // ====================
  // センサの初期化
  // ====================
  Wire.begin();
  sensor.begin(Wire, SHT40_I2C_ADDR_44);

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    M5.Lcd.println("Couldn't find BMP280");
    while (1) delay(1);
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

}

// ===================================================
// ===================================================
void loop()
{ 

  M5.update();

  float tmp = 0.0;
  float hum = 0.0;
  float pes = 0.0;

  sensor.measureLowestPrecision(tmp, hum);
  pes = (float)(bmp.readPressure())/100;

  deliveryMeasument(tmp, hum, pes);
  displayMeasument(tmp, hum, pes);
}
