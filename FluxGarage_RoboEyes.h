#ifndef _FLUXGARAGE_ROBOEYES_H
#define _FLUXGARAGE_ROBOEYES_H


// Цвета дисплея
uint8_t BGCOLOR = 0; // фон и оверлеи
uint8_t MAINCOLOR = 1; // рисунки

// Для переключателя типа настроения
#define DEFAULT 0
#define TIRED 1
#define ANGRY 2
#define HAPPY 3
#define SAD 4    // Новый: грусть
#define SURPRISED 5  // Новый: удивление

// Для включения или отключения чего-либо
#define ON 1
#define OFF 0

// Для переключателя "предопределённых позиций"
#define N 1 // север, верхний центр
#define NE 2 // северо-восток, верхний правый
#define E 3 // восток, средний правый
#define SE 4 // юго-восток, нижний правый
#define S 5 // юг, нижний центр
#define SW 6 // юго-запад, нижний левый
#define W 7 // запад, средний левый
#define NW 8 // северо-запад, верхний левый 
// для центрального положения используйте "DEFAULT"

// Уровни смеха
#define LAUGH_STRONG 2
#define LAUGH_WEAK 1
#define SMIRK 0  // Ухмылка

// Конструктор: принимает ссылку на активный объект дисплея Adafruit (например, Adafruit_SSD1327)
// Пример: roboEyes<Adafruit_SSD1327> = eyes(display);
template<typename AdafruitDisplay>
class RoboEyes
{
private:

// Да, всё пока остаётся доступным. Будьте ответственны и не портите ничего :)

public:

// Ссылка на объект дисплея Adafruit
AdafruitDisplay *display;

// Для общей настройки - размер экрана и макс. частота кадров
int screenWidth = 128; // ширина OLED-дисплея в пикселях
int screenHeight = 64; // высота OLED-дисплея в пикселях
int frameInterval = 20; // значение по умолчанию для 50 кадров в секунду (1000/50 = 20 миллисекунд)
unsigned long fpsTimer = 0; // для измерения кадров в секунду

// Для управления типами настроения и выражениями
bool tired = 0;
bool angry = 0;
bool happy = 0;
bool sad = 0;     // Новый: грусть
bool surprised = 0;  // Новый: удивление
bool curious = 0; // если true, при взгляде влево/вправо внешний глаз рисуется больше
bool cyclops = 0; // если true, рисуется только один глаз
bool eyeL_open = 0; // левый глаз открыт или закрыт?
bool eyeR_open = 0; // правый глаз открыт или закрыт?

// Новое для глаз как у EMO: зрачки и круглый режим
bool roundEyes = true;  // По умолчанию круглые, как у EMO
int pupilSize = 10;  // Размер зрачка
int pupilL_XOffset = 0;  // Смещение зрачка левого глаза (для взгляда)
int pupilL_YOffset = 0;
int pupilR_XOffset = 0;
int pupilR_YOffset = 0;
bool pupilsEnabled = true;  // Включить зрачки (как у EMO)

//********************************************************************************************* 
// РОТ - ОСНОВНЫЕ ПЕРЕМЕННЫЕ
//*********************************************************************************************
bool RotEnabled = 0;              // включить/выключить отображение рта
bool RotOpen = 0;                 // рот открыт или закрыт
bool RotSmile = 0;                // улыбка
bool RotSmileBad = 0;             // грустная улыбка (перевернутая)
bool RotSmileLine = 0;            // прямая линия рта
bool RotCurious = 0;              // любопытный рот

// Геометрия рта - размеры
int RotWidthDefault = 80;
int RotHeightDefault = 16;
int RotWidthCurrent = RotWidthDefault;
int RotHeightCurrent = 1;         // начинаем с закрытого рта
int RotWidthNext = RotWidthDefault;
int RotHeightNext = RotHeightDefault;

// Геометрия рта - радиус скругления
byte RotBorderRadiusDefault = 8;
byte RotBorderRadiusCurrent = RotBorderRadiusDefault;
byte RotBorderRadiusNext = RotBorderRadiusDefault;

// Геометрия рта - позиция
int RotXDefault = (128 - 80) / 2;  // будет пересчитано в begin()
int RotYDefault = 50;
int RotX = RotXDefault;
int RotY = RotYDefault;
int RotXNext = RotX;
int RotYNext = RotY;

// Выражения рта - губы
byte RotLipsTopHeight = 0;
byte RotLipsTopHeightNext = 0;
byte RotLipsBottomOffset = 0;
byte RotLipsBottomOffsetNext = 0;

// Анимация рта - автооткрывание
bool RotAutoblinker = 0;
int RotBlinkInterval = 2;
int RotBlinkIntervalVariation = 3;
unsigned long RotBlinktimer = 0;

// Анимация рта - режим простоя
bool RotIdle = 0;
int RotIdleInterval = 2;
int RotIdleIntervalVariation = 4;
unsigned long RotIdleAnimationTimer = 0;

// Анимация рта - горизонтальная дрожь
bool RotHFlicker = 0;
bool RotHFlickerAlternate = 0;
byte RotHFlickerAmplitude = 3;

// Анимация рта - вертикальная дрожь
bool RotVFlicker = 0;
bool RotVFlickerAlternate = 0;
byte RotVFlickerAmplitude = 3;

// Анимация рта - замешательство
bool RotConfused = 0;
unsigned long RotConfusedAnimationTimer = 0;
int RotConfusedAnimationDuration = 500;
bool RotConfusedToggle = 1;

// Анимация рта - смех
bool RotLaugh = 0;
unsigned long RotLaughAnimationTimer = 0;
int RotLaughAnimationDuration = 500;
bool RotLaughToggle = 1;

// Новые анимации рта
bool RotSpeaking = 0;  // Режим "говорит" (неравномерное открытие-закрытие)
unsigned long RotSpeakingTimer = 0;
int RotSpeakingIntervalMin = 100;  // Мин. интервал для речи (мс)
int RotSpeakingIntervalMax = 300;  // Макс. интервал
int RotSpeakingHeightVariation = 8;  // Вариация высоты (как у человека)

int RotLaughLevel = 0;  // Уровень смеха: 0 - ухмылка, 1 - слабый, 2 - сильный
bool RotLaughing = 0;  // Флаг для анимации смеха
unsigned long RotLaughingTimer = 0;
int RotLaughingDuration = 1000;  // Длительность смеха

bool RotAngryBreathing = 0;  // Режим "злится/дышит" (минимальное приоткрытие)
unsigned long RotBreathingTimer = 0;
int RotBreathingInterval = 500;  // Интервал дыхания (мс)
int RotBreathingHeight = 4;  // Минимальная высота приоткрытия

bool RotGrimace = 0;  // Кривляние (случайные изменения формы/выражения)
unsigned long RotGrimaceTimer = 0;
int RotGrimaceInterval = 200;  // Интервал изменений

// Дополнительные анимации
bool RotYawn = 0;  // Зевок (широко открывается медленно)
unsigned long RotYawnTimer = 0;
int RotYawnDuration = 2000;

bool RotSigh = 0;  // Вздох (медленное открытие-закрытие с дрожью)
unsigned long RotSighTimer = 0;
int RotSighDuration = 1500;

//********************************************************************************************* 
//  Геометрия глаз (оригинальная, с добавлением для EMO-подобия)
//*********************************************************************************************

