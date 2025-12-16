	///////////////////////////////БИБЛИОТЕКИ////////////////////////////////////////////////////// 
	#include <Wire.h>                 // Библиотека I2C (обмен данными с OLED, датчиками и др.)
	#include <Adafruit_GFX.h>         // Графический движок: линии, текст, шрифты, примитивы
	//#include <Adafruit_SSD1306.h>   // Драйвер OLED-дисплея SSD1306 (128x64 по I2C)
	//#include <FluxGarage_RoboEyes.h>// Анимация глаз робота (EMO-style, эмоции, моргание)
	#include <MPU6050_light.h>        // Работа с гироскопом/акселерометром MPU6050 (наклон, движение)
	//#include "Eyes333.h"            // Набор анимаций глаз робота
	#include "FS.h"										// Для SD CARD  Файловая система
	#include "SD_MMC.h"								// SD CARD  Чтение и запись
	#include <VL53L0X.h>              // Датчик лазерный ToF-дальномер
	#include <Ultrasonic.h>           // Ультразвуковой дальномер   
	#include <Adafruit_Sensor.h>      // Общая бичисплея ST7789
	#include <Adafruit_GC9A01A.h>     // Драйвер дисплея ST7789
	#include <Adafruit_AHTX0.h>   		// для AHT20/10
	#include <Adafruit_BMP280.h>  		// для BMP280
	#include <BH1750.h>          			// для датчика освещенности

	// Модули
	//////////////////////////////////////////////////////////////////////////////////////////////
	#include "preua.h" 									 // Bitmap_s изоображений 
	//////////////////////// КОНФИГУРАЦИЯ ПИНОВ //////////////////////////////////
	#define I2C_SDA 21 
	#define I2C_SCL 22 
	#define TFT_CS   5 
	#define TFT_DC   2 
	#define TFT_RST  4 
	#define US_TRIG 26 
	#define US_ECHO 27 
	#define TTP1	  32
	#define TTP2 	  33
	#define TTP3	  25

	//////////////////////// ОБЪЕКТЫ /////////////////////////////////////////////
  //Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);	
	Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);
	MPU6050 mpu(Wire); 																	
	VL53L0X tof;																			
	Adafruit_AHTX0 aht; 																
	Adafruit_BMP280 bmp; 																
	BH1750 lightMeter;																
	Ultrasonic ultrasonic(US_TRIG, US_ECHO);											
	//////////////////////// ТАЙМЕРЫ /////////////////////////////////////////////
	unsigned long tMPU  = 0;   
	unsigned long tEnv  = 0; 					
	unsigned long tDist = 0;					
	//////////////////////// ТАЧ КНОПКИ //////////////////////////////////////////
	bool lastTTP1 = false;
	bool lastTTP2 = false;
	bool lastTTP3 = false;
	//////////////////////// ЛОГО ////////////////////////////////////////////////
	unsigned long logoStartTime = 0;
	bool logoShown = false;
	//////////////////////// ULTRASONIC ///////////////////////////////////////////
	long distanceCM = -1;                     // последнее измерение
	static unsigned long lastRead = 0;
	static const unsigned long interval = 100; // 100 мс
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
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////// ТАЧ КНОПКИ //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	
void checkTTP() {
  bool t1 = digitalRead(TTP1);
  bool t2 = digitalRead(TTP2);
  bool t3 = digitalRead(TTP3);

  if (t1 && !lastTTP1) Serial.println("TTP1: TOUCH");
  if (!t1 && lastTTP1) Serial.println("TTP1: RELEASE");

  if (t2 && !lastTTP2) Serial.println("TTP2: TOUCH");
  if (!t2 && lastTTP2) Serial.println("TTP2: RELEASE");

  if (t3 && !lastTTP3) Serial.println("TTP3: TOUCH");
  if (!t3 && lastTTP3) Serial.println("TTP3: RELEASE");

  lastTTP1 = t1;
  lastTTP2 = t2;
  lastTTP3 = t3;
}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////// SETUP ///////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	void setup() {				
		Serial.begin(115200);
		SPI.begin(18, -1, 23, TFT_CS);  // SCK, MISO, MOSI, CS
		Wire.begin(I2C_SDA, I2C_SCL);
	//	tft.init(240, 240);
		tft.begin();

		//
		    /*
				tft.setRotation(2);
				tft.fillScreen(0xF800);   // RED
				delay(3000);
				tft.fillScreen(0x07E0);   // GREEN
				delay(3000);
				tft.fillScreen(0x001F);   // BLUE
				delay(3000);
        */
						tft.fillScreen(0x0000);
						int x = (240 - PREUA_WIDTH) / 2;
						int y = (240 - PREUA_HEIGHT) / 2;
						tft.drawBitmap(x, y, preua, PREUA_WIDTH, PREUA_HEIGHT, 0xFFFF);
						logoStartTime = millis();
						logoShown = true;

				tft.setTextColor(0xFFFF);
				tft.setTextSize(2);
				tft.setCursor(20, 110);
				tft.println("TREETEX.shop");
		//
		//tft.fillScreen(ST77XX_BLACK);
		Serial.println("=== Предзагрузка датчиков ===");
		// ---- MPU6050 ----
		sensors[0].connected = mpu.begin();
		if (sensors[0].connected) mpu.calcOffsets();
		Serial.printf("Инициализация %s - %s\n", sensors[0].name,
		sensors[0].connected ? "Подключено" : "Не подключено");
		// ---- VL53L0X ----Лазерный
		sensors[1].connected = tof.init();
		if (sensors[1].connected) tof.setTimeout(500);
		Serial.printf("Инициализация %s - %s\n", sensors[1].name,
									sensors[1].connected ? "Подключено" : "Не подключено");
		// ---- AHT20 ----
		sensors[2].connected = aht.begin();
		Serial.printf("Инициализация %s - %s\n", sensors[2].name,
									sensors[2].connected ? "Подключено" : "Не подключено");
		// ---- BMP280 ----
		sensors[3].connected = bmp.begin(0x77);  //0x76
		Serial.printf("Инициализация %s - %s\n", sensors[3].name,
									sensors[3].connected ? "Подключено" : "Не подключено");
		// ---- BH1750 ----
		sensors[4].connected = lightMeter.begin();
		Serial.printf("Инициализация %s - %s\n", sensors[4].name,
									sensors[4].connected ? "Подключено" : "Не подключено");
		// ---- SD_MMC ----
		sensors[5].connected = SD_MMC.begin();
		Serial.printf("%s - %s\n", sensors[5].name,
									sensors[5].connected ? "Подключено" : "Не подключено");
		// ---- Ультразвуковой датчик ----
		ultrasonicInit(US_TRIG, US_ECHO);
		Serial.printf("%s - Инициализирован\n", sensors[6].name);
		Serial.println("=== Все датчики проверены ===");


		// ---- ТАЧ КНОПКИ ----
		pinMode(TTP1, INPUT);
		pinMode(TTP2, INPUT);
		pinMode(TTP3, INPUT);
		Serial.println("TTP223 Тач инициализированы");

	}
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////// LOOP ////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
 void loop() {

  unsigned long now = millis();

  ///////////////////////// LOGO //////////////////////////////////////////////
  if (logoShown) {
    if (now - logoStartTime >= 95000) {
      tft.fillScreen(0x0000);   // убрать лого
      logoShown = false;
    }
    return; // ❗ ПОКА ЛОГО — НИЧЕГО ДАЛЬШЕ НЕ ВЫПОЛНЯЕМ
  }
  /////////////////////////////////////////////////////////////////////////////

  // ===================== ОСНОВНАЯ ЛОГИКА ===================================

  // --- MPU6050 ---
  if (sensors[0].connected && now - tMPU > 50) {
    tMPU = now;
    mpu.update();
  }

  // --- Температура / давление / свет ---
  if ((sensors[2].connected || sensors[3].connected || sensors[4].connected) && now - tEnv > 2000) {
    tEnv = now;

    float tempVal = 0, humVal = 0, pressure = 0, lux = 0;
    sensors_event_t hum, temp;

    if (sensors[2].connected) {
      aht.getEvent(&hum, &temp);
      tempVal = temp.temperature;
      humVal  = hum.relative_humidity;
    }

    if (sensors[3].connected)
      pressure = bmp.readPressure() / 100.0;

    if (sensors[4].connected)
      lux = lightMeter.readLightLevel();

    Serial.printf(
      "T: %.1f  H: %.1f  P: %.1f  Lux: %.1f\n",
      tempVal, humVal, pressure, lux
    );
  }

  // --- Ультразвук (асинхронно) ---
  if (sensors[6].connected) {
    ultrasonicUpdate();

    if (now - tDist > 500) {
      tDist = now;
      Serial.printf("Дальность : %ld cm\n", ultrasonicGetDistance());
    }
  }
////////////////////////////Лазерній дальномер////////////////////////////////////////////////
// --- VL53L0X ---

			tof.startContinuous();

    if (sensors[1].connected) {
        Serial.print("VL53L0X Лазер: ");
        Serial.print(tof.readRangeContinuousMillimeters());
        Serial.println(" mm");
    }
		
  // ---- ТАЧ КНОПКИ ----
  checkTTP();
}
