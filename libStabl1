/*
 * FluxGarage RoboEyes for GC9A01A Round Display V 2.0.0
*/

#ifndef _FLUXGARAGE_ROBOEYES_GC9A01A_H
#define _FLUXGARAGE_ROBOEYES_GC9A01A_H

// Цвета дисплея по умолчанию (RGB565)
uint16_t BGCOLOR_GC = 0x0000;      // фон - черный
uint16_t MAINCOLOR_GC = 0xFFFF;    // белок глаза - белый
uint16_t PUPILCOLOR_GC = 0x0000;   // зрачок - черный

// Для переключателя типа настроения
#define DEFAULT 0
#define TIRED 1
#define ANGRY 2
#define HAPPY 3
#define SURPRISED 4
#define SAD 5

// Для включения или отключения
#define ON 1
#define OFF 0

// Для переключателя "предопределённых позиций"
#define N 1   // север, верхний центр
#define NE 2  // северо-восток, верхний правый
#define E 3   // восток, средний правый
#define SE 4  // юго-восток, нижний правый
#define S 5   // юг, нижний центр
#define SW 6  // юго-запад, нижний левый
#define W 7   // запад, средний левый
#define NW 8  // северо-запад, верхний левый

// Вспомогательная функция для конвертации RGB в RGB565
inline uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Конструктор: принимает ссылку на Adafruit_GC9A01A
template<typename AdafruitDisplay>
class RoboEyes
{
private:
  // Приватные вспомогательные функции
  
  // Проверка, находится ли точка в круге (для круглого дисплея)
  bool isInsideCircle(int x, int y, int cx, int cy, int radius) {
    int dx = x - cx;
    int dy = y - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
  }
  
  // Ограничение координат в пределах радиуса от центра
  void constrainToRadius(int &x, int &y, int eyeWidth, int eyeHeight) {
    if (!radialConstraintEnabled) return;
    
    // Центр дисплея
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    
    // Проверяем, не выходят ли границы глаза за радиус
    int eyeCenterX = x + eyeWidth / 2;
    int eyeCenterY = y + eyeHeight / 2;
    
    // Расстояние от центра дисплея до центра глаза
    float dx = eyeCenterX - centerX;
    float dy = eyeCenterY - centerY;
    float distance = sqrt(dx * dx + dy * dy);
    
    // Максимальный радиус с учетом размера глаза
    float maxRadius = radialConstraintRadius - max(eyeWidth, eyeHeight) / 2;
    
    // Если глаз выходит за границы - корректируем
    if (distance > maxRadius) {
      float ratio = maxRadius / distance;
      eyeCenterX = centerX + dx * ratio;
      eyeCenterY = centerY + dy * ratio;
      
      x = eyeCenterX - eyeWidth / 2;
      y = eyeCenterY - eyeHeight / 2;
    }
  }

public:

  // Ссылка на объект дисплея Adafruit
  AdafruitDisplay *display;

  // Для общей настройки - размер экрана и макс. частота кадров
  int screenWidth = 240;     // ширина дисплея GC9A01A
  int screenHeight = 240;    // высота дисплея GC9A01A
  int frameInterval = 20;    // 50 FPS по умолчанию
  unsigned long fpsTimer = 0;

  // Центр дисплея для радиального ограничения
  int displayCenterX = 120;
  int displayCenterY = 120;
  bool radialConstraintEnabled = false;
  int radialConstraintRadius = 100;

  // Цвета для каждого глаза и зрачка отдельно
  uint16_t eyeLeftColor = 0xFFFF;      // белый для левого белка
  uint16_t eyeRightColor = 0xFFFF;     // белый для правого белка
  uint16_t pupilLeftColor = 0x0000;    // черный для левого зрачка
  uint16_t pupilRightColor = 0x0000;   // черный для правого зрачка
  uint16_t backgroundColor = 0x0000;   // черный фон

  // Для управления типами настроения и выражениями
  bool tired = 0;
  bool angry = 0;
  bool happy = 0;
  bool surprised = 0;
  bool sad = 0;
  bool curious = 0;
  bool cyclops = 0;
  bool eyeL_open = 0;
  bool eyeR_open = 0;

  //*********************************************************************************************
  //  Геометрия глаз
  //*********************************************************************************************