 // ЛЕВЫЙ ГЛАЗ - размер и радиус скругления
 int eyeLwidthDefault = 36;
 int eyeLheightDefault = 36;
 int eyeLwidthCurrent = eyeLwidthDefault;
 int eyeLheightCurrent = 1; // начинаем с закрытого глаза, иначе установить в eyeLheightDefault
 int eyeLwidthNext = eyeLwidthDefault;
 int eyeLheightNext = eyeLheightDefault;
 int eyeLheightOffset = 0;
 // Радиус скругления
 byte eyeLborderRadiusDefault = 18;  // Для круглого вида как у EMO (половина ширины)
 byte eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
 byte eyeLborderRadiusNext = eyeLborderRadiusDefault;

 // ПРАВЫЙ ГЛАЗ - размер и радиус скругления
 int eyeRwidthDefault = eyeLwidthDefault;
 int eyeRheightDefault = eyeLheightDefault;
 int eyeRwidthCurrent = eyeRwidthDefault;
 int eyeRheightCurrent = 1; // начинаем с закрытого глаза, иначе установить в eyeRheightDefault
 int eyeRwidthNext = eyeRwidthDefault;
 int eyeRheightNext = eyeRheightDefault;
 int eyeRheightOffset = 0;
 // Радиус скругления
 byte eyeRborderRadiusDefault = 18;
 byte eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
 byte eyeRborderRadiusNext = eyeRborderRadiusDefault;

 // ЛЕВЫЙ ГЛАЗ - координаты
 int eyeLxDefault = ((screenWidth)-(eyeLwidthDefault+spaceBetweenDefault+eyeRwidthDefault))/2;
 int eyeLyDefault = ((screenHeight-eyeLheightDefault)/2);
 int eyeLx = eyeLxDefault;
 int eyeLy = eyeLyDefault;
 int eyeLxNext = eyeLx;
 int eyeLyNext = eyeLy;

 // ПРАВЫЙ ГЛАЗ - координаты
 int eyeRxDefault = eyeLx+eyeLwidthCurrent+spaceBetweenDefault;
 int eyeRyDefault = eyeLy;
 int eyeRx = eyeRxDefault;
 int eyeRy = eyeRyDefault;
 int eyeRxNext = eyeRx;
 int eyeRyNext = eyeRy;

 // ОБА ГЛАЗА
 // Верхняя высота века
 byte eyelidsHeightMax = eyeLheightDefault/2; // макс. высота верхних век
 byte eyelidsTiredHeight = 0;
 byte eyelidsTiredHeightNext = eyelidsTiredHeight;
 byte eyelidsAngryHeight = 0;
 byte eyelidsAngryHeightNext = eyelidsAngryHeight;
 // Смещение нижнего века для happy
 byte eyelidsHappyBottomOffsetMax = (eyeLheightDefault/2)+3;
 byte eyelidsHappyBottomOffset = 0;
 byte eyelidsHappyBottomOffsetNext = 0;
 // Расстояние между глазами
 int spaceBetweenDefault = 10;
 int spaceBetweenCurrent = spaceBetweenDefault;
 int spaceBetweenNext = 10;


 //*********************************************************************************************
 //  Макро-анимации
 //*********************************************************************************************

 // Анимация - горизонтальная дрожь/вибрация
 bool hFlicker = 0;
 bool hFlickerAlternate = 0;
 byte hFlickerAmplitude = 2;

 // Анимация - вертикальная дрожь/вибрация
 bool vFlicker = 0;
 bool vFlickerAlternate = 0;
 byte vFlickerAmplitude = 10;

 // Анимация - автоподмигивание
 bool autoblinker = 0; // включить автоматическую анимацию моргания
 int blinkInterval = 1; // базовый интервал между морганиями в полных секундах
 int blinkIntervalVariation = 4; // диапазон вариации интервала в секундах; случайное число из этого диапазона добавится к базовому blinkInterval; установите 0 для без вариации
 unsigned long blinktimer = 0; // для организации тайминга моргания

 // Анимация - режим простоя: глаза смотрят в случайных направлениях
 bool idle = 0;
 int idleInterval = 1; // базовый интервал между перемещениями глаз в полных секундах
 int idleIntervalVariation = 3; // диапазон вариации интервала в секундах; случайное число из этого диапазона добавится к базовому idleInterval; установите 0 для без вариации
 unsigned long idleAnimationTimer = 0; // для организации тайминга анимации простоя

 // Анимация - глаза в замешательстве: тряска влево-вправо
 bool confused = 0;
 unsigned long confusedAnimationTimer = 0;
 int confusedAnimationDuration = 500;
 bool confusedToggle = 1;

 // Анимация - глаза смеются: тряска вверх-вниз
 bool laugh = 0;
 unsigned long laughAnimationTimer = 0;
 int laughAnimationDuration = 500;
 bool laughToggle = 1;

 // Анимация - пот на лбу
 bool sweat = 0;
 byte sweatBorderradius = 3;

 // Капля пота 1
 int sweat1XPosInitial = 2;
 int sweat1XPos;
 float sweat1YPos = 2;
 int sweat1YPosMax;
 float sweat1Height = 2;
 float sweat1Width = 1;

 // Капля пота 2
 int sweat2XPosInitial = 2;
 int sweat2XPos;
 float sweat2YPos = 2;
 int sweat2YPosMax;
 float sweat2Height = 2;
 float sweat2Width = 1;

