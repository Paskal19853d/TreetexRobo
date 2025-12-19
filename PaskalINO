///////////////////////////////БИБЛИОТЕКИ//////////////////////////////////////////////////////
#include <Wire.h>                 // Библиотека I2C (обмен данными с OLED, датчиками и др.)
#include <Adafruit_GFX.h>         // Графический движок: линии, текст, шрифты, примитивы
#include <VL53L0X.h>              // Датчик лазерный ToF-дальномер
#include <Ultrasonic.h>           // Ультразвуковой дальномер   
#include <Adafruit_Sensor.h>      // Общая библиотека сенсоров
#include <Adafruit_GC9A01A.h>     // Драйвер дисплея GC9A01A
#include <Adafruit_AHTX0.h>       // для AHT20/10
#include <Adafruit_BMP280.h>      // для BMP280
#include <BH1750.h>               // для датчика освещенности
#include <DFRobotDFPlayerMini.h>  // Player
#include <HardwareSerial.h>       // For Player
#include "I2Cdev.h"               // I2C
#include "MPU6050.h"              // Работа с гироскопом/акселерометром MPU6050 (наклон, движение)
#include <FluxGarage_RoboEyes.h>
// Модули
//////////////////////////////////////////////////////////////////////////////////////////////
#include "preua.h"                // Bitmap_s изображений 

//////////////////////// КОНФИГУРАЦИЯ ПИНОВ //////////////////////////////////
#define I2C_SDA 21      //SDA
#define I2C_SCL 22      //SCL
#define TFT_CS   5      //CS
#define TFT_DC   2      //DC
#define TFT_RST  4      //RST
#define US_TRIG 26      //TRIG
#define US_ECHO 27      //ECHO
#define TTP1    32      //Tuch
#define TTP2    33      //Tuch
#define TTP3    25      //Tuch
#define BUZZER_PIN 13   //BUZZER
#define CHRG_PIN   12   //Цифровой вход  TP4056 CHRG LED  1,8- 2V
#define BAT_ADC    34   // ADC1   Battery voltage ADC  Через делитель!!!!


//////////////////////// ОБЪЕКТЫ /////////////////////////////////////////////
Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);  
RoboEyes<Adafruit_GC9A01A> eyes(tft);
MPU6050 mpu(0x68);                                
VL53L0X tof;                                        
Adafruit_AHTX0 aht;                                 
Adafruit_BMP280 bmp;                                
BH1750 lightMeter;                                  
Ultrasonic ultrasonic(US_TRIG, US_ECHO);            

HardwareSerial dfSerial(2);   // UART2
DFRobotDFPlayerMini dfPlayer;

//////////////////////// ТАЙМЕРЫ /////////////////////////////////////////////
unsigned long tMPU    = 0;   // таймер для MPU6050
unsigned long tTable  = 0;   // таймер для чтения датчиков и Serial
unsigned long tDisplay = 0;  // таймер для обновления дисплея (отдельно для асинхронности)
unsigned long dataDisplayStart = 0;  // Время начала отображения данных (для 2 мин)
bool showData = true;  // Флаг: показывать данные на дисплее
const unsigned long DATA_DISPLAY_DURATION = 120000;  // 2 минуты

//////////////////////// ТАЧ КНОПКИ //////////////////////////////////////////
bool lastTTP1 = false;
bool lastTTP2 = false;
bool lastTTP3 = false;

//////////////////////// ЛОГО ////////////////////////////////////////////////
unsigned long logoStartTime = 0;
bool logoShown = false;
const unsigned long LOGO_DURATION = 5000; // 5 секунд показ логотипа
//////////////////////// ГЛАЗА ///////////////////////////////////////////
unsigned long animationTimer = 0;
unsigned long currentAnimation = 0;
unsigned long tEyes = 0;  //****
//////////////////////// ULTRASONIC ///////////////////////////////////////////
long distanceCM = -1;                         // последнее измерение
static unsigned long lastRead = 0;
static const unsigned long interval = 100;    // 100 мс между измерениями


