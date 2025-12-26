
#ifndef _TFT_ROBOEYES_H
#define _TFT_ROBOEYES_H
#include "eye.h"                // Bitmap_s изображений  

#include <TFT_eSPI.h>
// #include <TFT_eSprite.h>  // Include the sprite class header if needed

// Default color definitions (can be changed via setColors)
#define DEFAULT_BGCOLOR   TFT_BLACK
#define DEFAULT_MAINCOLOR TFT_WHITE

// Mood and position defines (same as original)
#define DEFAULT   0
#define TIRED     1
#define ANGRY     2
#define HAPPY     3

#define ON  1
#define OFF 0

// Predefined positions for the left eye
#define N   1  // north, top center
#define NE  2  // north-east, top right
#define E   3  // east, middle right
#define SE  4  // south-east, bottom right
#define S   5  // south, bottom center
#define SW  6  // south-west, bottom left
#define W   7  // west, middle left
#define NW  8  // north-west, top left

class TFT_RoboEyes {
  public:
    // Reference to the TFT display object
    TFT_eSPI *tft;
    
    // Off-screen drawing buffer (sprite) to reduce flicker
    TFT_eSprite *sprite;

    // Display configuration – you can update these via setScreenSize()
    int screenWidth = 240;   // effective width (set by user)
    int screenHeight = 240;  // effective height (set by user)
    uint16_t bgColor;        // background color for drawing overlays
    uint16_t mainColor;      // color for the eyes

    // Frame rate control
    int frameInterval;       // milliseconds per frame
    unsigned long fpsTimer;

    // Mood flags
    bool tired;
    bool angry;
    bool happy;
    bool curious;
    bool cyclops;   // if true, draw a single (center) eye
    bool eyeL_open;
    bool eyeR_open;

    // --- Eye Geometry Properties ---
    // Left eye dimensions
    int eyeLwidthDefault, eyeLheightDefault;
    int eyeLwidthCurrent, eyeLheightCurrent;
    int eyeLwidthNext, eyeLheightNext;
    int eyeLheightOffset;
    uint8_t eyeLborderRadiusDefault, eyeLborderRadiusCurrent, eyeLborderRadiusNext;

    // Right eye dimensions – mirror of left eye
    int eyeRwidthDefault, eyeRheightDefault;
    int eyeRwidthCurrent, eyeRheightCurrent;
    int eyeRwidthNext, eyeRheightNext;
    int eyeRheightOffset;
    uint8_t eyeRborderRadiusDefault, eyeRborderRadiusCurrent, eyeRborderRadiusNext;

    // Coordinates for left eye
    int eyeLxDefault, eyeLyDefault;
    int eyeLx, eyeLy;
    int eyeLxNext, eyeLyNext;

    // Coordinates for right eye
    int eyeRxDefault, eyeRyDefault;
    int eyeRx, eyeRy;
    int eyeRxNext, eyeRyNext;

    // Both eyes: eyelid and space parameters
    uint8_t eyelidsHeightMax;
    uint8_t eyelidsTiredHeight, eyelidsTiredHeightNext;
    uint8_t eyelidsAngryHeight, eyelidsAngryHeightNext;
    uint8_t eyelidsHappyBottomOffsetMax;
    uint8_t eyelidsHappyBottomOffset, eyelidsHappyBottomOffsetNext;
    int spaceBetweenDefault, spaceBetweenCurrent, spaceBetweenNext;

    // --- Animation Flags & Timers ---
    bool hFlicker;
    bool hFlickerAlternate;
    uint8_t hFlickerAmplitude;
    bool vFlicker;
    bool vFlickerAlternate;
    uint8_t vFlickerAmplitude;
    bool autoblinker;
    int blinkInterval;
    int blinkIntervalVariation;
    unsigned long blinktimer;
    bool idle;
    int idleInterval;
    int idleIntervalVariation;
    unsigned long idleAnimationTimer;
    bool confused;
    unsigned long confusedAnimationTimer;
    int confusedAnimationDuration;
    bool confusedToggle;
    bool laugh;
    unsigned long laughAnimationTimer;
    int laughAnimationDuration;
    bool laughToggle;