 // Капля пота 3
 int sweat3XPosInitial = 2;
 int sweat3XPos;
 float sweat3YPos = 2;
 int sweat3YPosMax;
 float sweat3Height = 2;
 float sweat3Width = 1;


 //*********************************************************************************************
 //  ОБЩИЕ МЕТОДЫ
 //*********************************************************************************************

 RoboEyes(AdafruitDisplay &disp) : display(&disp) {};

// Запуск RoboEyes с заданной шириной, высотой экрана и макс. FPS
 void begin(int width, int height, byte frameRate) {
  screenWidth = width; // ширина OLED-дисплея в пикселях
  screenHeight = height; // высота OLED-дисплея в пикселях
  display->clearDisplay(); // очистить буфер дисплея
  display->display(); // показать пустой экран
  eyeLheightCurrent = 1; // начать с закрытых глаз
  eyeRheightCurrent = 1; // начать с закрытых глаз
  setFramerate(frameRate); // вычислить интервал кадров на основе заданного frameRate
  // Инициализация позиции рта
  RotXDefault = (screenWidth - RotWidthDefault) / 2;
  RotYDefault = screenHeight - RotHeightDefault - 5;
  RotX = RotXDefault;
  RotY = RotYDefault;
}

//-----------------------------------------------------------------------

 void update(){
  // Ограничиваем обновления рисования до заданной макс. частоты кадров
  if(millis()-fpsTimer >= frameInterval){
    drawEyes();
    fpsTimer = millis();
  }
 }


 //*********************************************************************************************
 //  SETTERS (УСТАНОВОЧНЫЕ МЕТОДЫ)
 //*********************************************************************************************

 // Вычислить интервал кадров по заданному fps
 void setFramerate(byte fps){
  frameInterval = 1000/fps;
 }

 // Установить цветовые значения
 void setDisplayColors(uint8_t background, uint8_t main) {
  BGCOLOR = background; // фон и оверлеи, выберите 0 для монохромных дисплеев и 0x00 для градационных дисплеев типа SSD1322
  MAINCOLOR = main; // рисунки, выберите 1 для монохромных и 0x0F для градационных дисплеев типа SSD1322 (0x0F = максимальная яркость)
 }

 void setWidth(byte leftEye, byte rightEye) {
  eyeLwidthNext = leftEye;
  eyeRwidthNext = rightEye;
  eyeLwidthDefault = leftEye;
  eyeRwidthDefault = rightEye;
  // Для круглого вида
  eyeLborderRadiusNext = leftEye / 2;
  eyeRborderRadiusNext = rightEye / 2;
 }

 void setHeight(byte leftEye, byte rightEye) {
  eyeLheightNext = leftEye;
  eyeRheightNext = rightEye;
  eyeLheightDefault = leftEye;
  eyeRheightDefault = rightEye;
 }

 // Установить радиус скругления для левого и правого глаз
 void setBorderradius(byte leftEye, byte rightEye) {
  eyeLborderRadiusNext = leftEye;
  eyeRborderRadiusNext = rightEye;
  eyeLborderRadiusDefault = leftEye;
  eyeRborderRadiusDefault = rightEye;
 }

 // Установить расстояние между глазами, может быть отрицательным
 void setSpacebetween(int space) {
  spaceBetweenNext = space;
  spaceBetweenDefault = space;
 }

 // Установить выражение настроения (расширенное)
 void setMood(unsigned char mood)
  {
    tired=0; angry=0; happy=0; sad=0; surprised=0;
    switch (mood)
    {
    case TIRED:
      tired=1; 
      break;
    case ANGRY:
      angry=1; 
      break;
    case HAPPY:
      happy=1; 
      break;
    case SAD:
      sad=1; 
      break;
    case SURPRISED:
      surprised=1; 
      break;
    default:
      break;
    }
  }

 // Установить предопределённую позицию
 void setPosition(unsigned char position)
  {
    switch (position)
    {
    case N:
      // Север, верхний центр
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = 0;
      break;
    case NE:
      // Северо-восток, верхний правый
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = 0;
      break;
    case E:
      // Восток, средний правый
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = getScreenConstraint_Y()/2;
      break;
    case SE:
      // Юго-восток, нижний правый
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = getScreenConstraint_Y();
      break;
    case S:
      // Юг, нижний центр
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = getScreenConstraint_Y();
      break;
    case SW:
      // Юго-запад, нижний левый
      eyeLxNext = 0;
      eyeLyNext = getScreenConstraint_Y();
      break;
    case W:
      // Запад, средний левый
      eyeLxNext = 0;
      eyeLyNext = getScreenConstraint_Y()/2;
      break;
    case NW:
      // Северо-запад, верхний левый
      eyeLxNext = 0;
      eyeLyNext = 0;
      break;
    default:
      // Центрально по середине
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = getScreenConstraint_Y()/2;
      break;
    }
  }

 // Включить автоматическое моргание, минимальный интервал в секундах и вариация интервала в секундах
 void setAutoblinker(bool active, int interval, int variation){
  autoblinker = active;
  blinkInterval = interval;
  blinkIntervalVariation = variation;
 }
 void setAutoblinker(bool active){
  autoblinker = active;
 }

 // Установить режим простоя - автоматическое переставление глаз, минимальный интервал в секундах и вариация интервала
 void setIdleMode(bool active, int interval, int variation){
  idle = active;
  idleInterval = interval;
  idleIntervalVariation = variation;
 }
 void setIdleMode(bool active) {
  idle = active;
 }

 // Установить режим любопытства - внешний глаз становится больше при взгляде влево/вправо
 void setCuriosity(bool curiousBit) {
  curious = curiousBit;
 }

 // Установить циклоп-режим - показывать только один глаз
 void setCyclops(bool cyclopsBit) {
  cyclops = cyclopsBit;
 }

 // Включить горизонтальную дрожь (смещение глаз влево/вправо)
 void setHFlicker (bool flickerBit, byte Amplitude) {
  hFlicker = flickerBit; // включить/выключить дрожь
  hFlickerAmplitude = Amplitude; // задать амплитуду дрожи в пикселях
 }
 void setHFlicker (bool flickerBit) {
  hFlicker = flickerBit; // включить/выключить дрожь
 }

 // Включить вертикальную дрожь (смещение глаз вверх/вниз)
 void setVFlicker (bool flickerBit, byte Amplitude) {
  vFlicker = flickerBit; // включить/выключить дрожь
  vFlickerAmplitude = Amplitude; // задать амплитуду дрожи в пикселях
 }
 void setVFlicker (bool flickerBit) {
  vFlicker = flickerBit; // включить/выключить дрожь
 }

