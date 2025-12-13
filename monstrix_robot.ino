/*
 * MONSTRIX Emotional Robot
 * ESP32-C3 + OLED 0.96" + MPU6050 + FluxGarage_RoboEyes
 *
 * Hardware:
 * - ESP32-C3 microcontroller
 * - OLED 0.96" SSD1306 display (I2C address 0x3C)
 * - MPU6050 gyroscope/accelerometer
 * - I2C connections: SDA=GPIO8, SCL=GPIO9
 *
 * Dependencies:
 * - Adafruit_GFX
 * - Adafruit_SSD1306
 * - FluxGarage_RoboEyes
 * - MPU6050_light
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>
#include <MPU6050_light.h>
#include "Eyes333.h"
// Display configuration
#define SDA_PIN 8
#define SCL_PIN 9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define TOUCH_PIN 3                // сенсорная кнопка на пине 3
#define TRIPLE_CLICK_INTERVAL 1000 // интервал между нажатиями для распознавания тройного клика (мс)

// Create objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RoboEyes<Adafruit_SSD1306> eyes(display);
MPU6050 mpu(Wire);


// Timing variables
unsigned long lastEmotionChange = 0;
unsigned long lastAngryTime = 0;
const int EMOTION_INTERVAL = 6000; // Change emotion every 6 seconds
const int ANGRY_DURATION = 3000;   // Stay angry for 3 seconds
bool isAngry = false;
//-----------------------------
unsigned long lastTouchTime = 0;
int touchCount = 0;
//-----------------------------

// ----------------- Freeze variables -----------------
bool freezeMood = false;                        // флаг фиксации эмоции
unsigned long freezeStart = 0;
const unsigned long FREEZE_DURATION = 300000UL;//3600000UL; // 1 час в миллисекундах
// 5 мин
// Текущий выбранный face (0..23) — сохраняется в setRandomMood()
int currentFace = 0;

const unsigned char preua [] PROGMEM = {
 // 'emblem_ukraine_coat_of_arms_trident_icon_232970 (1), 126x64px
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x0f, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xfc, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x0c, 0x00, 0x03, 0xfc, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x1e, 0x00, 0x01, 0xfc, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xc0, 0x00, 0x3f, 0x00, 0x00, 0xfc, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xc0, 0x00, 0x7f, 0x80, 0x00, 0xfc, 
	0xfe, 0x7f, 0xfc, 0xfc, 0xcf, 0xcf, 0x0c, 0x3f, 0xf9, 0xff, 0x80, 0x00, 0x7f, 0x80, 0x00, 0x7c, 
	0xfe, 0x7f, 0xfc, 0xfc, 0xcf, 0xce, 0x00, 0x1f, 0xf9, 0xff, 0x80, 0x00, 0x7f, 0x80, 0x00, 0x7c, 
	0xfe, 0x7f, 0xfe, 0x7c, 0xcf, 0xcc, 0x61, 0x8f, 0xf9, 0xff, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x3c, 
	0xfe, 0x7f, 0xfe, 0x7c, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x3c, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x3c, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x3c, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x00, 0xc0, 0x0c, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xfe, 0x00, 0xc0, 0x0c, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xfc, 0x7f, 0x8f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xfe, 0x7c, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xf8, 0x7f, 0x87, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xfc, 0xfc, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xf8, 0x3f, 0x07, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xfc, 0xfc, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xf0, 0x3f, 0x03, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xfc, 0xfc, 0xcf, 0xcc, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xf0, 0x3f, 0x03, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xfc, 0x7c, 0xe7, 0x9c, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xf8, 0x1e, 0x07, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xfe, 0x00, 0xe0, 0x1c, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xf8, 0x1e, 0x07, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x00, 0xf8, 0x7c, 0xf3, 0xcf, 0xf9, 0xff, 0x00, 0xfc, 0x1e, 0x0f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0x00, 0xfe, 0x1e, 0x1f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0x00, 0xfe, 0x1e, 0x1f, 0xc0, 0x3c, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0xfe, 0x1e, 0x1f, 0xc0, 0x3c, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0xfe, 0x1e, 0x1f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0x00, 0xfe, 0x1e, 0x1f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0x00, 0xfe, 0x1e, 0x1f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x3e, 0x1f, 0xf3, 0xfc, 0x03, 0xf9, 0xff, 0x00, 0xfe, 0x1e, 0x1f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x38, 0x07, 0xf3, 0xf8, 0x03, 0xf9, 0xff, 0x00, 0xfc, 0x1e, 0x0f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x39, 0xe7, 0xf3, 0xf1, 0xff, 0xf9, 0xff, 0x00, 0xfc, 0x3f, 0x0f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x33, 0xf3, 0xf3, 0xf3, 0xff, 0xf9, 0xff, 0x00, 0xfc, 0x3f, 0x0f, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x33, 0xf3, 0xf3, 0xf3, 0xff, 0xf9, 0xff, 0x00, 0xf8, 0x3f, 0x07, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x33, 0xf3, 0xf3, 0xf3, 0xff, 0xf9, 0xff, 0x00, 0xf8, 0x3f, 0x07, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x33, 0xf3, 0xf3, 0xf1, 0xff, 0xf9, 0xff, 0x00, 0xf0, 0x3f, 0x03, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xff, 0x33, 0xf3, 0xf3, 0xf8, 0x0f, 0xf9, 0xff, 0x00, 0xe0, 0x3f, 0x01, 0xc0, 0x3c, 
	0xfe, 0x7f, 0xc0, 0x33, 0xf3, 0xf3, 0xfc, 0x07, 0xf9, 0xff, 0x00, 0xc0, 0x3f, 0x00, 0xc0, 0x3c, 
	0xfe, 0x7f, 0x80, 0x33, 0xf3, 0xf3, 0xff, 0xe3, 0xf9, 0xff, 0x00, 0x80, 0x3f, 0x00, 0x40, 0x3c, 
	0xfe, 0x7f, 0x1f, 0x33, 0xf3, 0xf3, 0xff, 0xf3, 0xf9, 0xff, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x3c, 
	0xfe, 0x7f, 0x3f, 0x33, 0xf3, 0xf3, 0xff, 0xf3, 0xf9, 0xff, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3c, 
	0xfe, 0x7f, 0x3f, 0x33, 0xf3, 0xf3, 0xff, 0xf3, 0xf9, 0xff, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3c, 
	0xfe, 0x7f, 0x1f, 0x39, 0xe7, 0xf3, 0xff, 0xe3, 0xf9, 0xff, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x3c, 
	0xfe, 0x7f, 0x80, 0x38, 0x07, 0x00, 0x30, 0x07, 0xf9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 
	0xfe, 0x7f, 0xc0, 0x3e, 0x1f, 0x00, 0x30, 0x0f, 0xf9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 
	0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7c, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc
};
 



void setup() {
  // Initialize I2C (ESP32-C3 uses GPIO8=SDA, GPIO9=SCL by default)
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT_PULLDOWN);
  Serial.println("UA Стартуємо...");
    // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 allocation failed");
    return;
  }
//-------------------------------------------
  // ------------------ Дальше стандартный setup ------------------
  eyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60);
  //---------------------Герб
  display.drawBitmap(0, 0, preua, 126, 64, WHITE);
  display.display();

  delay(3000); // Показывать 3 секунды Герб
  display.clearDisplay();
  display.display();
  //---------------------
  eyes.setAutoblinker(ON, 3, 1);
  eyes.setIdleMode(ON, 2, 1);
  eyes.setMood(DEFAULT);

  Serial.begin(115200);
//-------------------------------------------


  display.setRotation(2);
  display.clearDisplay();
  display.display();
  Serial.println("Display initialized");

  // Initialize robot eyes
  eyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60);
  eyes.setAutoblinker(ON, 3, 1);  // Auto blink every 3 seconds
  eyes.setIdleMode(ON, 2, 1);     // Idle animations every 2 seconds
  eyes.setMood(DEFAULT);          // Start with default mood
  Serial.println("RoboEyes initialized");

  // Initialize MPU6050
  byte status = mpu.begin();
  if(status != 0) {
    Serial.print("MPU6050 connection failed. Status: ");
    Serial.println(status);
  } else {
    Serial.println("MPU6050 connected");
  }

  delay(1000);
  mpu.calcGyroOffsets(); // Calibrate gyroscope (keep robot still!)
  Serial.println("MPU6050 calibrated");

  Serial.println("MONSTRIX Robot Ready!");
  Serial.println("Shake me to see anger! 😠");

  // Start with a random mood
  lastEmotionChange = millis();
  setRandomMood();
}

bool showBattery = false;
unsigned long batteryStartTime = 0;
const unsigned long BATTERY_DISPLAY_TIME = 3000; // 3 секунды

void loop() {
  // Update sensors and display



  unsigned long now = millis();

  // Обновляем глаза/MPU если не показываем батарею.
  // Если freeze активен, глаза всё равно обновляем (анимация хранится в eyes.update()).
  if (!showBattery) {
    mpu.update();
    eyes.update();
  }

  delay(16); // ~60 FPS

  // Calculate total acceleration (shake detection)
  float acc = abs(mpu.getAccX()) + abs(mpu.getAccY()) + abs(mpu.getAccZ());

  // ----------------- Обработка тач-кнопки -----------------
  static bool touched = false;
  int touchVal = digitalRead(TOUCH_PIN);
  unsigned long nowPI = millis();

  if (touchVal == HIGH && !touched) {
    touched = true;
    if (nowPI - lastTouchTime > TRIPLE_CLICK_INTERVAL) {
      touchCount = 0; // сбрасываем счётчик, если долго не нажимали
    }
    touchCount++;
    lastTouchTime = nowPI;

    if (touchCount == 1) {
      // 1 нажатие — показываем батарею (как было)
      showBattery = true;
      batteryStartTime = nowPI;
    }
    else if (touchCount == 2) {
      // 2 нажатия — новый рандом
      setRandomMood();
      // currentFace обновится внутри setRandomMood()
    }
    else if (touchCount == 3) {
      // 3 нажатия — freeze текущего face на 1 час
      Serial.println("3 нажатия — фиксируем текущий рандом на 1 час");

      // НЕ выключаем батарею принудительно здесь — пользователь должен иметь возможность увидеть батарею даже в freeze
      // Включаем freeze и сохраняем время
      freezeMood = true;
      freezeStart = nowPI;
       Serial.print(freezeStart);
      // Перерисуем тот face, который считается текущим (currentFace)
      setFaceByNumber(currentFace);
      // сбрасываем счетчик, чтобы следующий клик начинал новый цикл
      touchCount = 0;
      Serial.print("Freeze started for face = ");
     
    }
    // прочие нажатия игнорируем, счетчик сбросится по таймауту
  }

  if (touchVal == LOW && touched) {
    touched = false;
  }

  if (touchCount > 0 && nowPI - lastTouchTime > TRIPLE_CLICK_INTERVAL) touchCount = 0;

  // ----------------- ПОКАЗ БАТАРЕИ (перед freeze) -----------------
  // показываем батарею независимо от freeze — это позволяет 1 нажатие всегда показывать заряд
  if (showBattery) {
      int raw = analogRead(A0);
      float batteryVoltage = raw * (3.3 / 4095.0) * 2.2; // 2.0 → 2.2 под 5.1V
      if (batteryVoltage > 5.1) batteryVoltage = 5.1;    // лимит 100%
      if (batteryVoltage < 3.3) batteryVoltage = 3.3;    // лимит 0%
      int percent = map((int)(batteryVoltage * 1000), 3300, 5100, 0, 100);

      // Ограничиваем диапазон
      if (percent < 0) percent = 0;
      if (percent > 100) percent = 100;

      // Вывод в Serial
      Serial.print("DC: ");
      Serial.print(batteryVoltage);
      Serial.print(" V, Battery: ");
      Serial.print(percent);
      Serial.println("%");

      // Формируем градиентную полоску
      const int segments = 6; // число сегментов
      int filled = percent / (100 / segments);          // полные блоки
      int partial = ((percent % (100 / segments)) > 0) ? 1 : 0; // один полублок
      int empty = segments - filled - partial;

      String bar = "";   // [=][=|][=][=,][ ][ ]=
      for (int i = 0; i < filled; i++) bar += "[]";
      for (int i = 0; i < partial; i++) bar += "|";
      for (int i = 0; i < empty; i++) bar += "=";
      bar += "+";

      // Отображение на OLED
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);

      // Центрируем полоску
      display.setCursor((SCREEN_WIDTH - (bar.length() * 6)) / 2, SCREEN_HEIGHT / 2 - 4);
      display.print(bar);

      // Процент под полоской
      String percText = String(percent) + "%";
      display.setCursor((SCREEN_WIDTH - (percText.length() * 6)) / 2, SCREEN_HEIGHT / 2 + 8);
      display.print(percText);

      display.display();

      // Проверка времени показа батареи
      if (millis() - batteryStartTime > BATTERY_DISPLAY_TIME) {
          showBattery = false;
          display.clearDisplay();
          display.display();
      }

      return; // чтобы остальные действия в loop не выполнялись
  }

  // ----------------- Если freeze активен — блокируем автосмену и встряхивания, но позволяем батарее показываться выше -----------------
  if (freezeMood) {
    // проверяем окончание freeze
    if (now - freezeStart >= FREEZE_DURATION) {
      freezeMood = false;
      lastEmotionChange = now; // сбрасываем таймер автосмены
      Serial.println("Freeze закончен — возвращаемся к рандомной смене эмоций");
      setRandomMood();
    } else {


        // Поддерживаем текущий face
        setFaceByNumber(currentFace);
        eyes.update();

        // --- Вывод оставшегося времени freeze ---
        unsigned long remaining = FREEZE_DURATION - (millis() - freezeStart);
        unsigned long seconds = remaining / 1000;
        unsigned long minutes = seconds / 60;
        seconds = seconds % 60;
        Serial.print("Freeze осталось: ");
        Serial.print(minutes);
        Serial.print(" мин ");
        Serial.print(seconds);
        Serial.println(" сек");

      // пока freeze активен — показываем и поддерживаем выбранный face
 
      return; // ничего больше не делаем
    }
  }

  // ----------------- Check for shake (if not already angry) -----------------
  if (!isAngry && acc > 2.5) {
    Serial.println("Shake detected! Getting angry... 😠");
    eyes.setMood(ANGRY);
    isAngry = true;
    lastAngryTime = now;
    return;
  }

  // ----------------- Check if anger duration expired -----------------
  if (isAngry && (now - lastAngryTime > ANGRY_DURATION)) {
    Serial.println("Calming down... 😌");
    isAngry = false;
    lastEmotionChange = now; // Reset emotion timer
    setRandomMood();
    return;
  }

  // ----------------- Auto emotion change (only if not angry) -----------------
  if (!isAngry && (now - lastEmotionChange > EMOTION_INTERVAL)) {
    lastEmotionChange = now;
    setRandomMood();
  }

  //----------------loop next------>
}

// ===================== setRandomMood() (оставлена без изменений логики) =====================
void setRandomMood() {
   eyes.setSweat(OFF);
    eyes.setCuriosity(OFF);
       eyes.setVFlicker(OFF, 0);
        eyes.setHFlicker(OFF, 0);
  int mood = random(0, 31);

  // Сохраняем текущий face чтобы при freeze зафиксировать именно его
  currentFace = mood;
    
  String moodName;
switch (mood) {        
    // ═══════════════════════════════════════════════════════════════
    case 0:
      eyes.RotSetEnabled(false);                       // Рот полностью выключен
      eyes.setMood(DEFAULT);                           // Нейтральное лицо (базовое состояние)
      eyes.GlasSize(8, 13, 8);                         // Зрачки быстро увеличиваются с 8 до 13 пикселей — лёгкое удивление
      eyes.setGlas(0, -4, 0, -4, 2, 16);                  // Взгляд чуть вверх, плавно + живые саккады (естественное движение зрачков)
      moodName = "Case 0: По умолчанию + лёгкое удивление и восторг";
      break;

    case 1:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(HAPPY);                             // Счастливое выражение лица
      eyes.setGlas(-8, 0, 8, 0, 3, 18);                // Глаза смотрят в разные стороны — весёлое безумие
      eyes.setPupilSize(12);                           // Большие радостные зрачки — 12 пикселей
      moodName = "Case 1: Счастливый — глаза бегают, как у шалуна";
      break;

    case 2:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(TIRED);                             // Усталое выражение лица
      eyes.setGlas(0, 0, 0, 0, 1, 20);                 // Взгляд неподвижен — зрачки почти не двигаются
      eyes.setPupilSize(8);                            // Зрачки маленькие — 8 пикселей (усталость)
      moodName = "Case 2: Усталый — смотрит в одну точку, как зомби";
      break;

    case 3:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.anim_confused();                            // Анимация тряски глаз — растерянность
      eyes.setPupilSize(10);                           // Зрачки средние — 10 пикселей
      moodName = "Case 3: Смущённый / растерянный — глаза трясутся";
      break;

    case 4:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение лица
      eyes.setPupilSize(7);                            // Самые маленькие зрачки — 7 пикселей (чистая злость)
      moodName = "Case 4: Злой — базовое выражение злости";
      break;

    case 5:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.blink(0,1);                                 // Моргание только правым глазом
      eyes.GlasSize(13, 7, 30);                        // Зрачки медленно сужаются с 13 до 7 — злой прищур
      moodName = "Case 5: Злой прищур — моргает правым глазом";
      break;

    case 6:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.blink(1,1);                                 // Обычное моргание обоими глазами
      eyes.setPupilSize(10);                           // Зрачки средние — 10 пикселей
      moodName = "Case 6: Обычное моргание — спокойное состояние";
      break;

    case 7:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(DEFAULT);                           // Полный сброс всех эмоций
      eyes.setPupilSize(10);                           // Возврат к стандартному размеру зрачков
      moodName = "Case 7: Полный сброс — нейтральное лицо";
      break;

    case 8:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.GlasSize(13, 7, 4);                         // Зрачки РЕЗКО сужаются до 7 пикселей — вспышка ярости!
      moodName = "Case 8: Злой — максимальная напряжённость и ярость";
      break;

    case 9:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.setPupilSize(7);                            // Самые маленькие зрачки — настороженность
      eyes.setGlas(0, 0, 0, 0, 2, 18);                 // Живые саккады — зрачки слегка дрожат
      moodName = "Case 9: Злой — настороженный, готов к атаке";
      break;

    case 10:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.setPosition(NW);                            // Взгляд в левый верхний угол
      eyes.setPupilSize(7);                            // Маленькие злые зрачки
      moodName = "Case 10: Злой — смотрит влево-вверх (угрожающе)";
      break;

    case 11:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.setPosition(NE);                            // Взгляд в правый верхний угол
      eyes.setPupilSize(7);                            // Маленькие злые зрачки
      moodName = "Case 11: Злой — смотрит вправо-вверх (угрожающе)";
      break;

    case 12:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.blink(1,0);                                 // Моргание только левым глазом
      eyes.setPupilSize(7);                            // Маленькие злые зрачки
      moodName = "Case 12: Злой — моргает левым глазом (подозрительно)";
      break;

    case 13:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.setWidth(50, 50);                           // Глаза сильно увеличены
      eyes.setHeight(50, 50);
      eyes.setPupilSize(8);                            // Зрачки чуть больше — шок + злость
      moodName = "Case 13: Злой — выпученные глаза (шок + злость)";
      break;

    case 14:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.setVFlicker(ON,1);                          // Вертикальная дрожь глаз
      eyes.setHFlicker(ON,2);                          // Горизонтальная дрожь глаз
      eyes.setPosition(N);                             // Смотрит вверх
      eyes.setCuriosity(ON);                           // Глаза увеличиваются при взгляде вбок
      eyes.setPupilSize(7);                            // Самые маленькие зрачки — ярость
      moodName = "Case 14: Злой — на чеку, нервный, готов к прыжку";
      break;

    case 15:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.setWidth(37,37); eyes.setHeight(26,26);      // Маленькие глаза
      eyes.setSweat(ON);                               // Включаем пот на лбу
      eyes.setPupilSize(8);                            // Зрачки маленькие — усталость
      moodName = "Case 15: Больной — усталый, потный, слабый";
      break;

    case 16:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.setWidth(36,40); eyes.setHeight(26,26);      // Маленькие глаза
      eyes.setHFlicker(ON,1);                          // Горизонтальная дрожь
      eyes.setSweat(ON);                               // Пот включён
      eyes.setPupilSize(7);                            // Самые маленькие зрачки
      moodName = "Case 16: Больной — раздражённый, потный, нервный";
      break;

    case 17:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.setWidth(36,40); eyes.setHeight(26,26);      // Маленькие глаза
      eyes.setSweat(ON);                               // Пот включён
      eyes.setPupilSize(8);                            // Зрачки маленькие
      moodName = "Case 17: Больной — нервный, потный, слабый";
      break;

    case 18:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.setWidth(36,40); eyes.setHeight(26,26);      // Маленькие глаза
      eyes.setSweat(ON);                               // Пот включён
      eyes.blink(1,1);                                 // Моргание обоими глазами
      eyes.setPupilSize(8);                            // Зрачки маленькие
      moodName = "Case 18: Больной — слабое моргание, потный";
      break;

    case 19:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.setWidth(36,40); eyes.setHeight(26,26);      // Маленькие глаза
      eyes.setSweat(ON);                               // Пот включён
      eyes.setPupilSize(8);                            // Зрачки маленькие
      moodName = "Case 19: Больной — максимум пота, нервный";
      break;

    case 20:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(HAPPY);                             // Счастливое выражение
      eyes.setBorderradius(6, 6);                      // Глаза чуть менее круглые
      eyes.setCuriosity(ON);                           // Любопытство — глаза увеличиваются при взгляде вбок
      eyes.setPupilSize(13);                           // Самые большие зрачки — 13 пикселей
      moodName = "Case 20: Счастливый — милые круглые глаза, любопытство";
      break;

    case 21:
      eyes.RotSetEnabled(false);                       // Рот выключен
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.setWidth(40, 40);                           // Глаза чуть больше
      eyes.setHeight(25, 25);
      eyes.setSpacebetween(15);
      eyes.setBorderradius(3, 3);                       // Глаза более овальные
      eyes.setCuriosity(ON);
      eyes.setPupilSize(8);                            // Зрачки маленькие
      moodName = "Case 21: Усталый — маленькие прищуренные глаза";
      break;

    case 22:
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.RotSetEnabled(true);                        // РОТ ВКЛЮЧЁН (согласно твоему оригиналу)
      eyes.RotSetMood(HAPPY);                          // Улыбка
      eyes.RotSetAutoblinker(true);                    // Рот дышит/моргает
      eyes.RotAnim_laugh();                            // Смеётся
      eyes.setWidth(40, 40);
      eyes.setHeight(25, 25);
      eyes.setSpacebetween(10);
      eyes.setBorderradius(1, 1);
      eyes.setCuriosity(OFF);
      eyes.setPupilSize(12);                           // Большие зрачки — радость
      moodName = "Case 22: Усталый, но улыбается и смеётся";
      break;

    case 23:
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.RotSetEnabled(true);                        // РОТ ВКЛЮЧЁН (согласно оригиналу)
      eyes.RotSetMood(ANGRY);                          // Злой рот
      eyes.RotSetAutoblinker(true);                    // Рот дышит
      eyes.setWidth(40, 40);
      eyes.setHeight(23,23);
      eyes.setSpacebetween(15);
      eyes.setBorderradius(3, 3);
      eyes.setPupilSize(7);                            // Маленькие злые зрачки
      moodName = "Case 23: Усталый и злой — ворчит сквозь сон";
      break;

    case 24:
      eyes.RotSetSpeaking(true, 70, 280, 14);           // САМЫЙ ЖИВОЙ РЕЧЬ (рот говорит реалистично)
      eyes.setIdleMode(true);                          // Глаза бегают (любопытство)
      eyes.setAutoblinker(true, 3, 5);                  // Естественное моргание
      eyes.setPupilSize(11);                           // Зрачки средние
      moodName = "Case 24: Разговор — максимально реалистичный голос";
      break;

    case 25:
      eyes.RotSetGrimace(true, 150);                    // Постоянные гримасы ртом
      eyes.anim_confused();                            // Глаза трясутся
      eyes.setMood(HAPPY);                             // Счастливое выражение
      eyes.setPupilSize(12);                           // Большие зрачки — веселье
      moodName = "Case 25: Шутник — кривляется и пугает";
      break;

    case 26:
      eyes.setMood(ANGRY);                             // Злое выражение
      eyes.RotSetMood(ANGRY);                          // Злой рот
      eyes.RotSetAngryBreathing(true);                 // Тяжёлое злобное дыхание ртом
      eyes.setPupilSize(7);                            // Самые маленькие зрачки
      moodName = "Case 26: Злой — сдержанная ярость, дышит как дракон";
      break;

    case 27:
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.RotSetMood(TIRED);                          // Усталый рот
      eyes.RotSetYawn(true);                           // Зевает ртом
      eyes.setPupilSize(8);                            // Зрачки маленькие
      moodName = "Case 27: Сонный — зевает, хочет спать";
      break;

    case 28:
      eyes.RotSetSpeaking(true, 80, 250, 10);           // Речь на 5 секунд
      delay(5000);
      eyes.RotSetSpeaking(false);
      eyes.setPupilSize(11);                           // Зрачки средние
      moodName = "Case 28: Говорит 5 секунд — реалистичная речь";
      break;

    case 29:
      eyes.setMood(SURPRISED);                         // Удивлённое выражение
      eyes.RotSetMood(SURPRISED);                      // Рот в состоянии удивления
      delay(1000);
      eyes.RotSetLaughing(LAUGH_STRONG, 2000);          // Сильный смех ртом
      delay(2500);
      eyes.setMood(SAD);                               // Грустное выражение
      eyes.RotSetMood(SAD);
      eyes.RotSetSigh(true);                           // Вздох ртом
      delay(3000);
      eyes.setPupilSize(13);                           // Максимальные зрачки при удивлении
      moodName = "Case 29: Комбо — удивление → смех → грусть и вздох";
      break;

    case 30:
      eyes.setMood(SURPRISED);                         // Удивлённое выражение
      eyes.RotSetMood(SURPRISED);                      // Рот удивлён
      eyes.setPupilSize(13);                           // Максимальные зрачки — шок
      eyes.setPupilOffset(0, 10, 0, 10);                // Смотрит вниз
      eyes.RotSetSpeaking(true, 70, 250, 12);           // Говорит
      moodName = "Case 30: Удивлённый — огромные зрачки, смотрит вниз, говорит";
      break;

    case 31:
      eyes.setMood(TIRED);                             // Усталое выражение
      eyes.RotSetMood(SURPRISED);                      // Рот в состоянии удивления
      eyes.setGlas(0, -6, 0, -6, 2, 15);                // Взгляд вверх
      eyes.GlasSize(10, 13, 12);                      // Зрачки увеличиваются до 13 пикселей
      eyes.setPupilOffset(10, 0, 10, 0);               // Смотрит вправо
      eyes.RotSetSpeaking(true, 70, 250, 12);           // Говорит
      moodName = "Case 31: Усталый + удивлённый — зрачки растут, смотрит вверх-вправо, говорит";
      break;

    // ═══════════════════════════════════════════════════════════════

  }
  Serial.print("Новая эмоция: ");
  Serial.println(moodName);

 //-------------------------------------------------

}

// Вставляем функцию, которая воспроизводит нужный face по номеру
void setFaceByNumber(int mood) {
  switch (mood) {
    case 0:
      eyes.setMood(DEFAULT);
      break;
    case 1:
      eyes.setMood(HAPPY);
      break;
    case 2:
      eyes.setMood(TIRED);
      break;
    case 3:
      eyes.anim_confused();
      break;
    case 4:
      eyes.setMood(ANGRY);
      break;
    case 5:
      eyes.blink(0,1);
      break;
    case 6:
      eyes.blink(1,1);
      break;
    case 7:
      eyes.setMood(DEFAULT);
      break;
    case 8:
      eyes.setMood(ANGRY);
      break;
    case 9:
      eyes.setMood(ANGRY);
      eyes.setWidth(50, 50);
      eyes.setHeight(50, 50);
      break;
    case 10:
      eyes.setMood(ANGRY);
      eyes.setPosition(NW);
      break;
    case 11:
      eyes.setMood(ANGRY);
      eyes.setPosition(NE);
      break;
    case 12:
      eyes.setMood(ANGRY);
      eyes.blink(1,0);
      break;
    case 13:
      eyes.setMood(ANGRY);
      eyes.blink(0,1);
      break;
    case 14:
      eyes.setMood(ANGRY);
      eyes.setVFlicker(ON,1);
      eyes.setHFlicker(ON,2);
      eyes.setPosition(N);
      eyes.setCuriosity(ON);
      break;
    case 15:
      eyes.setMood(TIRED);
      eyes.setWidth(37,37);
      eyes.setHeight(26,26);
      break;
    case 16:
      eyes.setMood(ANGRY);
      eyes.setWidth(36,40);
      eyes.setHeight(26,26);
      eyes.setHFlicker(ON,1);
      break;
    case 17:
      eyes.setMood(ANGRY);
      eyes.setVFlicker(ON,1);
      eyes.setHFlicker(ON,2);
      eyes.setPosition(N);
      eyes.setCuriosity(ON);
      break;
    case 18:
      eyes.setMood(TIRED);
      eyes.setWidth(36,40); 
      eyes.setHeight(26,26);
      eyes.blink(1,1);
      break;
    case 19:
      eyes.setMood(TIRED);
      eyes.setWidth(36,40);
      eyes.setHeight(26,26);
      eyes.setSweat(ON);
      break;
    case 20:
      eyes.setMood(TIRED);
      eyes.setWidth(36,40); 
      eyes.setHeight(26,26);
      eyes.setSweat(ON);
      break;
    case 21:
      eyes.setMood(HAPPY); 
      eyes.setBorderradius(6, 6);
      eyes.setCuriosity(ON);
      break;
    case 22:
      eyes.setMood(TIRED); 
      eyes.setWidth(40, 40);
      eyes.setHeight(25, 25);
      eyes.setSpacebetween(15);
      eyes.setBorderradius(3, 3);
      eyes.setCuriosity(ON);
      break;
    case 23:
      eyes.setMood(TIRED); 
      eyes.setWidth(40, 40);
      eyes.setHeight(25, 25);
      eyes.setSpacebetween(10);
      eyes.setBorderradius(1, 1);
      eyes.setCuriosity(OFF);
      break;
    default:
      eyes.setMood(TIRED); 
      eyes.setWidth(40, 40);
      eyes.setHeight(10, 10);
      eyes.setSpacebetween(15);
      eyes.setBorderradius(3, 3);
      break;
  }
}    