  // ЛЕВЫЙ ГЛАЗ - размер и радиус скругления
  int eyeLwidthDefault = 50;
  int eyeLheightDefault = 50;
  int eyeLwidthCurrent = eyeLwidthDefault;
  int eyeLheightCurrent = 1;
  int eyeLwidthNext = eyeLwidthDefault;
  int eyeLheightNext = eyeLheightDefault;
  int eyeLheightOffset = 0;
  byte eyeLborderRadiusDefault = 12;
  byte eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
  byte eyeLborderRadiusNext = eyeLborderRadiusDefault;

  // ПРАВЫЙ ГЛАЗ - размер и радиус скругления
  int eyeRwidthDefault = eyeLwidthDefault;
  int eyeRheightDefault = eyeLheightDefault;
  int eyeRwidthCurrent = eyeRwidthDefault;
  int eyeRheightCurrent = 1;
  int eyeRwidthNext = eyeRwidthDefault;
  int eyeRheightNext = eyeRheightDefault;
  int eyeRheightOffset = 0;
  byte eyeRborderRadiusDefault = 12;
  byte eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
  byte eyeRborderRadiusNext = eyeRborderRadiusDefault;

  // ЗРАЧКИ - размер и положение
  int pupilSizeDefault = 20;
  int pupilSizeCurrent = pupilSizeDefault;
  int pupilSizeNext = pupilSizeDefault;
  
  // Смещение зрачков относительно центра глаза
  int pupilLoffsetX = 0;
  int pupilLoffsetY = 0;
  int pupilRoffsetX = 0;
  int pupilRoffsetY = 0;
  int pupilLoffsetXNext = 0;
  int pupilLoffsetYNext = 0;
  int pupilRoffsetXNext = 0;
  int pupilRoffsetYNext = 0;

  // ЛЕВЫЙ ГЛАЗ - координаты
  int spaceBetweenDefault = 20;
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
  byte eyelidsHeightMax = eyeLheightDefault/2;
  byte eyelidsTiredHeight = 0;
  byte eyelidsTiredHeightNext = eyelidsTiredHeight;
  byte eyelidsAngryHeight = 0;
  byte eyelidsAngryHeightNext = eyelidsAngryHeight;
  byte eyelidsHappyBottomOffsetMax = (eyeLheightDefault/2)+3;
  byte eyelidsHappyBottomOffset = 0;
  byte eyelidsHappyBottomOffsetNext = 0;
  byte eyelidsSurprisedTopOffset = 0;
  byte eyelidsSurprisedTopOffsetNext = 0;
  int spaceBetweenCurrent = spaceBetweenDefault;
  int spaceBetweenNext = 20;

  // --- предыдущие области для очистки ---*
  int prev_eyeLx = -1, prev_eyeLy = -1;
  int prev_eyeLw = 0,  prev_eyeLh = 0;
  int prev_eyeRx = -1, prev_eyeRy = -1;
  int prev_eyeRw = 0,  prev_eyeRh = 0;


  //*********************************************************************************************
  //  Макро-анимации
  //*********************************************************************************************

  // Горизонтальная дрожь
  bool hFlicker = 0;
  bool hFlickerAlternate = 0;
  byte hFlickerAmplitude = 2;

  // Вертикальная дрожь
  bool vFlicker = 0;
  bool vFlickerAlternate = 0;
  byte vFlickerAmplitude = 10;

  // Автоматическое моргание
  bool autoblinker = 0;
  int blinkInterval = 1;
  int blinkIntervalVariation = 4;
  unsigned long blinktimer = 0;

  // Режим простоя
  bool idle = 0;
  int idleInterval = 1;
  int idleIntervalVariation = 3;
  unsigned long idleAnimationTimer = 0;

  // Анимация "confused"
  bool confused = 0;
  unsigned long confusedAnimationTimer = 0;
  int confusedAnimationDuration = 500;
  bool confusedToggle = 1;

  // Анимация "laugh"
  bool laugh = 0;
  unsigned long laughAnimationTimer = 0;
  int laughAnimationDuration = 500;
  bool laughToggle = 1;

  // Анимация "sweat"
  bool sweat = 0;
  byte sweatBorderradius = 3;

  // Капли пота
  int sweat1XPosInitial = 2;
  int sweat1XPos;
  float sweat1YPos = 2;
  int sweat1YPosMax;
  float sweat1Height = 2;
  float sweat1Width = 1;