 void setSweat (bool sweatBit) {
  sweat = sweatBit; // включить/выключить пот
 }
//********************************************************************************************* 
// SETTERS ДЛЯ РТА
//*********************************************************************************************
// Анимация зрачков (аналог setMood, но для направлений)
void setGlas(int leftX, int leftY, int rightX, int rightY, byte normSpeed = 1, byte fastSpeed = 10, bool enableSaccades = true) {
  glasTargetL_X = constrain(leftX, -12, 12);
  glasTargetL_Y = constrain(leftY, -8, 8);
  glasTargetR_X = constrain(rightX, -12, 12);
  glasTargetR_Y = constrain(rightY, -8, 8);
  glasNormSpeed = constrain(normSpeed, 1, 5);
  glasFastSpeed = constrain(fastSpeed, 5, 20);
  glasEnabled = true;
  glasSaccadesEnabled = enableSaccades;
  glasAnimTimer = millis();
  
  // Сбрасываем таймер саккад при смене направления
  saccadeTimer = millis() + random(saccadeDelayMin, saccadeDelayMax);
}

void setGlasEnabled(bool enabled) {
  glasEnabled = enabled;
  if (!enabled) {
    pupilL_XOffset = 0; pupilL_YOffset = 0;     // Сразу в центр (статично)
    pupilR_XOffset = 0; pupilR_YOffset = 0;
  }
}
// Плавная/резкая смена размера зрачков
void GlasSize(int startSize, int targetSize, int speedMs = 10, bool keepSaccades = true) {
  glasSizeStart = constrain(startSize, 0, 25);
  glasSizeTarget = constrain(targetSize, 0, 25);
  glasSizeCurrent = glasSizeStart;
  glasSizeSpeedMs = constrain(speedMs, 1, 50);  // 1 = очень быстро, 50 = медленно
  glasSizeEnabled = true;
  glasSizeTimer = millis();
  glasSaccadesEnabled = keepSaccades;  // Можно отключить саккады при смене размера
}

// Выключить эффект изменения размера (возврат к дефолту)
void GlasSizeOff() {
  glasSizeEnabled = false;
  pupilSize = 10;  // Возврат к обычному размеру (или можно оставить текущий)
}


// Включить/выключить рот
void RotSetEnabled(bool enabled) {
  RotEnabled = enabled;
    
}

// Установить ширину рта
void RotSetWidth(int width) {
  RotWidthNext = width;
  RotWidthDefault = width;
}

// Установить высоту рта
void RotSetHeight(int height) {
  RotHeightNext = height;
  RotHeightDefault = height;
}

// Установить радиус скругления рта
void RotSetBorderradius(byte radius) {
  RotBorderRadiusNext = radius;
  RotBorderRadiusDefault = radius;
}

// Установить позицию рта по X и Y
void RotSetPosition(int x, int y) {
  RotXNext = x;
  RotYNext = y;
}

// Установить настроение рта (расширенное)
void RotSetMood(unsigned char mood) {
  RotSmile = 0;
  RotSmileBad = 0;
  RotSmileLine = 0;
  switch (mood) {
    case TIRED:
      RotSmileBad = 1;
      break;
    case ANGRY:
      RotSmileBad = 1;
      break;
    case HAPPY:
      RotSmile = 1;
      break;
    case SAD:
      RotSmileBad = 1;
      break;
    case SURPRISED:
      RotSmile = 0;
      RotSmileBad = 0;
      RotSmileLine = 0;
      RotHeightNext = RotHeightDefault * 2;
      break;
    default:
      RotSmileLine = 1;
      break;
  }
}

// Автоматическое открывание/закрывание рта
void RotSetAutoblinker(bool active, int interval, int variation) {
  RotAutoblinker = active;
  RotBlinkInterval = interval;
  RotBlinkIntervalVariation = variation;
}

void RotSetAutoblinker(bool active) {
  RotAutoblinker = active;
}

// Режим простоя для рта
void RotSetIdleMode(bool active, int interval, int variation) {
  RotIdle = active;
  RotIdleInterval = interval;
  RotIdleIntervalVariation = variation;
}

void RotSetIdleMode(bool active) {
  RotIdle = active;
}

// Горизонтальная дрожь рта
void RotSetHFlicker(bool flickerBit, byte Amplitude) {
  RotHFlicker = flickerBit;
  RotHFlickerAmplitude = Amplitude;
}

void RotSetHFlicker(bool flickerBit) {
  RotHFlicker = flickerBit;
}

// Вертикальная дрожь рта
void RotSetVFlicker(bool flickerBit, byte Amplitude) {
  RotVFlicker = flickerBit;
  RotVFlickerAmplitude = Amplitude;
}

void RotSetVFlicker(bool flickerBit) {
  RotVFlicker = flickerBit;
}

// Установить любопытный режим для рта
void RotSetCuriosity(bool curiousBit) {
  RotCurious = curiousBit;
}

// Новые setters для рта
void RotSetSpeaking(bool active, int minInterval = 100, int maxInterval = 300, int heightVar = 8) {
  RotSpeaking = active;
  RotSpeakingIntervalMin = minInterval;
  RotSpeakingIntervalMax = maxInterval;
  RotSpeakingHeightVariation = heightVar;
  if (active) RotSpeakingTimer = millis();
}

void RotSetLaughing(int level, int duration = 1000) {
  RotLaughLevel = level;
  RotLaughing = true;
  RotLaughingDuration = duration;
  RotLaughingTimer = millis();
}

void RotSetAngryBreathing(bool active, int interval = 500, int height = 4) {
  RotAngryBreathing = active;
  RotBreathingInterval = interval;
  RotBreathingHeight = height;
  if (active) RotBreathingTimer = millis();
}

void RotSetYawn(bool active, int duration = 2000) {
  RotYawn = active;
  RotYawnDuration = duration;
  RotYawnTimer = millis();
}

void RotSetSigh(bool active, int duration = 1500) {
  RotSigh = active;
  RotSighDuration = duration;
  RotSighTimer = millis();
}

void RotSetGrimace(bool active, int interval = 200) {
  RotGrimace = active;
  RotGrimaceInterval = interval;
  if (active) RotGrimaceTimer = millis();
}

// Новые setters для глаз (EMO-подобные)
void setRoundEyes(bool enable) {
  roundEyes = enable;
}

void setPupilOffset(int leftX, int leftY, int rightX, int rightY) {
  pupilL_XOffset = leftX;
  pupilL_YOffset = leftY;
  pupilR_XOffset = rightX;
  pupilR_YOffset = rightY;
}

void setPupilSize(byte size) {
  pupilSize = size;
}

void setPupilsEnabled(bool enable) {
  pupilsEnabled = enable;
}

 