void ultrasonicInit(uint8_t trigPin, uint8_t echoPin) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void ultrasonicUpdate() {
  unsigned long now = millis();
  if (now - lastRead >= interval) {
    lastRead = now;
    distanceCM = ultrasonic.read();
  }
}

long ultrasonicGetDistance() { return distanceCM; }

///////////////////////// ДАННЫЕ ДАТЧИКОВ /////////////////////////////////////
struct SensorData {
  // MPU6050
  float angleX = 0;
  float angleY = 0;
  float angleZ = 0;
  
  // VL53L0X
  int laserDistMM = 0;
  
  // AHT20
  float ahtTemp = 0;
  float ahtHum = 0;
  
  // BMP280
  float bmpTemp = 0;
  float bmpPressure = 0;
  
  // BH1750
  float lux = 0;
  
  // Ultrasonic
  long ultrasonicCM = 0;
} sensorData;

///////////////////////// ПРЕДЗАГРУЗКА ДАТЧИКОВ ///////////////////////////////
struct SensorStatus {
  const char* name;
  bool connected;
};

SensorStatus sensors[] = {
  {"MPU6050", false},
  {"VL53L0X", false},
  {"AHT20", false},
  {"BMP280", false},
  {"BH1750", false},
  {"SD_MMC", false},
  {"Ultrasonic", true}   // ультразвук инициализируем всегда
};

// ---------- КЭШ ДИСПЛЕЯ ----------
struct DisplayCache {
  float angleX = -9999;  // Добавил для MPU (чтобы очищать только при изменении)
  float angleY = -9999;
  float angleZ = -9999;
  int laser = -9999;
  float ahtT = -9999;
  float ahtH = -9999;
  float bmpT = -9999;
  float bmpP = -9999;
  float lux  = -9999;
  long ultra = -9999;
} disp;



/////////////////////////////////////////////////////////////////////////////
/////////////////////// ТАЧ КНОПКИ //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void checkTTP() {
  bool t1 = digitalRead(TTP1);
  bool t2 = digitalRead(TTP2);
  bool t3 = digitalRead(TTP3);

  if (t1 && !lastTTP1) Serial.println("TTP1: TOUCH");
  if (!t1 && lastTTP1) Serial.println("TTP1: RELEASE");
//--------------------------------------------------------
  if (t2 && !lastTTP2) Serial.println("TTP2: TOUCH");
  if (!t2 && lastTTP2) Serial.println("TTP2: RELEASE");
//--------------------------------------------------------
  if (t3 && !lastTTP3) Serial.println("TTP3: TOUCH");
  if (!t3 && lastTTP3) Serial.println("TTP3: RELEASE");

  lastTTP1 = t1;
  lastTTP2 = t2;
  lastTTP3 = t3;

   
}

/////////////////////////////////////////////////////////////////////////////
////////////// ЧТЕНИЕ ВСЕХ ДАТЧИКОВ /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void readAllSensors() {
  // --- MPU6050 ---
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  if (sensors[0].connected) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Вычисляем углы наклона (пример простого акселерометрного метода)
    sensorData.angleX = atan2(ay, az) * 180.0 / PI;
    sensorData.angleY = atan2(ax, az) * 180.0 / PI;
    sensorData.angleZ = atan2(ax, ay) * 180.0 / PI;
  }
  
  // --- VL53L0X (лазерный дальномер) ---
  if (sensors[1].connected) {
    int dist = tof.readRangeContinuousMillimeters();
    if (!tof.timeoutOccurred()) {
      sensorData.laserDistMM = dist;   // обновляем ТОЛЬКО если нет таймаута
    }
    // если таймаут — просто ничего не делаем
  }
  
  // --- AHT20 (температура + влажность) ---
  if (sensors[2].connected) {
    sensors_event_t hum, temp;
    aht.getEvent(&hum, &temp);
    sensorData.ahtTemp = temp.temperature;
    sensorData.ahtHum = hum.relative_humidity;
  }
  
  // --- BMP280 (давление + температура) ---
  if (sensors[3].connected) {
    sensorData.bmpTemp = bmp.readTemperature();
    sensorData.bmpPressure = bmp.readPressure() / 100.0; // в гПа
  }
  
  // --- BH1750 (освещенность) ---
  if (sensors[4].connected) {
    sensorData.lux = lightMeter.readLightLevel();
  }
  
  // --- Ультразвук ---
  if (sensors[6].connected) {
    sensorData.ultrasonicCM = ultrasonicGetDistance();
  }
}