  int sweat2XPosInitial = 2;
  int sweat2XPos;
  float sweat2YPos = 2;
  int sweat2YPosMax;
  float sweat2Height = 2;
  float sweat2Width = 1;

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

  // Запуск RoboEyes с заданными параметрами
  void begin(int width, int height, byte frameRate) {
    screenWidth = width;
    screenHeight = height;
    displayCenterX = width / 2;
    displayCenterY = height / 2;
  //  display->fillScreen(backgroundColor);
    eyeLheightCurrent = 1;
    eyeRheightCurrent = 1;
    setFramerate(frameRate);
  }

  void update() {
    if(millis() - fpsTimer >= frameInterval) {
      drawEyes();
      fpsTimer = millis();
    }
  }

  //*********************************************************************************************
  //  НОВЫЕ МЕТОДЫ ДЛЯ УПРАВЛЕНИЯ ЦВЕТАМИ И РАДИУСОМ
  //*********************************************************************************************

  // Установить центр дисплея для радиального ограничения
  void Centrals(int center) {
    displayCenterX = center;
    displayCenterY = center;
  }

  // Включить/выключить радиальное ограничение движения
  void CRad(bool enabled) {
    radialConstraintEnabled = enabled;
  }

  // Установить радиус ограничения
  void setRadiusConstraint(int radius) {
    radialConstraintRadius = radius;
  }

  // Установить цвет фона
  void bgColor(uint16_t color) {
    backgroundColor = color;
    BGCOLOR_GC = color;
  }

  // Установить цвет фона через RGB
  void bgColor(uint8_t r, uint8_t g, uint8_t b) {
    backgroundColor = RGB565(r, g, b);
    BGCOLOR_GC = backgroundColor;
  }

  // Установить цвета белков обоих глаз (RGB)
  void EyColoryEy(uint8_t rL, uint8_t gL, uint8_t bL, uint8_t rR, uint8_t gR, uint8_t bR) {
    eyeLeftColor = RGB565(rL, gL, bL);
    eyeRightColor = RGB565(rR, gR, bR);
  }

  // Установить цвет правого белка (RGB)
  void EyRiht(uint8_t r, uint8_t g, uint8_t b) {
    eyeRightColor = RGB565(r, g, b);
  }

  // Установить цвет левого белка (RGB)
  void EyLeft(uint8_t r, uint8_t g, uint8_t b) {
    eyeLeftColor = RGB565(r, g, b);
  }

  // Установить цвета зрачков обоих глаз (RGB)
  void EyColsZR(uint8_t rL, uint8_t gL, uint8_t bL, uint8_t rR, uint8_t gR, uint8_t bR) {
    pupilLeftColor = RGB565(rL, gL, bL);
    pupilRightColor = RGB565(rR, gR, bR);
  }

  // Установить цвет правого зрачка (RGB)
  void EyRihtZR(uint8_t r, uint8_t g, uint8_t b) {
    pupilRightColor = RGB565(r, g, b);
  }

  // Установить цвет левого зрачка (RGB)
  void EyLeftZR(uint8_t r, uint8_t g, uint8_t b) {
    pupilLeftColor = RGB565(r, g, b);
  }

  // Сбросить цвета зрачков на черный
  void ResetZR() {
    pupilLeftColor = 0x0000;
    pupilRightColor = 0x0000;
  }

  // Сбросить цвета белков на белый
  void ResetGlas() {
    eyeLeftColor = 0xFFFF;
    eyeRightColor = 0xFFFF;
  }

  //*********************************************************************************************
  //  МЕТОДЫ УПРАВЛЕНИЯ ЗРАЧКАМИ
  //*********************************************************************************************

  // Установить размер зрачков
  void setPupilSize(int size) {
    pupilSizeNext = size;
    pupilSizeDefault = size;
  }

  // Установить смещение зрачков (для направления взгляда)
  void setPupilOffset(int xL, int yL, int xR, int yR) {
    pupilLoffsetXNext = xL;
    pupilLoffsetYNext = yL;
    pupilRoffsetXNext = xR;
    pupilRoffsetYNext = yR;
  }

  // Анимированное изменение размера зрачка
  void GlasSize(int startSize, int endSize, int speed) {
    pupilSizeCurrent = startSize;
    pupilSizeNext = endSize;
    // speed определяет скорость перехода (применить в update)
  }