 //*********************************************************************************************
 //  GETTERS (МЕТОДЫ-ПОЛУЧАТЕЛИ)
 //*********************************************************************************************

 // Возвращает макс. x-позицию для левого глаза
 int getScreenConstraint_X(){
  return screenWidth-eyeLwidthCurrent-spaceBetweenCurrent-eyeRwidthCurrent;
 } 

 // Возвращает макс. y-позицию для левого глаза
 int getScreenConstraint_Y(){
  return screenHeight-eyeLheightDefault; // используем высоту по умолчанию, т.к. высота изменяется при моргании и в curious режиме
 }
// Getters для рта
int RotGetScreenConstraint_X(){
  return screenWidth - RotWidthCurrent;
}

int RotGetScreenConstraint_Y(){
  return screenHeight - RotHeightDefault;
}

 //*********************************************************************************************
 //  БАЗОВЫЕ МЕТОДЫ АНИМАЦИИ
 //*********************************************************************************************

 // МОРГАНИЕ ДЛЯ ОБОИХ ГЛАЗ ОДНОВРЕМЕННО
 // Закрыть оба глаза
 void close() {
  eyeLheightNext = 1; // закрытие левого глаза
  eyeRheightNext = 1; // закрытие правого глаза
  eyeL_open = 0; // левый глаз закрыт
  eyeR_open = 0; // правый глаз закрыт
 }

 // Открыть оба глаза
 void open() {
  eyeL_open = 1; // левый глаз открыт - drawEyes() позаботится об открытии
  eyeR_open = 1; // правый глаз открыт
 }

 // Запустить анимацию моргания
 void blink() {
  close();
  open();
 }

 // МОРГАНИЕ ДЛЯ ОТДЕЛЬНЫХ ГЛАЗ, КОНТРОЛЬ КАЖДОГО ГЛАЗА ОТДЕЛЬНО
 // Закрыть глаз(а)
 void close(bool left, bool right) {
  if(left){
    eyeLheightNext = 1; // моргание левого глаза
    eyeL_open = 0; // левый глаз закрыт
  }
  if(right){
      eyeRheightNext = 1; // моргание правого глаза
      eyeR_open = 0; // правый глаз закрыт
  }
 }

 // Открыть глаз(а)
 void open(bool left, bool right) {
  if(left){
    eyeL_open = 1; // левый глаз открыт - drawEyes() займётся открытием
  }
  if(right){
    eyeR_open = 1; // правый глаз открыт
  }
 }

 // Запустить анимацию моргания для выбранных глаз
 void blink(bool left, bool right) {
  close(left, right);
  open(left, right);
 }

  // Анимация зрачков (Glas — как "глаз", аналог mood)
  bool glasEnabled = false;              // Включить анимацию зрачков
  int glasTargetL_X = 0;                 // Цель для левого зрачка X
  int glasTargetL_Y = 0;                 // Цель для левого зрачка Y
  int glasTargetR_X = 0;                 // Цель для правого зрачка X
  int glasTargetR_Y = 0;                 // Цель для правого зрачка Y
  byte glasNormSpeed = 1;                // Нормальная скорость (медленно, 1–5)
  byte glasFastSpeed = 10;               // Быстрая скорость (резко, 5–20)
  unsigned long glasAnimTimer = 0;       // Таймер для плавности
    // Живые зрачки — саккады (быстрые случайные движения)
    bool glasSaccadesEnabled = true;        // Включить саккады (живость)
    unsigned long saccadeTimer = 0;         // Таймер следующего дёргания
    int saccadeDelayMin = 800;              // Мин. пауза между саккадами (мс)
    int saccadeDelayMax = 3000;             // Макс. пауза (как у человека)
    int saccadeAmplitude = 8;               // Насколько сильно дёргаются (пикселей)
    byte saccadeSpeed = 20;                 // Скорость дёргания (чем больше — резче)
    // Анимация размера зрачков (GlasSize — плавно/резко меняем размер)
    bool glasSizeEnabled = false;           // Включена ли анимация размера
    int glasSizeStart = 10;                 // Начальный размер
    int glasSizeTarget = 10;                // Целевой размер
    int glasSizeCurrent = 10;               // Текущий размер (интерполяция)
    unsigned long glasSizeTimer = 0;        // Таймер анимации
    int glasSizeSpeedMs = 10;               // Скорость в мс на шаг (меньше = быстрее)

 //*********************************************************************************************
 //  МАКРО-АНИМАЦИИ
 //*********************************************************************************************

 // Запустить анимацию "confused" - одноразовая анимация дрожания глаз влево-вправо
 void anim_confused() {
  confused = 1;
 }

 // Запустить анимацию "laugh" - одноразовая анимация дрожания глаз вверх-вниз
 void anim_laugh() {
  laugh = 1;
 }
//********************************************************************************************* 
// БАЗОВЫЕ АНИМАЦИИ РТА
//*********************************************************************************************

// Закрыть рот
void RotClose() {
  RotHeightNext = 1;
  RotOpen = 0;
}

// Открыть рот
void RotOpenMouth() {
  RotOpen = 1;
  RotHeightNext = RotHeightDefault;
}

// Моргнуть ртом (открыть-закрыть)
void RotBlink() {
  RotClose();
  RotOpenMouth();
}

//********************************************************************************************* 
// МАКРО-АНИМАЦИИ РТА
//*********************************************************************************************

// Анимация замешательства для рта
void RotAnim_confused() {
  RotConfused = 1;
}

// Анимация смеха для рта
void RotAnim_laugh() {
  RotLaugh = 1;
}
 //*********************************************************************************************
 //  ПРЕДВЫЧИСЛЕНИЯ И СОБСТВЕННОЕ РИСОВАНИЕ
 //*********************************************************************************************