    // --- New Blink State for AutoBlinker ---
    bool blinkingActive;             // indicates if a blink is in progress (closed state)
    unsigned long blinkCloseDurationTimer; // timer for how long to stay closed
    int blinkCloseDuration = 150;    // blink closed duration in milliseconds

    // -------------------------->
    // === Параметры зрачков ===
    int pupilLxOffset = 0;   // смещение левого зрачка по X внутри глаза
    int pupilLyOffset = 0;   // смещение левого зрачка по Y
    int pupilRxOffset = 0;   // смещение правого зрачка по X
    int pupilRyOffset = 0;   // смещение правого зрачка по Y
    int pupilSize = 14;      // диаметр зрачка
    uint16_t pupilColor = TFT_BLACK; // цвет зрачка
    // --------------------------<
     
    bool dropsEnabled = false;   // показывать снег или нет
    bool dropsEnabled2 = false;   // показывать дождь или нет

    // ---------------------------
    // Constructor
    // ---------------------------
    TFT_RoboEyes(TFT_eSPI &display, bool portrait = true, int rotations = 1) {
      tft = &display;
      // Sprite will be allocated in begin()

      // Handle orientation
      if (!portrait) {
        screenWidth = 240;
        screenHeight = 240;
        tft->setRotation(rotations);
      }

      // Set default colors
      bgColor = DEFAULT_BGCOLOR;
      mainColor = DEFAULT_MAINCOLOR;

      // Default frame rate: 50fps
      frameInterval = 1000 / 50;
      fpsTimer = 0;

      // Initialize mood flags
      tired = angry = happy = curious = cyclops = false;
      eyeL_open = eyeR_open = false;

      // Initialize eye dimensions (default values, you can adjust later)
      eyeLwidthDefault = 36;
      eyeLheightDefault = 36;
      eyeLwidthCurrent = eyeLwidthDefault;
      eyeLheightCurrent = 1; // start closed
      eyeLwidthNext = eyeLwidthDefault;
      eyeLheightNext = eyeLheightDefault;
      eyeLheightOffset = 0;
      eyeLborderRadiusDefault = 8;
      eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
      eyeLborderRadiusNext = eyeLborderRadiusDefault;

      eyeRwidthDefault = eyeLwidthDefault;
      eyeRheightDefault = eyeLheightDefault;
      eyeRwidthCurrent = eyeRwidthDefault;
      eyeRheightCurrent = 1; // start closed
      eyeRwidthNext = eyeRwidthDefault;
      eyeRheightNext = eyeRheightDefault;
      eyeRheightOffset = 0;
      eyeRborderRadiusDefault = 8;
      eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
      eyeRborderRadiusNext = eyeRborderRadiusDefault;

      // Calculate default positions:
      spaceBetweenDefault = 10;
      spaceBetweenCurrent = spaceBetweenDefault;
      spaceBetweenNext = spaceBetweenDefault;
      eyeLxDefault = (screenWidth - (eyeLwidthDefault + spaceBetweenDefault + eyeRwidthDefault)) / 2;
      eyeLyDefault = (screenHeight - eyeLheightDefault) / 2;
      eyeLx = eyeLxDefault;
      eyeLy = eyeLyDefault;
      eyeLxNext = eyeLx;
      eyeLyNext = eyeLy;
      // Right eye
      eyeRxDefault = eyeLxDefault + eyeLwidthDefault + spaceBetweenDefault;
      eyeRyDefault = eyeLyDefault;
      eyeRx = eyeRxDefault;
      eyeRy = eyeRyDefault;
      eyeRxNext = eyeRx;
      eyeRyNext = eyeRy;

      eyelidsHeightMax = eyeLheightDefault / 2;
      eyelidsTiredHeight = 0;
      eyelidsTiredHeightNext = 0;
      eyelidsAngryHeight = 0;
      eyelidsAngryHeightNext = 0;
      eyelidsHappyBottomOffsetMax = (eyeLheightDefault / 2) + 3;
      eyelidsHappyBottomOffset = 0;
      eyelidsHappyBottomOffsetNext = 0;

      // Animation defaults
      hFlicker = false; hFlickerAlternate = false; hFlickerAmplitude = 2;
      vFlicker = false; vFlickerAlternate = false; vFlickerAmplitude = 10;
      autoblinker = false; blinkInterval = 1; blinkIntervalVariation = 4; blinktimer = 0;
      idle = false; idleInterval = 1; idleIntervalVariation = 3; idleAnimationTimer = 0;
      confused = false; confusedAnimationDuration = 500; confusedToggle = true;
      laugh = false; laughAnimationDuration = 500; laughToggle = true;

      // New auto-blink state
      blinkingActive = false;
      blinkCloseDurationTimer = 0;
    }