  // Установить позицию взгляда зрачков
  void setGlas(int xL, int yL, int xR, int yR, int intensity, int saccades) {
    pupilLoffsetXNext = xL;
    pupilLoffsetYNext = yL;
    pupilRoffsetXNext = xR;
    pupilRoffsetYNext = yR;
    // intensity и saccades можно использовать для дополнительных эффектов
  }

  //*********************************************************************************************
  //  SETTERS (УСТАНОВОЧНЫЕ МЕТОДЫ)
  //*********************************************************************************************

  void setFramerate(byte fps) {
    frameInterval = 1000 / fps;
  }

  void setDisplayColors(uint16_t background, uint16_t main) {
    backgroundColor = background;
    BGCOLOR_GC = background;
    MAINCOLOR_GC = main;
  }

  void setWidth(byte leftEye, byte rightEye) {
    eyeLwidthNext = leftEye;
    eyeRwidthNext = rightEye;
    eyeLwidthDefault = leftEye;
    eyeRwidthDefault = rightEye;
  }

  void setHeight(byte leftEye, byte rightEye) {
    eyeLheightNext = leftEye;
    eyeRheightNext = rightEye;
    eyeLheightDefault = leftEye;
    eyeRheightDefault = rightEye;
  }

  void setBorderradius(byte leftEye, byte rightEye) {
    eyeLborderRadiusNext = leftEye;
    eyeRborderRadiusNext = rightEye;
    eyeLborderRadiusDefault = leftEye;
    eyeRborderRadiusDefault = rightEye;
  }

  void setSpacebetween(int space) {
    spaceBetweenNext = space;
    spaceBetweenDefault = space;
  }

  void setMood(unsigned char mood) {
    switch (mood) {
      case TIRED:
        tired = 1; angry = 0; happy = 0; surprised = 0; sad = 0;
        break;
      case ANGRY:
        tired = 0; angry = 1; happy = 0; surprised = 0; sad = 0;
        break;
      case HAPPY:
        tired = 0; angry = 0; happy = 1; surprised = 0; sad = 0;
        break;
      case SURPRISED:
        tired = 0; angry = 0; happy = 0; surprised = 1; sad = 0;
        break;
      case SAD:
        tired = 0; angry = 0; happy = 0; surprised = 0; sad = 1;
        break;
      default:
        tired = 0; angry = 0; happy = 0; surprised = 0; sad = 0;
        break;
    }
  }

  void setPosition(unsigned char position) {
    int maxX = getScreenConstraint_X();
    int maxY = getScreenConstraint_Y();
    
    switch (position) {
      case N:
        eyeLxNext = maxX / 2;
        eyeLyNext = 0;
        break;
      case NE:
        eyeLxNext = maxX;
        eyeLyNext = 0;
        break;
      case E:
        eyeLxNext = maxX;
        eyeLyNext = maxY / 2;
        break;
      case SE:
        eyeLxNext = maxX;
        eyeLyNext = maxY;
        break;
      case S:
        eyeLxNext = maxX / 2;
        eyeLyNext = maxY;
        break;
      case SW:
        eyeLxNext = 0;
        eyeLyNext = maxY;
        break;
      case W:
        eyeLxNext = 0;
        eyeLyNext = maxY / 2;
        break;
      case NW:
        eyeLxNext = 0;
        eyeLyNext = 0;
        break;
      default:
        eyeLxNext = maxX / 2;
        eyeLyNext = maxY / 2;
        break;
    }
  }

  void setAutoblinker(bool active, int interval, int variation) {
    autoblinker = active;
    blinkInterval = interval;
    blinkIntervalVariation = variation;
  }

  void setAutoblinker(bool active) {
    autoblinker = active;
  }

  void setIdleMode(bool active, int interval, int variation) {
    idle = active;
    idleInterval = interval;
    idleIntervalVariation = variation;
  }

  void setIdleMode(bool active) {
    idle = active;
  }

  void setCuriosity(bool curiousBit) {
    curious = curiousBit;
  }

  void setCyclops(bool cyclopsBit) {
    cyclops = cyclopsBit;
  }

  void setHFlicker(bool flickerBit, byte Amplitude) {
    hFlicker = flickerBit;
    hFlickerAmplitude = Amplitude;
  }

  void setHFlicker(bool flickerBit) {
    hFlicker = flickerBit;
  }

  void setVFlicker(bool flickerBit, byte Amplitude) {
    vFlicker = flickerBit;
    vFlickerAmplitude = Amplitude;
  }