 void drawEyes(){

  //// ПРЕДВЫЧИСЛЕНИЯ - РАЗМЕРЫ ГЛАЗ И ЗНАЧЕНИЯ ДЛЯ ПЕРЕХОДОВ АНИМАЦИИ //// 

  // Вертикальное смещение для увеличения глаз при взгляде влево/вправо (curious gaze)
  if(curious){
    if(eyeLxNext<=10){eyeLheightOffset=8;}
    else if (eyeLxNext>=(getScreenConstraint_X()-10) && cyclops){eyeLheightOffset=8;}
    else{eyeLheightOffset=0;} // левый глаз
    if(eyeRxNext>=screenWidth-eyeRwidthCurrent-10){eyeRheightOffset=8;}
    else{eyeRheightOffset=0;} // правый глаз
  } else {
    eyeLheightOffset=0; // сброс смещения высоты для левого глаза
    eyeRheightOffset=0; // сброс смещения высоты для правого глаза
  }

  // Высота левого глаза
  eyeLheightCurrent = (eyeLheightCurrent + eyeLheightNext + eyeLheightOffset)/2;
  eyeLy+= ((eyeLheightDefault-eyeLheightCurrent)/2); // вертикальное центрирование глаза при закрытии
  eyeLy-= eyeLheightOffset/2;
  // Высота правого глаза
  eyeRheightCurrent = (eyeRheightCurrent + eyeRheightNext + eyeRheightOffset)/2;
  eyeRy+= (eyeRheightDefault-eyeRheightCurrent)/2; // вертикальное центрирование глаза при закрытии
  eyeRy-= eyeRheightOffset/2;


  // Открывать глаза снова после их закрытия
  if(eyeL_open){
    if(eyeLheightCurrent <= 1 + eyeLheightOffset){eyeLheightNext = eyeLheightDefault;} 
  }
  if(eyeR_open){
    if(eyeRheightCurrent <= 1 + eyeRheightOffset){eyeRheightNext = eyeRheightDefault;} 
  }

  // Ширина левого глаза
  eyeLwidthCurrent = (eyeLwidthCurrent + eyeLwidthNext)/2;
  // Ширина правого глаза
  eyeRwidthCurrent = (eyeRwidthCurrent + eyeRwidthNext)/2;


  // Расстояние между глазами
  spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext)/2;

  // Координаты левого глаза
  eyeLx = (eyeLx + eyeLxNext)/2;
  eyeLy = (eyeLy + eyeLyNext)/2;
  // Координаты правого глаза
  eyeRxNext = eyeLxNext+eyeLwidthCurrent+spaceBetweenCurrent; // x правого глаза зависит от позиции левого + расстояния
  eyeRyNext = eyeLyNext; // y правого глаза такой же, как у левого
  eyeRx = (eyeRx + eyeRxNext)/2;
  eyeRy = (eyeRy + eyeRyNext)/2;

  // Радиус скругления левого глаза
  eyeLborderRadiusCurrent = (eyeLborderRadiusCurrent + eyeLborderRadiusNext)/2;
  // Радиус скругления правого глаза
  eyeRborderRadiusCurrent = (eyeRborderRadiusCurrent + eyeRborderRadiusNext)/2;
  

  //// ПРИМЕНЕНИЕ МАКРО-АНИМАЦИЙ ////

  if(autoblinker){
    if(millis() >= blinktimer){
    blink();
    blinktimer = millis()+(blinkInterval*1000)+(random(blinkIntervalVariation)*1000); // вычислить следующее время моргания
    }
  }

  // Смеющийся режим - тряска глаз вверх/вниз в течение laughAnimationDuration (по умолчанию = 500ms)
  if(laugh){
    if(laughToggle){
      setVFlicker(1, 5);
      laughAnimationTimer = millis();
      laughToggle = 0;
    } else if(millis() >= laughAnimationTimer+laughAnimationDuration){
      setVFlicker(0, 0);
      laughToggle = 1;
      laugh=0; 
    }
  }

  // Confused - тряска глаз влево-вправо в течение confusedAnimationDuration (по умолчанию = 500ms)
  if(confused){
    if(confusedToggle){
      setHFlicker(1, 20);
      confusedAnimationTimer = millis();
      confusedToggle = 0;
    } else if(millis() >= confusedAnimationTimer+confusedAnimationDuration){
      setHFlicker(0, 0);
      confusedToggle = 1;
      confused=0; 
    }
  }

  // Idle - глаза двигаются в случайные позиции на экране
  if(idle){
    if(millis() >= idleAnimationTimer){
      eyeLxNext = random(getScreenConstraint_X());
      eyeLyNext = random(getScreenConstraint_Y());
      idleAnimationTimer = millis()+(idleInterval*1000)+(random(idleIntervalVariation)*1000); // вычислить следующее время перемещения глаз
    }
  }

  // Добавляем смещение для горизонтальной дрожи/вибрации
  if(hFlicker){
    if(hFlickerAlternate) {
      eyeLx += hFlickerAmplitude;
      eyeRx += hFlickerAmplitude;
    } else {
      eyeLx -= hFlickerAmplitude;
      eyeRx -= hFlickerAmplitude;
    }
    hFlickerAlternate = !hFlickerAlternate;
  }