/////////////////////////////////////////////////////////////////////////////
////////////// ВЫВОД ТАБЛИЦЫ В SERIAL ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void printSerialTable() {
  Serial.println("\n========== SENSOR DATA ==========");

  // MPU6050
  if (sensors[0].connected) {
    Serial.printf("MPU6050  | X:%.1f Y:%.1f Z:%.1f\n", 
                  sensorData.angleX, sensorData.angleY, sensorData.angleZ);
  } else {
    Serial.println("MPU6050  | NO");
  }
  
  // VL53L0X
  if (sensors[1].connected) {
    if (sensorData.laserDistMM >= 0) {
      Serial.printf("VL53L0X  | %d mm\n", sensorData.laserDistMM);
    } else {
      Serial.println("VL53L0X  | TIMEOUT");
    }
  } else {
    Serial.println("VL53L0X  | NO");
  }
  
  // AHT20
  if (sensors[2].connected) {
    Serial.printf("AHT20    | T:%.1f°C H:%.1f%%\n", 
                  sensorData.ahtTemp, sensorData.ahtHum);
  } else {
    Serial.println("AHT20    | NO");
  }
  
  // BMP280
  if (sensors[3].connected) {
    Serial.printf("BMP280   | T:%.1f°C P:%.1f hPa\n", 
                  sensorData.bmpTemp, sensorData.bmpPressure);
  } else {
    Serial.println("BMP280   | NO");
  }
  
  // BH1750
  if (sensors[4].connected) {
    Serial.printf("BH1750   | %.1f lux\n", sensorData.lux);
  } else {
    Serial.println("BH1750   | NO");
  }
  
  // Ultrasonic
  if (sensors[6].connected) {
    Serial.printf("US-HC04  | %ld cm\n", sensorData.ultrasonicCM);
  } else {
    Serial.println("US-HC04  | NO");
  }
  
  Serial.println("=================================\n");
}