  void setVFlicker(bool flickerBit) {
    vFlicker = flickerBit;
  }

  void setSweat(bool sweatBit) {
    sweat = sweatBit;
  }

  //*********************************************************************************************
  //  GETTERS
  //*********************************************************************************************

  int getScreenConstraint_X() {
    return screenWidth - eyeLwidthCurrent - spaceBetweenCurrent - eyeRwidthCurrent;
  }

  int getScreenConstraint_Y() {
    return screenHeight - eyeLheightDefault;
  }

  //*********************************************************************************************
  //  БАЗОВЫЕ МЕТОДЫ АНИМАЦИИ
  //*********************************************************************************************

  void close() {
    eyeLheightNext = 1;
    eyeRheightNext = 1;
    eyeL_open = 0;
    eyeR_open = 0;
  }

  void open() {
    eyeL_open = 1;
    eyeR_open = 1;
  }

  void blink() {
    close();
    open();
  }

  void close(bool left, bool right) {
    if (left) {
      eyeLheightNext = 1;
      eyeL_open = 0;
    }
    if (right) {
      eyeRheightNext = 1;
      eyeR_open = 0;
    }
  }

  void open(bool left, bool right) {
    if (left) {
      eyeL_open = 1;
    }
    if (right) {
      eyeR_open = 1;
    }
  }

  void blink(bool left, bool right) {
    close(left, right);
    open(left, right);
  }

  //*********************************************************************************************
  //  МАКРО-АНИМАЦИИ
  //*********************************************************************************************

  void anim_confused() {
    confused = 1;
  }

  void anim_laugh() {
    laugh = 1;
  }

  //*********************************************************************************************
  //  ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ СОВМЕСТИМОСТИ
  //*********************************************************************************************

  // Заглушки для функций из оригинального скетча
  void RotSetEnabled(bool enabled) { /* Заглушка */ }
  void RotSetMood(unsigned char mood) { /* Заглушка */ }
  void RotSetAutoblinker(bool enabled) { /* Заглушка */ }
  void RotAnim_laugh() { anim_laugh(); }
  void RotSetGrimace(bool enabled, int duration) { /* Заглушка */ }
  void RotSetAngryBreathing(bool enabled) { /* Заглушка */ }
  void RotSetYawn(bool enabled) { /* Заглушка */ }
  void RotSetSpeaking(bool enabled, int min, int max, int duration) { /* Заглушка */ }
  void RotSetLaughing(int type, int duration) { /* Заглушка */ }
  void RotSetSigh(bool enabled) { /* Заглушка */ }


  //*********************************************************************************************
  //  ЛОКАЛЬНАЯ ОЧИСТКА*
  //*********************************************************************************************
    void clearEyeArea(int x, int y, int w, int h) {
      if (w <= 0 || h <= 0) return;
      display->fillRect(x - 4, y - 4, w + 8, h + 8, backgroundColor);  //±4 px — запас 
      }
  //*********************************************************************************************
  //  РИСОВАНИЕ ГЛАЗ
  //*********************************************************************************************

  void drawEyes() {
    
    //// ПРЕДВЫЧИСЛЕНИЯ ////

    // Смещение высоты для curious режима
    if(curious) {
      if(eyeLxNext <= 10) {
        eyeLheightOffset = 8;
      } else if(eyeLxNext >= (getScreenConstraint_X() - 10) && cyclops) {
        eyeLheightOffset = 8;
      } else {
        eyeLheightOffset = 0;
      }
      
      if(eyeRxNext >= screenWidth - eyeRwidthCurrent - 10) {
        eyeRheightOffset = 8;
      } else {
        eyeRheightOffset = 0;
      }
    } else {
      eyeLheightOffset = 0;
      eyeRheightOffset = 0;
    }

    // Высота глаз
    eyeLheightCurrent = (eyeLheightCurrent + eyeLheightNext + eyeLheightOffset) / 2;
    eyeLy += ((eyeLheightDefault - eyeLheightCurrent) / 2);
    eyeLy -= eyeLheightOffset / 2;
    
    eyeRheightCurrent = (eyeRheightCurrent + eyeRheightNext + eyeRheightOffset) / 2;
    eyeRy += (eyeRheightDefault - eyeRheightCurrent) / 2;
    eyeRy -= eyeRheightOffset / 2;

    // Открытие глаз
    if(eyeL_open) {
      if(eyeLheightCurrent <= 1 + eyeLheightOffset) {
        eyeLheightNext = eyeLheightDefault;
      }
    }
    if(eyeR_open) {
      if(eyeRheightCurrent <= 1 + eyeRheightOffset) {
        eyeRheightNext = eyeRheightDefault;
      }
    }

    // Ширина глаз
    eyeLwidthCurrent = (eyeLwidthCurrent + eyeLwidthNext) / 2;
    eyeRwidthCurrent = (eyeRwidthCurrent + eyeRwidthNext) / 2;

    // Расстояние между глазами
    spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext) / 2;