  // Добавляем смещение для вертикальной дрожи/вибрации
  if(vFlicker){
    if(vFlickerAlternate) {
      eyeLy += vFlickerAmplitude;
      eyeRy += vFlickerAmplitude;
    } else {
      eyeLy -= vFlickerAmplitude;
      eyeRy -= vFlickerAmplitude;
    }
    vFlickerAlternate = !vFlickerAlternate;
  }
  // Анимация зрачков (плавное движение к цели)
   // === ЖИВЫЕ ЗРАЧКИ: основное направление + саккады (как у человека) ===
   // === ЖИВЫЕ ЗРАЧКИ: направление + саккады + плавная смена размера ===
  if(glasEnabled || glasSizeEnabled) {
    if(millis() - glasAnimTimer > 40) {
      int speed = glasNormSpeed;

      // === Саккады (быстрые дёргания) ===
      if(glasSaccadesEnabled && millis() > saccadeTimer) {
        speed = saccadeSpeed;
        pupilL_XOffset += random(-saccadeAmplitude, saccadeAmplitude+1);
        pupilL_YOffset += random(-saccadeAmplitude/2, saccadeAmplitude/2+1);
        pupilR_XOffset += random(-saccadeAmplitude, saccadeAmplitude+1);
        pupilR_YOffset += random(-saccadeAmplitude/2, saccadeAmplitude/2+1);
        saccadeTimer = millis() + random(saccadeDelayMin, saccadeDelayMax);
      }

      // === Плавное движение к цели (направление взгляда) ===
      pupilL_XOffset += (glasTargetL_X - pupilL_XOffset) / speed;
      pupilL_YOffset += (glasTargetL_Y - pupilL_YOffset) / speed;
      pupilR_XOffset += (glasTargetR_X - pupilR_XOffset) / speed;
      pupilR_YOffset += (glasTargetR_Y - pupilR_YOffset) / speed;

      glasAnimTimer = millis();
    }

    // === Анимация размера зрачков ===
    if(glasSizeEnabled && millis() - glasSizeTimer > glasSizeSpeedMs) {
      if(glasSizeCurrent < glasSizeTarget) {
        glasSizeCurrent++;
      } else if(glasSizeCurrent > glasSizeTarget) {
        glasSizeCurrent--;
      } else {
        glasSizeEnabled = false;  // Анимация завершена
      }
      pupilSize = glasSizeCurrent;
      glasSizeTimer = millis();
    }
  }
  // Циклоп-режим, установить размер второго глаза и расстояние между ними в 0
  if(cyclops){
    eyeRwidthCurrent = 0;
    eyeRheightCurrent = 0;
    spaceBetweenCurrent = 0;
  }
  //// ПРЕДВЫЧИСЛЕНИЯ РТА

if(RotEnabled) {
  // Плавное изменение размеров рта
  RotWidthCurrent = (RotWidthCurrent + RotWidthNext)/2;
  RotHeightCurrent = (RotHeightCurrent + RotHeightNext)/2;
  RotBorderRadiusCurrent = (RotBorderRadiusCurrent + RotBorderRadiusNext)/2;

  // Плавное изменение позиции рта
  RotX = (RotX + RotXNext)/2;
  RotY = (RotY + RotYNext)/2;

  // Открывание рта
  if(RotOpen) {
    if(RotHeightCurrent <= 1) {
      RotHeightNext = RotHeightDefault;
    }
  }

  // Подготовка выражений рта
  if(RotSmile) {
    RotLipsBottomOffsetNext = RotHeightCurrent/2;
    RotLipsTopHeightNext = 0;
  } else if(RotSmileBad) {
    RotLipsTopHeightNext = RotHeightCurrent/2;
    RotLipsBottomOffsetNext = 0;
  } else {
    RotLipsTopHeightNext = 0;
    RotLipsBottomOffsetNext = 0;
  }

  RotLipsTopHeight = (RotLipsTopHeight + RotLipsTopHeightNext)/2;
  RotLipsBottomOffset = (RotLipsBottomOffset + RotLipsBottomOffsetNext)/2;

  // Новая анимация: Говорит (неравномерное открытие-закрытие, беседа)
  if(RotSpeaking) {
    if(millis() >= RotSpeakingTimer) {
      RotOpen = !RotOpen;
      if(RotOpen) {
        RotHeightNext = RotHeightDefault + random(-RotSpeakingHeightVariation, RotSpeakingHeightVariation);
      } else {
        RotHeightNext = 1;
      }
      RotSpeakingTimer = millis() + random(RotSpeakingIntervalMin, RotSpeakingIntervalMax);
    }
  }

  // Новая: Смех (сильно/слабо/ухмылка)
  if(RotLaughing) {
    if(millis() - RotLaughingTimer < RotLaughingDuration) {
      RotSmile = 1;  // Улыбка
      int amp = (RotLaughLevel == LAUGH_STRONG) ? 10 : (RotLaughLevel == LAUGH_WEAK) ? 5 : 2;
      RotVFlicker = 1;
      RotVFlickerAmplitude = amp;  // Дрожь губ
      RotHeightNext = (RotLaughLevel == SMIRK) ? RotHeightDefault / 2 : RotHeightDefault + amp / 2;
    } else {
      RotLaughing = 0;
      RotVFlicker = 0;
      RotHeightNext = 1;
    }
  }

  // Новая: Злится/дышит
  if(RotAngryBreathing) {
    if(millis() >= RotBreathingTimer) {
      RotOpen = !RotOpen;
      RotHeightNext = RotOpen ? RotBreathingHeight : 1;
      RotBreathingTimer = millis() + RotBreathingInterval + random(-100, 100);  // Вариация как у человека
    }
  }

  // Дополнительная: Зевок
  if(RotYawn) {
    if(millis() - RotYawnTimer < RotYawnDuration) {
      RotHeightNext = map(millis() - RotYawnTimer, 0, RotYawnDuration, 1, RotHeightDefault * 2);
    } else {
      RotYawn = 0;
      RotHeightNext = 1;
    }
  }

  // Дополнительная: Вздох
  if(RotSigh) {
    if(millis() - RotSighTimer < RotSighDuration) {
      if(millis() - RotSighTimer < RotSighDuration / 2) {
        RotHeightNext = map(millis() - RotSighTimer, 0, RotSighDuration / 2, 1, RotHeightDefault);
      } else {
        RotHeightNext = map(millis() - RotSighTimer, RotSighDuration / 2, RotSighDuration, RotHeightDefault, 1);
      }
      RotVFlicker = 1;
      RotVFlickerAmplitude = 2;
    } else {
      RotSigh = 0;
      RotVFlicker = 0;
      RotHeightNext = 1;
    }
  }

  // Новая: Кривляние (случайные изменения выражения, формы)
  if(RotGrimace) {
    if(millis() >= RotGrimaceTimer) {
      int randExpr = random(0, 3);
      if(randExpr == 0) RotSmile = 1;
      else if(randExpr == 1) RotSmileBad = 1;
      else RotSmileLine = 1;
      RotHeightNext = random(1, RotHeightDefault + 5);
      RotWidthNext = random(RotWidthDefault - 10, RotWidthDefault + 10);
      RotHFlicker = 1;
      RotHFlickerAmplitude = random(1, 5);
      RotGrimaceTimer = millis() + RotGrimaceInterval;
    }
  }

  // Старые анимации рта
  if(RotAutoblinker) {
    if(millis() >= RotBlinktimer) {
      RotBlink();
      RotBlinktimer = millis() + (RotBlinkInterval*1000) + (random(RotBlinkIntervalVariation)*1000);
    }
  }

  if(RotLaugh) {
    if(RotLaughToggle) {
      RotSetVFlicker(1, 5);
      RotLaughAnimationTimer = millis();
      RotLaughToggle = 0;
    } else if(millis() >= RotLaughAnimationTimer + RotLaughAnimationDuration) {
      RotSetVFlicker(0, 0);
      RotLaughToggle = 1;
      RotLaugh = 0;
    }
  }

  if(RotConfused) {
    if(RotConfusedToggle) {
      RotSetHFlicker(1, 15);
      RotConfusedAnimationTimer = millis();
      RotConfusedToggle = 0;
    } else if(millis() >= RotConfusedAnimationTimer + RotConfusedAnimationDuration) {
      RotSetHFlicker(0, 0);
      RotConfusedToggle = 1;
      RotConfused = 0;
    }
  }

  if(RotIdle) {
    if(millis() >= RotIdleAnimationTimer) {
      RotXNext = random(RotGetScreenConstraint_X());
      RotIdleAnimationTimer = millis() + (RotIdleInterval*1000) + (random(RotIdleIntervalVariation)*1000);
    }
  }

  if(RotHFlicker) {
    if(RotHFlickerAlternate) {
      RotX += RotHFlickerAmplitude;
    } else {
      RotX -= RotHFlickerAmplitude;
    }
    RotHFlickerAlternate = !RotHFlickerAlternate;
  }

  if(RotVFlicker) {
    if(RotVFlickerAlternate) {
      RotY += RotVFlickerAmplitude;
    } else {
      RotY -= RotVFlickerAmplitude;
    }
    RotVFlickerAlternate = !RotVFlickerAlternate;
  }
}
  //// СОБСТВЕННОЕ РИСОВАНИЕ ////