    // ---------------------------
    // Public methods
    // ---------------------------

    // ---------------------------------------------------
    void kapli(bool on) {
       dropsEnabled = on;
    }
        void sneg(bool on) {
       dropsEnabled2 = on;
    }
    // -------------------------------------------------->
      // смена цвета зрачков
      void setPupilColor(uint16_t color) {
        pupilColor = color;
      }

      // смена размера зрачка
      void setPupilSize(int size) {
        pupilSize = size;
      }
    // --------------------------------------------------<
    //Зрачок картинка
          void drawTransparentImage(
          TFT_eSprite *sprite,
          int x, int y,
          const uint16_t *img,
          int w, int h
        ) {
          for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
              uint16_t c = img[j * w + i];
              if (c != 0x0000) {
                sprite->drawPixel(x + i, y + j, c);
              }
            }
          }
        }


    // --------------------------------------------------<





    // Call from setup() to set up the sprite and reset the eyes.
    void begin(byte frameRate = 50) {
      // Allocate and create the sprite (off-screen buffer)
      sprite = new TFT_eSprite(tft);
      sprite->setColorDepth(8);
      sprite->createSprite(screenWidth, screenHeight);
      sprite->fillSprite(bgColor);

      eyeLheightCurrent = 1;
      eyeRheightCurrent = 1;
      setFramerate(frameRate);
      //---------------------------------------------------->инициализация СНЕГА:
    for (int i = 0; i < MAX_DROPS; i++) {
        drops[i].x = random(screenWidth);
        drops[i].y = random(-screenHeight, 0);
        drops[i].size = random(1, 6);  // размер 2…5 пикселей
        drops[i].speed = random(1, 2); // скорость падения
        drops[i].color = TFT_CYAN;  // можно менять на любой цвет
    }
      //----------------------------------------------------<
      //---------------------------------------------------->инициализация дОЖДЯ:
      for (int i2 = 0; i2 < MAX_DOZD; i2++) {
          dozd[i2].x2 = random(screenWidth);
          dozd[i2].y2 = random(-screenHeight, 0);
          dozd[i2].size2 = random(2, 5);
          dozd[i2].speed2 = random(1, 4);
          dozd[i2].color2 = TFT_SKYBLUE;  // можно менять цвет дождя
      }
      //----------------------------------------------------<

    }

    // Update the display; call often (e.g., inside loop())
    void update() {
      if (millis() - fpsTimer >= frameInterval) {
        drawEyes();                // draw on the sprite
        sprite->pushSprite(0, 0);    // push the complete frame in one go
        fpsTimer = millis();
      }
    }

    // Set the target frame rate (fps)
    void setFramerate(byte fps) {
      frameInterval = 1000 / fps;
    }

    // Use this function to update the screen dimensions (e.g., when switching orientation)
    void setScreenSize(int w, int h) {
      screenWidth = w;
      screenHeight = h;
      // Recalculate default positions for centering the eyes
      eyeLxDefault = (screenWidth - (eyeLwidthDefault + spaceBetweenDefault + eyeRwidthDefault)) / 2;
      eyeLyDefault = (screenHeight - eyeLheightDefault) / 2;
      eyeRxDefault = eyeLxDefault + eyeLwidthDefault + spaceBetweenDefault;
      eyeRyDefault = eyeLyDefault;
      // Update next positions for smooth transition
      eyeLxNext = eyeLxDefault;
      eyeLyNext = eyeLyDefault;
      eyeRxNext = eyeRxDefault;
      eyeRyNext = eyeRyDefault;
      // Recreate sprite with new dimensions
      if(sprite) {
        sprite->deleteSprite();
        sprite->createSprite(screenWidth, screenHeight);
      }
    }

    // Customization methods
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

    // Set mood expression
    void setMood(uint8_t mood) {
      switch (mood) {
        case TIRED:  tired = true; angry = false; happy = false; break;
        case ANGRY:  tired = false; angry = true; happy = false; break;
        case HAPPY:  tired = false; angry = false; happy = true; break;
        default:     tired = false; angry = false; happy = false; break;
      }
    }

    // Predefined position for left eye (affects both eyes)
    void setPosition(uint8_t position) {
      switch (position) {
        case N:
          eyeLxNext = getScreenConstraint_X() / 2;
          eyeLyNext = 0;
          break;
        case NE:
          eyeLxNext = getScreenConstraint_X();
          eyeLyNext = 0;
          break;
        case E:
          eyeLxNext = getScreenConstraint_X();
          eyeLyNext = getScreenConstraint_Y() / 2;
          break;
        case SE:
          eyeLxNext = getScreenConstraint_X();
          eyeLyNext = getScreenConstraint_Y();
          break;
        case S:
          eyeLxNext = getScreenConstraint_X() / 2;
          eyeLyNext = getScreenConstraint_Y();
          break;
        case SW:
          eyeLxNext = 0;
          eyeLyNext = getScreenConstraint_Y();
          break;
        case W:
          eyeLxNext = 0;
          eyeLyNext = getScreenConstraint_Y() / 2;
          break;
        case NW:
          eyeLxNext = 0;
          eyeLyNext = 0;
          break;
        default:
          // DEFAULT (center): use the preset default positions.
          eyeLxNext = eyeLxDefault;
          eyeLyNext = eyeLyDefault;
          break;
      }
    }

    // Set auto blink feature (in seconds)
    void setAutoblinker(bool active, int interval = 1, int variation = 4) {
      autoblinker = active;
      blinkInterval = interval;
      blinkIntervalVariation = variation;
      // Reset blink timers and state when enabling
      blinktimer = millis() + (blinkInterval * 1000UL) + (random(blinkIntervalVariation) * 1000UL);
      blinkingActive = false;
    }

    // Set idle mode (random repositioning)
    void setIdleMode(bool active, int interval = 1, int variation = 3) {
      idle = active;
      idleInterval = interval;
      idleIntervalVariation = variation;
    }

    // Enable or disable curious mode
    void setCuriosity(bool curiousBit) {
      curious = curiousBit;
    }

    // Enable or disable cyclops mode
    void setCyclops(bool cyclopsBit) {
      cyclops = cyclopsBit;
    }

    // Horizontal flickering
    void setHFlicker(bool flickerBit, uint8_t amplitude = 2) {
      hFlicker = flickerBit;
      hFlickerAmplitude = amplitude;
    }
    void setHFlicker(bool flickerBit) {
      hFlicker = flickerBit;
    }

    // Vertical flickering
    void setVFlicker(bool flickerBit, uint8_t amplitude = 10) {
      vFlicker = flickerBit;
      vFlickerAmplitude = amplitude;
    }
    void setVFlicker(bool flickerBit) {
      vFlicker = flickerBit;
    }

    // Set custom colors for drawing
    void setColors(uint16_t main, uint16_t background) {
      mainColor = main;
      bgColor = background;
    }

    // ---------------------------
    // Getters for screen constraints
    int getScreenConstraint_X() {
      return screenWidth - eyeLwidthCurrent - spaceBetweenCurrent - eyeRwidthCurrent;
    }
    int getScreenConstraint_Y() {
      return screenHeight - eyeLheightDefault;
    }

    // ---------------------------
    // Basic animation methods with modified close/open behavior
    void close() {
      eyeLheightNext = 1;
      eyeRheightNext = 1;
      eyeL_open = false;
      eyeR_open = false;
      eyeLborderRadiusNext = 0;
      eyeRborderRadiusNext = 0;
    }
    void open() {
      eyeL_open = true;
      eyeR_open = true;
      eyeLheightNext = eyeLheightDefault;
      eyeRheightNext = eyeRheightDefault;
      eyeLborderRadiusNext = eyeLborderRadiusDefault;
      eyeRborderRadiusNext = eyeRborderRadiusDefault;
    }
    void blink(bool left = true, bool right = true) {
      close();
      open();
    }
    void close(bool left, bool right) {
      if (left) { 
        eyeLheightNext = 1; 
        eyeL_open = false; 
        eyeLborderRadiusNext = 0;
      }
      if (right){ 
        eyeRheightNext = 1; 
        eyeR_open = false;
        eyeRborderRadiusNext = 0;
      }
    }
    void open(bool left, bool right) {
      if (left) { 
        eyeL_open = true; 
        eyeLheightNext = eyeLheightDefault;
        eyeLborderRadiusNext = eyeLborderRadiusDefault;
      }
      if (right){ 
        eyeR_open = true;
        eyeRheightNext = eyeRheightDefault;
        eyeRborderRadiusNext = eyeRborderRadiusDefault;
      }
    }

    void anim_confused() {
      confused = true;
    }
    void anim_laugh() {
      laugh = true;
    }

          // ----------------------------------------------------------------->

              // Ограничение смещения зрачка внутри белка глаза
        int constrainPupil(int offset, int eyeWidth, int eyeHeight) {
          int limitX = (eyeWidth / 2) - (pupilSize / 2) - 1;
          int limitY = (eyeHeight / 2) - (pupilSize / 2) - 1;
          if (offset > limitX) offset = limitX;
          if (offset < -limitX) offset = -limitX;
          if (offset > limitY) offset = limitY;
          if (offset < -limitY) offset = -limitY;
          return offset;
        }

      // -------------------------------------------------------------------------------<

  private: 
           
    // ---массив СНЕГА и параметры для анимации---
    struct Drop {
        int x;
        int y;
        int size;
        int speed;
        uint16_t color;
    };
    static const int MAX_DROPS = 20;  // количество капель
    Drop drops[MAX_DROPS];
    //------------------+++++дОЖДЬ
    struct Dozd {
        int x2;
        int y2;
        int size2;
        int speed2;
        float wind; 
        uint16_t color2;
    };
    static const int MAX_DOZD = 20;     // количество капель дождя
    Dozd dozd[MAX_DOZD];                 // массив дождевых капель

    // ---------------------------Рисую Сенег:
          void updateDrops() {
        for (int i = 0; i < MAX_DROPS; i++) {
            // стираем старую каплю
        // круглые капли    
        sprite->fillCircle(drops[i].x, drops[i].y, drops[i].size, bgColor);
  
            // обновляем позицию
            drops[i].y += drops[i].speed;
            if (drops[i].y > screenHeight) {
                drops[i].y = -drops[i].size;
                drops[i].x = random(screenWidth);
            }

            // рисуем новую каплю
            sprite->fillCircle(drops[i].x, drops[i].y, drops[i].size, drops[i].color);
         }

       }
   // ----------------------------------<
   // ----------------------------------->рисую Дождь v1
  /*    void updateDozd() {
          for (int i2 = 0; i2 < MAX_DOZD; i2++) {
              // вытягивает каплю по вертикали, чтобы больше походила на дождь.
              sprite->fillEllipse(dozd[i2].x2, dozd[i2].y2, dozd[i2].size2, dozd[i2].size2*2, bgColor);

              // обновляем позицию
              dozd[i2].y2 += dozd[i2].speed2;
              if (dozd[i2].y2 > screenHeight) {
                  dozd[i2].y2 = -dozd[i2].size2;
                  dozd[i2].x2 = random(screenWidth);
              }

              // рисуем новую каплю
              sprite->fillEllipse(dozd[i2].x2, dozd[i2].y2, dozd[i2].size2, dozd[i2].size2*2, dozd[i2].color2);
          }
      }
  */
   //-------------------------------------<
   // ----------------------------------->рисую Дождь v2
          void updateDozd() {
            for (int i2 = 0; i2 < MAX_DOZD; i2++) {

              // стереть старую каплю
              sprite->fillEllipse(
                dozd[i2].x2,
                dozd[i2].y2,
                dozd[i2].size2,
                dozd[i2].size2 * 3,
                bgColor
              );

              // физика падения
              dozd[i2].y2 += dozd[i2].speed2;
              dozd[i2].x2 += dozd[i2].wind;

              // ускорение (гравитация)
              dozd[i2].speed2 += 0.15;

              // если вышла за экран — пересоздаём
              if (dozd[i2].y2 > screenHeight) {
                dozd[i2].y2 = -random(20, 80);
                dozd[i2].x2 = random(screenWidth);
                dozd[i2].speed2 = random(1, 8);
                dozd[i2].size2 = random(1, 3);
                dozd[i2].wind = random(-10, 10) * 0.05;
              }

              // рисуем каплю
              sprite->fillEllipse(
                dozd[i2].x2,
                dozd[i2].y2,
                dozd[i2].size2,
                dozd[i2].size2 * 3,
                dozd[i2].color2
              );
            }
          }


   //-------------------------------------<

    // Core drawing logic – adapts animations and draws the eyes on the sprite.
                        

    void drawEyes() {

            float t = millis() * 0.002f;   // скорость движения взгляда
            // Единый вектор направления взгляда
            //---- Зрачки
            int pupilWidth = 32;
            int pupilHeight = 32;

        // --------------------------------------------------------------->
        // автоматическое небольшое движение зрачков

        // Левый глаз — плавное "осматривание"
           
             
            // Единый вектор направления взгляда
            float lookX = sinf(t) * 4.0f;
            float lookY = cosf(t * 0.8f) * 3.0f;

            // Левый глаз
            pupilLxOffset = lookX;
            pupilLyOffset = lookY;

            // Правый глаз — ТОТ ЖЕ вектор (никакого косоглазия)
            pupilRxOffset = lookX;
            pupilRyOffset = lookY;


        /*
          pupilLxOffset = sinf(millis() / 200.0f) * 3;
          pupilLyOffset = cosf(millis() / 200.0f) * 3;

          pupilRxOffset = sinf(millis() / 200.0f) * 3;
          pupilRyOffset = cosf(millis() / 200.0f) * 3;

      
          pupilLxOffset = sinf(millis() / 200.0f) * 3;
          pupilLyOffset = cosf(millis() / 200.0f) * 3;

          pupilRxOffset = sinf((millis() + 300) / 400.0f) * 4;
          pupilRyOffset = cosf((millis() + 300) / 400.0f) * 4;
        */
        // ---------------------------------------------------------------<

      // --- PRE-CALCULATIONS ---
      if (curious) {
        if (eyeLxNext <= 10) { eyeLheightOffset = 8; }
        else if (eyeLxNext >= (getScreenConstraint_X() - 10) && cyclops) { eyeLheightOffset = 8; }
        else { eyeLheightOffset = 0; }
        if (eyeRxNext >= screenWidth - eyeRwidthCurrent - 10) { eyeRheightOffset = 8; }
        else { eyeRheightOffset = 0; }
      } else {
        eyeLheightOffset = 0;
        eyeRheightOffset = 0;
      }

      // Smooth eye height transition
      eyeLheightCurrent = (eyeLheightCurrent + eyeLheightNext + eyeLheightOffset) / 2;
      eyeLy += ((eyeLheightDefault - eyeLheightCurrent) / 2);
      eyeLy -= eyeLheightOffset / 2;

      eyeRheightCurrent = (eyeRheightCurrent + eyeRheightNext + eyeRheightOffset) / 2;
      eyeRy += ((eyeRheightDefault - eyeRheightCurrent) / 2);
      eyeRy -= eyeRheightOffset / 2;

      if (eyeL_open) {
        if (eyeLheightCurrent <= 1 + eyeLheightOffset) { eyeLheightNext = eyeLheightDefault; }
      }
      if (eyeR_open) {
        if (eyeRheightCurrent <= 1 + eyeRheightOffset) { eyeRheightNext = eyeRheightDefault; }
      }

      // Smooth eye width transition
      eyeLwidthCurrent = (eyeLwidthCurrent + eyeLwidthNext) / 2;
      eyeRwidthCurrent = (eyeRwidthCurrent + eyeRwidthNext) / 2;
      spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext) / 2;

      // Smooth coordinate transitions
      eyeLx = (eyeLx + eyeLxNext) / 2;
      eyeLy = (eyeLy + eyeLyNext) / 2;
      // Right eye position depends on left eye pos + space + eye width
      eyeRxNext = eyeLxNext + eyeLwidthCurrent + spaceBetweenCurrent;
      eyeRyNext = eyeLyNext;
      eyeRx = (eyeRx + eyeRxNext) / 2;
      eyeRy = (eyeRy + eyeRyNext) / 2;

      // Smooth border radius transitions
      eyeLborderRadiusCurrent = (eyeLborderRadiusCurrent + eyeLborderRadiusNext) / 2;
      eyeRborderRadiusCurrent = (eyeRborderRadiusCurrent + eyeRborderRadiusNext) / 2;

      // --- MACRO ANIMATIONS ---
      if (autoblinker && !blinkingActive) {
        if (millis() >= blinktimer) {
          close();
          blinkingActive = true;
          blinkCloseDurationTimer = millis() + blinkCloseDuration; 
          blinktimer = millis() + (blinkInterval * 1000UL) + (random(blinkIntervalVariation) * 1000UL);
        }
      }
      if (blinkingActive && millis() >= blinkCloseDurationTimer) {
        open();
        blinkingActive = false;
      }

      if (laugh) {
        if (laughToggle) {
          setVFlicker(true, 5);
          laughAnimationTimer = millis();
          laughToggle = false;
        } else if (millis() >= laughAnimationTimer + laughAnimationDuration) {
          setVFlicker(false, 0);
          laughToggle = true;
          laugh = false;
        }
      }

      if (confused) {
        if (confusedToggle) {
          setHFlicker(true, 20);
          confusedAnimationTimer = millis();
          confusedToggle = false;
        } else if (millis() >= confusedAnimationTimer + confusedAnimationDuration) {
          setHFlicker(false, 0);
          confusedToggle = true;
          confused = false;
        }
      }

      if (idle) {
        if (millis() >= idleAnimationTimer) {
          eyeLxNext = random(getScreenConstraint_X());
          eyeLyNext = random(getScreenConstraint_Y());
          idleAnimationTimer = millis() + (idleInterval * 1000UL) + (random(idleIntervalVariation) * 1000UL);
        }
      }

      if (hFlicker) {
        if (hFlickerAlternate) {
          eyeLx += hFlickerAmplitude;
          eyeRx += hFlickerAmplitude;
        } else {
          eyeLx -= hFlickerAmplitude;
          eyeRx -= hFlickerAmplitude;
        }
        hFlickerAlternate = !hFlickerAlternate;
      }

      if (vFlicker) {
        if (vFlickerAlternate) {
          eyeLy += vFlickerAmplitude;
          eyeRy += vFlickerAmplitude;
        } else {
          eyeLy -= vFlickerAmplitude;
          eyeRy -= vFlickerAmplitude;
        }
        vFlickerAlternate = !vFlickerAlternate;
      }

      if (cyclops) {
        eyeRwidthCurrent = 0;
        eyeRheightCurrent = 0;
        spaceBetweenCurrent = 0;
      }

      // ----------------------------------------------------------------->
      // --- Обновление смещений зрачков ---Это гарантирует, что при любых размерах глаза зрачки остаются внутри белка.
      pupilLxOffset = constrainPupil(pupilLxOffset, eyeLwidthCurrent, eyeLheightCurrent);
      pupilLyOffset = constrainPupil(pupilLyOffset, eyeLwidthCurrent, eyeLheightCurrent);

      pupilRxOffset = constrainPupil(pupilRxOffset, eyeRwidthCurrent, eyeRheightCurrent);
      pupilRyOffset = constrainPupil(pupilRxOffset, eyeRwidthCurrent, eyeRheightCurrent);

      // -----------------------------------------------------------------<

      // --- ACTUAL DRAWINGS ---
      // Instead of clearing the TFT, clear the sprite to the background color.
      sprite->fillSprite(bgColor);

      // Draw eyes onto the sprite
      sprite->fillRoundRect(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, eyeLborderRadiusCurrent, mainColor);
      if (!cyclops) {
        sprite->fillRoundRect(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, eyeRborderRadiusCurrent, mainColor);
      }
      // ----------------------------------------------------------------->Важно: мы используем sprite — значит у всех спрайтов (глаз + зрачков) один буфер, и все оказываются на дисплее одновременно.
              // --- Рисуем зрачки картинкой ---
              if (!cyclops) {
                drawTransparentImage(
                  sprite,
                  eyeLx + (eyeLwidthCurrent / 2) - (pupilWidth / 2) + pupilLxOffset,
                  eyeLy + (eyeLheightCurrent / 2) - (pupilHeight / 2) + pupilLyOffset,
                  pupilImg, pupilWidth, pupilHeight
                );

                drawTransparentImage(
                  sprite,
                  eyeRx + (eyeRwidthCurrent / 2) - (pupilWidth / 2) + pupilRxOffset,
                  eyeRy + (eyeRheightCurrent / 2) - (pupilHeight / 2) + pupilRyOffset,
                  pupilImg, pupilWidth, pupilHeight
                );
              } else {
                drawTransparentImage(
                  sprite,
                  eyeLx + (eyeLwidthCurrent / 2) - (pupilWidth / 2) + pupilLxOffset,
                  eyeLy + (eyeLheightCurrent / 2) - (pupilHeight / 2) + pupilLyOffset,
                  pupilImg, pupilWidth, pupilHeight
                );
              }


      /*
          if (!cyclops) {
            // левый зрачок
            sprite->fillCircle(
              eyeLx + (eyeLwidthCurrent / 2) + pupilLxOffset,
              eyeLy + (eyeLheightCurrent / 2) + pupilLyOffset,
              pupilSize / 2,
              pupilColor
            );
            // правый зрачок
            sprite->fillCircle(
              eyeRx + (eyeRwidthCurrent / 2) + pupilRxOffset,
              eyeRy + (eyeRheightCurrent / 2) + pupilRyOffset,
              pupilSize / 2,
              pupilColor
            );
          } else {
            // если циклоп (один глаз)
            sprite->fillCircle(
              eyeLx + (eyeLwidthCurrent / 2) + pupilLxOffset,
              eyeLy + (eyeLheightCurrent / 2) + pupilLyOffset,
              pupilSize / 2,
              pupilColor
            );
          }
        */
      // -----------------------------------------------------------------<
      // Prepare mood transitions: tired, angry, happy
      if (tired) { 
        eyelidsTiredHeightNext = eyeLheightCurrent / 2; 
        eyelidsAngryHeightNext = 0; 
      } else { 
        eyelidsTiredHeightNext = 0; 
      }
      if (angry) { 
        eyelidsAngryHeightNext = eyeLheightCurrent / 2; 
        eyelidsTiredHeightNext = 0; 
      } else { 
        eyelidsAngryHeightNext = 0; 
      }
      if (happy) { 
        eyelidsHappyBottomOffsetNext = eyeLheightCurrent / 2; 
      } else { 
        eyelidsHappyBottomOffsetNext = 0; 
      } 

      //  добавляем проверку флага снега
          if (dropsEnabled) {
              updateDrops();
          }
      // -----------------------< 
      //  добавляем проверку флага капель дождя
        if (dropsEnabled2) {
            updateDozd();
        }
      // -----------------------< 

      // Tired eyelids
      eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext) / 2;
      if (!cyclops) {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx, eyeLy + eyelidsTiredHeight - 1, bgColor);
        sprite->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1,
                              eyeRx + eyeRwidthCurrent, eyeRy + eyelidsTiredHeight - 1, bgColor);
      } else {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1,
                              eyeLx, eyeLy + eyelidsTiredHeight - 1, bgColor);
        sprite->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx + eyeLwidthCurrent, eyeLy + eyelidsTiredHeight - 1, bgColor);
      }

      // Angry eyelids
      eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext) / 2;
      if (!cyclops) {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx + eyeLwidthCurrent, eyeLy + eyelidsAngryHeight - 1, bgColor);
        sprite->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1,
                              eyeRx, eyeRy + eyelidsAngryHeight - 1, bgColor);
      } else {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1,
                              eyeLx + (eyeLwidthCurrent / 2), eyeLy + eyelidsAngryHeight - 1, bgColor);
        sprite->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx + eyeLwidthCurrent, eyeLy + eyelidsAngryHeight - 1, bgColor);
      }

      // Happy (bottom) eyelids
      eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext) / 2;
      sprite->fillRoundRect(eyeLx - 1, (eyeLy + eyeLheightCurrent) - eyelidsHappyBottomOffset + 1,
                              eyeLwidthCurrent + 2, eyeLheightDefault, eyeLborderRadiusCurrent, bgColor);
      if (!cyclops) {
        sprite->fillRoundRect(eyeRx - 1, (eyeRy + eyeRheightCurrent) - eyelidsHappyBottomOffset + 1,
                              eyeRwidthCurrent + 2, eyeRheightDefault, eyeRborderRadiusCurrent, bgColor);
      }
    } // end drawEyes

}; // end class TFT_RoboEyes

#endif