    // Координаты глаз
    eyeLx = (eyeLx + eyeLxNext) / 2;
    eyeLy = (eyeLy + eyeLyNext) / 2;
    
    eyeRxNext = eyeLxNext + eyeLwidthCurrent + spaceBetweenCurrent;
    eyeRyNext = eyeLyNext;
    eyeRx = (eyeRx + eyeRxNext) / 2;
    eyeRy = (eyeRy + eyeRyNext) / 2;

    // Применение радиального ограничения
    if(radialConstraintEnabled) {
      constrainToRadius(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent);
      constrainToRadius(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent);
    }

    // Радиус скругления
    eyeLborderRadiusCurrent = (eyeLborderRadiusCurrent + eyeLborderRadiusNext) / 2;
    eyeRborderRadiusCurrent = (eyeRborderRadiusCurrent + eyeRborderRadiusNext) / 2;

    // Размер и смещение зрачков
    pupilSizeCurrent = (pupilSizeCurrent + pupilSizeNext) / 2;
    pupilLoffsetX = (pupilLoffsetX + pupilLoffsetXNext) / 2;
    pupilLoffsetY = (pupilLoffsetY + pupilLoffsetYNext) / 2;
    pupilRoffsetX = (pupilRoffsetX + pupilRoffsetXNext) / 2;
    pupilRoffsetY = (pupilRoffsetY + pupilRoffsetYNext) / 2;

    //// ПРИМЕНЕНИЕ МАКРО-АНИМАЦИЙ ////

    if(autoblinker) {
      if(millis() >= blinktimer) {
        blink();
        blinktimer = millis() + (blinkInterval * 1000) + (random(blinkIntervalVariation) * 1000);
      }
    }

    if(laugh) {
      if(laughToggle) {
        setVFlicker(1, 5);
        laughAnimationTimer = millis();
        laughToggle = 0;
      } else if(millis() >= laughAnimationTimer + laughAnimationDuration) {
        setVFlicker(0, 0);
        laughToggle = 1;
        laugh = 0;
      }
    }

    if(confused) {
      if(confusedToggle) {
        setHFlicker(1, 20);
        confusedAnimationTimer = millis();
        confusedToggle = 0;
      } else if(millis() >= confusedAnimationTimer + confusedAnimationDuration) {
        setHFlicker(0, 0);
        confusedToggle = 1;
        confused = 0;
      }
    }

    if(idle) {
      if(millis() >= idleAnimationTimer) {
        eyeLxNext = random(getScreenConstraint_X());
        eyeLyNext = random(getScreenConstraint_Y());
        idleAnimationTimer = millis() + (idleInterval * 1000) + (random(idleIntervalVariation) * 1000);
      }
    }

    if(hFlicker) {
      if(hFlickerAlternate) {
        eyeLx += hFlickerAmplitude;
        eyeRx += hFlickerAmplitude;
      } else {
        eyeLx -= hFlickerAmplitude;
        eyeRx -= hFlickerAmplitude;
      }
      hFlickerAlternate = !hFlickerAlternate;
    }

    if(vFlicker) {
      if(vFlickerAlternate) {
        eyeLy += vFlickerAmplitude;
        eyeRy += vFlickerAmplitude;
      } else {
        eyeLy -= vFlickerAmplitude;
        eyeRy -= vFlickerAmplitude;
      }
      vFlickerAlternate = !vFlickerAlternate;
    }

    if(cyclops) {
      eyeRwidthCurrent = 0;
      eyeRheightCurrent = 0;
      spaceBetweenCurrent = 0;
    }

    //// РИСОВАНИЕ ////
    // --- очистка предыдущих глаз ---
      if (prev_eyeLx >= 0) {
        clearEyeArea(prev_eyeLx, prev_eyeLy, prev_eyeLw, prev_eyeLh);
        }