  display->clearDisplay(); // начать с пустого экрана

  // Рисуем базовые прямоугольники глаз (с большим радиусом для круглого вида)
  display->fillRoundRect(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, eyeLborderRadiusCurrent, MAINCOLOR); // левый глаз
  if (!cyclops){
    display->fillRoundRect(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, eyeRborderRadiusCurrent, MAINCOLOR); // правый глаз
  }

  // Рисуем зрачки (как у EMO)
  if(pupilsEnabled) {
    int pupilL_X = eyeLx + eyeLwidthCurrent / 2 + pupilL_XOffset;
    int pupilL_Y = eyeLy + eyeLheightCurrent / 2 + pupilL_YOffset;
    display->fillCircle(pupilL_X, pupilL_Y, pupilSize, BGCOLOR); // Зрачок левый
    if (!cyclops) {
      int pupilR_X = eyeRx + eyeRwidthCurrent / 2 + pupilR_XOffset;
      int pupilR_Y = eyeRy + eyeRheightCurrent / 2 + pupilR_YOffset;
      display->fillCircle(pupilR_X, pupilR_Y, pupilSize, BGCOLOR); // Правый
    }
  }

  // Подготовка переходов настроений
  if (tired){eyelidsTiredHeightNext = eyeLheightCurrent/2; eyelidsAngryHeightNext = 0;} else{eyelidsTiredHeightNext = 0;}
  if (angry){eyelidsAngryHeightNext = eyeLheightCurrent/2; eyelidsTiredHeightNext = 0;} else{eyelidsAngryHeightNext = 0;}
  if (happy){eyelidsHappyBottomOffsetNext = eyeLheightCurrent/2;} else{eyelidsHappyBottomOffsetNext = 0;}
  if (sad){eyelidsTiredHeightNext = eyeLheightCurrent/3;} // Для грусти
  if (surprised){eyeLheightNext = eyeLheightDefault + 8; eyeLwidthNext = eyeLwidthDefault + 8;} // Увеличение для удивления

  // Рисуем уставшие верхние веки 
    eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext)/2;
    if (!cyclops){
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // левый глаз 
      display->fillTriangle(eyeRx, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy+eyelidsTiredHeight-1, BGCOLOR); // правый глаз
    } else {
      // Циклоп — уставшие веки по половинам
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // левая половина века
      display->fillTriangle(eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // правая половина века
    }

  // Рисуем злые верхние веки 
    eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext)/2;
    if (!cyclops){ 
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy+eyelidsAngryHeight-1, BGCOLOR); // левый глаз
      display->fillTriangle(eyeRx, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy-1, eyeRx, eyeRy+eyelidsAngryHeight-1, BGCOLOR); // правый глаз
    } else {
      // Циклоп — злые веки по половинам
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy+eyelidsAngryHeight-1, BGCOLOR); // левая половина
      display->fillTriangle(eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy+eyelidsAngryHeight-1, BGCOLOR); // правая половина
    }

  // Рисуем нижние happy-веки
    eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext)/2;
    display->fillRoundRect(eyeLx-1, (eyeLy+eyeLheightCurrent)-eyelidsHappyBottomOffset+1, eyeLwidthCurrent+2, eyeLheightDefault, eyeLborderRadiusCurrent, BGCOLOR); // левый глаз
    if (!cyclops){ 
      display->fillRoundRect(eyeRx-1, (eyeRy+eyeRheightCurrent)-eyelidsHappyBottomOffset+1, eyeRwidthCurrent+2, eyeRheightDefault, eyeRborderRadiusCurrent, BGCOLOR); // правый глаз
    }

  // Добавляем капли пота
    if (sweat){
      // ... (старый код для пота)
    }

    //// РИСОВАНИЕ РТА
if(RotEnabled) {
  // Синхронизация по центру между глазами
  RotX = (eyeLx + eyeRx + eyeLwidthCurrent) / 2 - (RotWidthCurrent / 2);
  RotY = eyeLy + max(eyeLheightCurrent, eyeRheightCurrent) + 8;

  // Рисуем базовый прямоугольник рта
  display->fillRoundRect(RotX, RotY, RotWidthCurrent, RotHeightCurrent, RotBorderRadiusCurrent, MAINCOLOR);

  // Рисуем улыбку (happy - нижняя губа поднята)
  if(RotSmile) {
    display->fillRoundRect(RotX-1, (RotY+RotHeightCurrent)-RotLipsBottomOffset+1, 
                          RotWidthCurrent+2, RotHeightDefault, 
                          RotBorderRadiusCurrent, BGCOLOR);
  }

  // Рисуем грустный рот (верхняя губа опущена)
  if(RotSmileBad) {
    display->fillTriangle(RotX, RotY-1, 
                         RotX+RotWidthCurrent, RotY-1, 
                         RotX+(RotWidthCurrent/2), RotY+RotLipsTopHeight-1, 
                         BGCOLOR);
  }
}

  display->display(); // отобразить рисунки на дисплее

 } // конец метода drawEyes


}; // конец класса RoboEyes


#endif