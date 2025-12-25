#include <TFT_eSPI.h>           // Подключаем библиотеку для работы с TFT-дисплеем
#include "RoboEyesTFT_eSPI.h"   // Подключаем библиотеку анимированных глаз RoboEyes

// Создаём объект дисплея TFT
TFT_eSPI tft = TFT_eSPI();  

// Создаём объект глаз RoboEyes
// false — ориентация экрана (false = альбомная, true = портретная)
TFT_RoboEyes roboEyes(tft, false, 6); // 3 — коэффициент сглаживания анимации (чем больше, тем плавнее)

 

void setup() {

  tft.init();           // Инициализация дисплея
  tft.setRotation(0);   // Поворот экрана (0, 1, 2, 3)  0 — стандартная ориентация
  roboEyes.begin(100);  // Запуск глаз с частотой обновления 100 FPS
                        // Установка цветов глаз:
  roboEyes.setColors(TFT_WHITE, TFT_BLACK); // первый параметр — цвет глаз (белки) \  второй — цвет фона

  // Включаем автоматическое моргание:
  roboEyes.setAutoblinker(true, 2, 1); // 2 — минимальный интервал , 1 — максимальный интервал 

  // Включаем режим "ожидания" (глаза двигаются сами)
  // 4 — частота движения
  // 0 — направление (0 = случайное)
  roboEyes.setIdleMode(true, 4, 0);
  roboEyes.setWidth(64, 64);          // Устанавливаем ширину каждого глаза
  roboEyes.setHeight(64, 64);         // Устанавливаем высоту каждого глаза
  roboEyes.setBorderradius(15, 15);   // Скругление углов глаз
  roboEyes.setSpacebetween(36);       // Расстояние между глазами
}
 
void loop() {
  // Обновление анимации глаз (ОБЯЗАТЕЛЬНО вызывать постоянно)
  roboEyes.update();
 
  //------Старт
 
    roboEyes.setColors(TFT_WHITE, TFT_BLACK);
    roboEyes.setMood(ANGRY_S);
     
  }