      if (!cyclops && prev_eyeRx >= 0) {
        clearEyeArea(prev_eyeRx, prev_eyeRy, prev_eyeRw, prev_eyeRh);
        }

   //!!!!!!!!!!!!!!   Обновление -перерисовка
    display->fillScreen(backgroundColor);

    // Рисуем белки глаз
    display->fillRoundRect(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, 
                          eyeLborderRadiusCurrent, eyeLeftColor);
    
    if(!cyclops) {
      display->fillRoundRect(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, 
                            eyeRborderRadiusCurrent, eyeRightColor);
    }

    // Рисуем зрачки
    if(eyeLheightCurrent > 10) {
      int pupilLx = eyeLx + (eyeLwidthCurrent / 2) - (pupilSizeCurrent / 2) + pupilLoffsetX;
      int pupilLy = eyeLy + (eyeLheightCurrent / 2) - (pupilSizeCurrent / 2) + pupilLoffsetY;
      
      // Ограничиваем зрачок внутри глаза
      int maxOffsetX = (eyeLwidthCurrent - pupilSizeCurrent) / 2;
      int maxOffsetY = (eyeLheightCurrent - pupilSizeCurrent) / 2;
      pupilLx = constrain(pupilLx, eyeLx + 2, eyeLx + eyeLwidthCurrent - pupilSizeCurrent - 2);
      pupilLy = constrain(pupilLy, eyeLy + 2, eyeLy + eyeLheightCurrent - pupilSizeCurrent - 2);
      
      display->fillCircle(pupilLx + pupilSizeCurrent/2, pupilLy + pupilSizeCurrent/2, 
                         pupilSizeCurrent / 2, pupilLeftColor);
    }

    if(!cyclops && eyeRheightCurrent > 10) {
      int pupilRx = eyeRx + (eyeRwidthCurrent / 2) - (pupilSizeCurrent / 2) + pupilRoffsetX;
      int pupilRy = eyeRy + (eyeRheightCurrent / 2) - (pupilSizeCurrent / 2) + pupilRoffsetY;
      
      pupilRx = constrain(pupilRx, eyeRx + 2, eyeRx + eyeRwidthCurrent - pupilSizeCurrent - 2);
      pupilRy = constrain(pupilRy, eyeRy + 2, eyeRy + eyeRheightCurrent - pupilSizeCurrent - 2);
      
      display->fillCircle(pupilRx + pupilSizeCurrent/2, pupilRy + pupilSizeCurrent/2, 
                         pupilSizeCurrent / 2, pupilRightColor);
    }

    // Подготовка настроений
    if(tired) {
      eyelidsTiredHeightNext = eyeLheightCurrent / 2;
      eyelidsAngryHeightNext = 0;
    } else {
      eyelidsTiredHeightNext = 0;
    }
    
    if(angry) {
      eyelidsAngryHeightNext = eyeLheightCurrent / 2;
      eyelidsTiredHeightNext = 0;
    } else {
      eyelidsAngryHeightNext = 0;
    }
    
    if(happy) {
      eyelidsHappyBottomOffsetNext = eyeLheightCurrent / 2;
    } else {
      eyelidsHappyBottomOffsetNext = 0;
    }

    if(surprised) {
      eyelidsSurprisedTopOffsetNext = eyeLheightCurrent / 3;
    } else {
      eyelidsSurprisedTopOffsetNext = 0;
    }