/////////////////////////////////////////////////////////////////////////////
////////////// ВЫВОД НА ДИСПЛЕЙ (в 1 столбца) //////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void displaySensorData() { // Функция показа параметров на дисплее
  tft.setTextSize(1);
  tft.setRotation(0);
  // Не очищаем весь экран, чтобы избежать мерцания — очищаем только области текста
  // Один столбик с отступом от края (8px от предполагаемого края круга, начинаем с x=20, y=20)
  int leftX = 30;  // Отступ слева (8px + запас для круга)
  int indentX = leftX + 10;  // Отступ для значений (подзаголовки)
  int y = 45;  // Отступ сверху
  int lh = 10;  // Высота строки (компактно, чтобы всё уместилось в ~200px высоты круга)
  int lineWidth = 200;  // Ширина очистки (почти весь дисплей, но внутри круга)
  // ---------- MPU6050 ----------
  // Очистка заголовка
  tft.fillRect(leftX, y, lineWidth, lh, 0x0000);
  tft.setCursor(leftX, y);
  tft.setTextColor(0x07FF);  // Синий заголовок
  tft.print("MPU6050");
  y += lh;

  // Очистка и вывод значений XYZ в одной строке (с отступом)
  tft.fillRect(indentX, y, lineWidth - indentX + leftX, lh, 0x0000);
  tft.setCursor(indentX, y);
  tft.setTextColor(0xFFFF);  // Белый текст
  tft.printf("X:%.1f Y:%.1f Z:%.1f", sensorData.angleX, sensorData.angleY, sensorData.angleZ);
  y += lh + 5;  // Малый пробел (убрали две строки, так что y короче)

  // ---------- AHT20 ----------
  tft.fillRect(leftX, y, lineWidth, lh, 0x0000);
  tft.setCursor(leftX, y);
  tft.setTextColor(0x07FF);
  tft.print("AHT20");
  y += lh;

  tft.fillRect(indentX, y, lineWidth - indentX + leftX, lh, 0x0000);
  tft.setCursor(indentX, y);
  tft.setTextColor(0xFFFF);
  tft.printf("T:%.1fC H:%.0f%%", sensorData.ahtTemp, sensorData.ahtHum);
  y += lh + 5;

  // ---------- BH1750 ----------
  tft.fillRect(leftX, y, lineWidth, lh, 0x0000);
  tft.setCursor(leftX, y);
  tft.setTextColor(0x07FF);
  tft.print("BH1750");
  y += lh;

  tft.fillRect(indentX, y, lineWidth - indentX + leftX, lh, 0x0000);
  tft.setCursor(indentX, y);
  tft.setTextColor(0xFFFF);
  tft.printf("%.0flux", sensorData.lux);
  y += lh + 5;

  // ---------- VL53L0X ----------
  tft.fillRect(leftX, y, lineWidth, lh, 0x0000);
  tft.setCursor(leftX, y);
  tft.setTextColor(0x07FF);
  tft.print("VL53L0X");
  y += lh;

  tft.fillRect(indentX, y, lineWidth - indentX + leftX, lh, 0x0000);
  tft.setCursor(indentX, y);
  tft.setTextColor(0xFFFF);
  if (sensorData.laserDistMM >= 0) {
    tft.printf("%dmm", sensorData.laserDistMM);
  } else {
    tft.setTextColor(0xFFE0);  // Жёлтый для таймаута
    tft.print("TIMEOUT");
  }
  y += lh + 5;

  // ---------- BMP280 ----------
  tft.fillRect(leftX, y, lineWidth, lh, 0x0000);
  tft.setCursor(leftX, y);
  tft.setTextColor(0x07FF);
  tft.print("BMP280");
  y += lh;

  tft.fillRect(indentX, y, lineWidth - indentX + leftX, lh, 0x0000);
  tft.setCursor(indentX, y);
  tft.setTextColor(0xFFFF);
  tft.printf("T:%.1fC P:%.0fhPa", sensorData.bmpTemp, sensorData.bmpPressure);
  y += lh + 5;

  // ---------- Ultrasonic ----------
  tft.fillRect(leftX, y, lineWidth, lh, 0x0000);
  tft.setCursor(leftX, y);
  tft.setTextColor(0x07FF);
  tft.print("US-HC04");
  y += lh;

  tft.fillRect(indentX, y, lineWidth - indentX + leftX, lh, 0x0000);
  tft.setCursor(indentX, y);
  tft.setTextColor(0xFFFF);
  tft.printf("%ldcm", sensorData.ultrasonicCM);

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////// ГЛАЗА Перебор////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Asiks() {

        unsigned long now = millis();

        if (now - animationTimer < 10000) return;
        animationTimer = now;

        currentAnimation = (currentAnimation + 1) % 5;    //currentAnimation = 1  только 1 анимация

        // СБРОС эффектов при каждом переключении
        eyes.setVFlicker(false, 0);
        eyes.setHFlicker(false, 0);
        eyes.setSweat(false);
        eyes.setCuriosity(false);
        eyes.setCyclops(false);

        switch (currentAnimation) {

          case 0:
            eyes.setMood(DEFAULT);
            Serial.println("Анимация 0: DEFAULT");
            break;

          case 1:
            eyes.setMood(TIRED);
            Serial.println("Анимация 1: TIRED");
            break;

          case 2:
            eyes.setMood(ANGRY);
            eyes.setVFlicker(true, 5);
            eyes.setHFlicker(true, 3);
            Serial.println("Анимация 2: ANGRY + FLICKER");
            break;

          case 3:
            eyes.setMood(HAPPY);
            eyes.setSweat(true);
            eyes.anim_laugh();   // ← один раз!
            Serial.println("Анимация 3: HAPPY + LAUGH");
            break;

          case 4:
            eyes.setMood(SURPRISED);
            eyes.setCuriosity(true);
            Serial.println("Анимация 4: SURPRISED");
            break;
        }  
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////// ФУНКЦИИ  ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool showData1 = false;  // глобальный флаг
void dispData() {
    if (digitalRead(TTP1) == HIGH) {
        showData1 = true;      // кнопка нажата → показываем данные
    } else {
        showData1 = false;     // кнопка отпущена → скрываем данные
    }

    // теперь функция displaySensorData реагирует на флаг
    if (showData1) {
        displaySensorData();
    } else {
        tft.fillScreen(0x0000);  // очищаем экран, если данные не показываем
    }
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////// ФУНКЦИИ ДАТЧИКОВ - ВЫЗОВОВ //////////////////////////
/////////////////////////////////////////////////////////////////////////////
          // 1️⃣ Работа с логотипом
          void handleLogo() {
              if (!logoShown) return;

              unsigned long now = millis();
              if (now - logoStartTime >= LOGO_DURATION) {
                  tft.fillScreen(0x0000);   // убрать лого
                  logoShown = false;
                  dataDisplayStart = now;    // Начать отсчёт 2 минут для данных
                  showData = true;           // Включить отображение данных
                  Serial.println("Логотип скрыт, начало работы датчиков...\n");
              }
          }

          // 2️⃣ Обновление MPU6050
          void updateMPU() {
              unsigned long now = millis();
              if (sensors[0].connected && now - tMPU > 50) {
                  tMPU = now;
                  // Чтение и расчёт углов перемещено в readAllSensors()
              }
          }

          // 3️⃣ Обновление ультразвукового датчика
          void updateUltrasonic() {
              if (sensors[6].connected) ultrasonicUpdate();
          }

          // 4️⃣ Чтение всех датчиков и вывод в Serial
          void updateSensorsAndSerial() {
              unsigned long now = millis();
              if (now - tTable >= 1000) {
                  tTable = now;
                  readAllSensors();
                  printSerialTable();
              }
          }

          // 5️⃣ Проверка таймера для отключения дисплея
          void checkDisplayTimeout() {
              static bool Isdp = false;
              unsigned long now = millis();

              if (showData && now - dataDisplayStart >= DATA_DISPLAY_DURATION) {
                  showData = false;
                  if (!Isdp) {
                      tft.fillScreen(0x0000); // Чёрный экран
                      Isdp = true;
                  }
                  Serial.println("Данные на дисплее скрыты после 2 минут.\n");
              }
          }

          // 6️⃣ Асинхронная анимация глаз
          void updateEyesAnimation() {
              Asiks();        // логика смены анимаций
              eyes.update();  // рендер каждого кадра
          }

          // 7️⃣ Проверка тач-кнопок
          void updateTouchButtons() {
              checkTTP();
          }

/////////////////////////////////////////////////////////////////////////////
/////////////////////// SETUP ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void setup() {        
  Serial.begin(115200);
  SPI.begin(18, -1, 23, TFT_CS);  // SCK, MISO, MOSI, CS
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(10);  // Задержка для стабилизации I2C
  
  // Инициализация дисплея
  tft.begin();
  tft.setRotation(0);   // стандартная ориентация (0, 1, 2 или 3 - попробуйте разные)
  eyes.begin(240, 240, 60); // 240x240, 60 FPS
  tft.fillScreen(0x0000);
  
  // Показываем логотип (центрируем для круглого дисплея)
  int x = (240 - PREUA_WIDTH) / 2;
  int y = (240 - PREUA_HEIGHT) / 2 - 30; // Поднимаем выше
  tft.drawBitmap(x, y, preua, PREUA_WIDTH, PREUA_HEIGHT, 0xFFE0);
  
  tft.setTextColor(0x001F); // желтый
  tft.setTextSize(2);  // увеличили размер
  tft.setCursor(40, 170); // позиция под логотипом
  tft.println("TREETEX.shop");
  
  logoStartTime = millis();
  logoShown = true;
  
  // Инициализация датчиков
  Serial.println("\n=== Предзагрузка датчиков ===");
  
  // ---- MPU6050 ----
  mpu.initialize();
  if (mpu.testConnection()) {
    sensors[0].connected = true;
    Serial.println("MPU6050 подключен!");
  } else {
    sensors[0].connected = false;
    Serial.println("Ошибка подключения MPU6050");
  }
  Serial.printf("Инициализация %s - %s\n", sensors[0].name,
                sensors[0].connected ? "Подключено" : "Не подключено");
  
  // ---- VL53L0X ----
  sensors[1].connected = tof.init();
  if (sensors[1].connected) {
    tof.setTimeout(500);
    tof.startContinuous(); // запускаем непрерывное измерение
  }
  Serial.printf("Инициализация %s - %s\n", sensors[1].name,
                sensors[1].connected ? "Подключено" : "Не подключено");
  
  // ---- AHT20 ----
  sensors[2].connected = aht.begin();
  Serial.printf("Инициализация %s - %s\n", sensors[2].name,
                sensors[2].connected ? "Подключено" : "Не подключено");
  
  // ---- BMP280 ----
  sensors[3].connected = bmp.begin(0x77);  //0x76 или 0x77
  Serial.printf("Инициализация %s - %s\n", sensors[3].name,
                sensors[3].connected ? "Подключено" : "Не подключено");
  
  // ---- BH1750 ----
  sensors[4].connected = lightMeter.begin();
  Serial.printf("Инициализация %s - %s\n", sensors[4].name,
                sensors[4].connected ? "Подключено" : "Не подключено");
  
  // ---- SD_MMC ----
  sensors[5].connected = ("/sdcard", true, false, 10); // Занлушка -специально sd временно не использую
  Serial.printf("%s - %s\n", sensors[5].name,
                sensors[5].connected ? "Подключено" : "Не подключено");
  
  // ---- Ультразвуковой датчик ----
  ultrasonicInit(US_TRIG, US_ECHO);
  Serial.printf("%s - Инициализирован\n", sensors[6].name);
  
  Serial.println("=== Все датчики проверены ===\n");

  // ---- ТАЧ КНОПКИ ----
  pinMode(TTP1, INPUT);
  pinMode(TTP2, INPUT);
  pinMode(TTP3, INPUT);
  Serial.println("TTP223 Тач инициализированы");
  
  // -----Воспроизвести инициализацию
  dfSerial.begin(9600, SERIAL_8N1, 16, 17); // RX2=16, TX2=17
  if (!dfPlayer.begin(dfSerial)) {
    Serial.println("DFPlayer not found");
    return;
  }
  //-------------------------------
  dfPlayer.volume(20); // 0..30
  dfPlayer.play(2);    // ▶ 001.mp3
  //-------------------------------
  tone(BUZZER_PIN, 1000);  // первый пик, 1000 Гц
  delay(100);               // длительность пика 200 мс
  noTone(BUZZER_PIN); 
  //-------------------------------
  
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////// LOOP ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
  void loop() {
      handleLogo();              // 1️⃣ Логотип
      if (logoShown) return;     // пока логотип — больше ничего не делаем
      updateMPU();               // 2️⃣ MPU
      updateUltrasonic();        // 3️⃣ Ультразвук
      updateSensorsAndSerial();  // 4️⃣ Датчики + Serial
      checkDisplayTimeout();     // 5️⃣ Таймер дисплея
      updateEyesAnimation();     // 6️⃣ Анимация глаз
      updateTouchButtons();      // 7️⃣ Тач-кнопки
      dispData();                // 8️⃣ Таблица параметров на дисплее
   // displaySensorData();       // 8️⃣ Таблица параметров на дисплее
  }