    // Рисуем уставшие веки
    eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext) / 2;
    if(!cyclops) {
      display->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1, 
                           eyeLx, eyeLy + eyelidsTiredHeight - 1, backgroundColor);
      display->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1, 
                           eyeRx + eyeRwidthCurrent, eyeRy + eyelidsTiredHeight - 1, backgroundColor);
    } else {
      display->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, 
                           eyeLx, eyeLy + eyelidsTiredHeight - 1, backgroundColor);
      display->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, 
                           eyeLx + eyeLwidthCurrent, eyeLy - 1, 
                           eyeLx + eyeLwidthCurrent, eyeLy + eyelidsTiredHeight - 1, backgroundColor);
    }

    // Рисуем злые веки
    eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext) / 2;
    if(!cyclops) {
      display->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1, 
                           eyeLx + eyeLwidthCurrent, eyeLy + eyelidsAngryHeight - 1, backgroundColor);
      display->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1, 
                           eyeRx, eyeRy + eyelidsAngryHeight - 1, backgroundColor);
    } else {
      display->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, 
                           eyeLx + (eyeLwidthCurrent / 2), eyeLy + eyelidsAngryHeight - 1, backgroundColor);
      display->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, 
                           eyeLx + eyeLwidthCurrent, eyeLy - 1, 
                           eyeLx + (eyeLwidthCurrent / 2), eyeLy + eyelidsAngryHeight - 1, backgroundColor);
    }

    // Рисуем счастливые веки
    eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext) / 2;
    display->fillRoundRect(eyeLx - 1, (eyeLy + eyeLheightCurrent) - eyelidsHappyBottomOffset + 1, 
                          eyeLwidthCurrent + 2, eyeLheightDefault, 
                          eyeLborderRadiusCurrent, backgroundColor);
    if(!cyclops) {
      display->fillRoundRect(eyeRx - 1, (eyeRy + eyeRheightCurrent) - eyelidsHappyBottomOffset + 1, 
                            eyeRwidthCurrent + 2, eyeRheightDefault, 
                            eyeRborderRadiusCurrent, backgroundColor);
    }

    // Рисуем капли пота
    if(sweat) {
      // Капля 1
      if(sweat1YPos <= sweat1YPosMax) {
        sweat1YPos += 0.5;
      } else {
        sweat1XPosInitial = random(30);
        sweat1YPos = 2;
        sweat1YPosMax = (random(10) + 10);
        sweat1Width = 1;
        sweat1Height = 2;
      }
      if(sweat1YPos <= sweat1YPosMax / 2) {
        sweat1Width += 0.5;
        sweat1Height += 0.5;
      } else {
        sweat1Width -= 0.1;
        sweat1Height -= 0.5;
      }
      sweat1XPos = sweat1XPosInitial - (sweat1Width / 2);
      display->fillRoundRect(sweat1XPos, sweat1YPos, sweat1Width, sweat1Height, 
                            sweatBorderradius, MAINCOLOR_GC);

      // Капля 2
      if(sweat2YPos <= sweat2YPosMax) {
        sweat2YPos += 0.5;
      } else {
        sweat2XPosInitial = random((screenWidth - 60)) + 30;
        sweat2YPos = 2;
        sweat2YPosMax = (random(10) + 10);
        sweat2Width = 1;
        sweat2Height = 2;
      }
      if(sweat2YPos <= sweat2YPosMax / 2) {
        sweat2Width += 0.5;
        sweat2Height += 0.5;
      } else {
        sweat2Width -= 0.1;
        sweat2Height -= 0.5;
      }
      sweat2XPos = sweat2XPosInitial - (sweat2Width / 2);
      display->fillRoundRect(sweat2XPos, sweat2YPos, sweat2Width, sweat2Height, 
                            sweatBorderradius, MAINCOLOR_GC);

      // Капля 3
      if(sweat3YPos <= sweat3YPosMax) {
        sweat3YPos += 0.5;
      } else {
        sweat3XPosInitial = (screenWidth - 30) + (random(30));
        sweat3YPos = 2;
        sweat3YPosMax = (random(10) + 10);
        sweat3Width = 1;
        sweat3Height = 2;
      }
      if(sweat3YPos <= sweat3YPosMax / 2) {
        sweat3Width += 0.5;
        sweat3Height += 0.5;
      } else {
        sweat3Width -= 0.1;
        sweat3Height -= 0.5;
      }
      sweat3XPos = sweat3XPosInitial - (sweat3Width / 2);
      display->fillRoundRect(sweat3XPos, sweat3YPos, sweat3Width, sweat3Height, 
                            sweatBorderradius, MAINCOLOR_GC);
    }

    // --- сохранение текущих областей ---*
      prev_eyeLx = eyeLx;
      prev_eyeLy = eyeLy;
      prev_eyeLw = eyeLwidthCurrent;
      prev_eyeLh = eyeLheightCurrent;

      prev_eyeRx = eyeRx;
      prev_eyeRy = eyeRy;
      prev_eyeRw = eyeRwidthCurrent;
      prev_eyeRh = eyeRheightCurrent;

  } // конец drawEyes

}; // конец класса RoboEyes

#endif // _FLUXGARAGE_ROBOEYES_GC9A01A_H
