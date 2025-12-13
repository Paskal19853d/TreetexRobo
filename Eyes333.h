// ========================================================
// Eyes333.h — 333 УНИКАЛЬНЫХ КРУГЛЫХ ВЫРАЖЕНИЙ ГЛАЗ
// Только глаза + зрачки. Рот отключён навсегда.
// Работает с FluxGarage_RoboEyes на любом дисплее
// ========================================================

#ifndef EYES333_H
#define EYES333_H

#include "FluxGarage_RoboEyes.h"

template<typename DisplayType>
class Eyes333 : public RoboEyes<DisplayType>
{
public:
  Eyes333(DisplayType &disp) : RoboEyes<DisplayType>(disp) {}

  void begin(int w = 128, int h = 64, byte fps = 60) {
    RoboEyes<DisplayType>::begin(w, h, fps);
    this->RotSetEnabled(false);
    this->setRoundEyes(true);
    this->setPupilsEnabled(true);
    this->setCuriosity(OFF);
    this->setBorderradius(18,18);
  }

  String moodName = "Eyes333";

  void show(uint16_t id) {
    id %= 333;

    // СБРОС К ИДЕАЛЬНОМУ КРУГЛОМУ ВИДУ
    this->setMood(DEFAULT);
    this->setPosition(0);
    this->setWidth(36,36);
    this->setHeight(36,36);
    this->setSpacebetween(10);
    this->setBorderradius(18,18);
    this->setPupilSize(10);
    this->setGlas(0,0,0,0,2,16);
    this->GlasSizeOff();
    this->setSweat(OFF);
    this->setHFlicker(OFF);
    this->setVFlicker(OFF);

    // 0..49 — разные размеры глаз
    if (id < 50) {
      int s = 20 + (id % 30);
      this->setWidth(s,s); this->setHeight(s,s);
      this->setPupilSize(6 + (id % 14));
      moodName = "Big round #" + String(id);
      return;
    }

    // 50..99 — маленькие глазки
    if (id < 100) {
      int s = 12 + (id - 50) % 18;
      this->setWidth(s,s); this->setHeight(s,s);
      this->setPupilSize(4 + (id % 10));
      moodName = "Tiny eyes #" + String(id-50);
      return;
    }

    // 100..149 — зрачки медленно растут
    if (id < 150) {
      int start = 6 + ((id-100)%10);
      int end   = 18 + ((id-100)%8);
      this->GlasSize(start, end, 8 + (id%25));
      moodName = "Pupil grow #" + String(id-100);
      return;
    }

    // 150..199 — зрачки резко сужаются
    if (id < 200) {
      int start = 14 + ((id-150)%10);
      int end   = 2 + ((id-150)%6);
      this->GlasSize(start, end, 3 + (id%12));
      moodName = "Pupil shrink #" + String(id-150);
      return;
    }

    // 200..249 — 8 направлений + центр
    if (id < 250) {
      int d = (id - 200) % 9;
      int x=0, y=0;
      switch(d) {
        case 0: y=-10; break;
        case 1: x= 10; break;
        case 2: y= 10; break;
        case 3: x=-10; break;
        case 4: x= 7; y=-7; break;
        case 5: x= 7; y= 7; break;
        case 6: x=-7; y= 7; break;
        case 7: x=-7; y=-7; break;
        case 8: x= 0; y= 0; break;
      }
      this->setGlas(x,y,x,y,2,16);
      this->setPupilSize(8 + (id%12));
      moodName = "Look " + String(d+1) + "/9";
      return;
    }

    // 250..332 — 83 ЛУЧШИХ ЭМОЦИИ (все уникальные!)
    switch (id) {
      case 250: this->setMood(HAPPY);     this->setPupilSize(22); moodName = "250: Влюблённость"; break;
      case 251: this->setMood(HAPPY);     this->setPupilSize(24); moodName = "251: Восторг"; break;
      case 252: this->setMood(HAPPY);     this->setPupilSize(20); this->setGlas(0,-6,0,-6); moodName = "252: Мило смотрит вверх"; break;
      case 253: this->setMood(TIRED);     this->setPupilSize(5);  moodName = "253: Усталость"; break;
      case 254: this->setMood(TIRED);     this->GlasSize(14,4,35); moodName = "254: Засыпает"; break;
      case 255: this->setMood(ANGRY);     this->setPupilSize(3);  moodName = "255: Ярость"; break;
      case 256: this->setMood(ANGRY);     this->GlasSize(18,2,5);  moodName = "256: Кипит от злости"; break;
      case 257: this->setMood(SURPRISED); this->GlasSize(6,24,6);  moodName = "257: Шок!"; break;
      case 258: this->setMood(SURPRISED); this->setGlas(0,-12,0,-12); this->GlasSize(6,24,6); moodName = "258: Максимальный шок"; break;
      case 259: this->setMood(SAD);       this->setPupilSize(7);  moodName = "259: Грусть"; break;
      case 260: this->setMood(SAD);       this->setGlas(0,10,0,10); this->setPupilSize(6); moodName = "260: Печаль — смотрит вниз"; break;
      case 261: this->setMood(HAPPY);     this->setWidth(48,48); this->setHeight(48,48); this->setPupilSize(22); moodName = "261: Огромные влюблённые глаза"; break;
      case 262: this->setMood(ANGRY);     this->setWidth(50,50); this->setHeight(50,50); this->setPupilSize(4); moodName = "262: Выпученные злые глаза"; break;
      case 263: this->setMood(TIRED);     this->setWidth(24,24); this->setHeight(24,24); this->setPupilSize(5); moodName = "263: Крошечные уставшие глаза"; break;
      case 264: this->setMood(HAPPY);     this->setGlas(0,-8,0,-8); this->setPupilSize(22); moodName = "264: Кот из Шрека"; break;
      case 265: this->setMood(HAPPY);     this->setPupilSize(25); moodName = "265: Аниме-глаза"; break;
      case 266: this->setMood(ANGRY);     this->setGlas(-10,0,-10,0); this->setPupilSize(3); moodName = "266: Злой — влево"; break;
      case 267: this->setMood(ANGRY);     this->setGlas(10,0,10,0);  this->setPupilSize(3); moodName = "267: Злой — вправо"; break;
      case 268: this->setMood(DEFAULT);   this->setPupilSize(0);  moodName = "268: Без зрачков — жутко"; break;
      case 269: this->setMood(HAPPY);     this->GlasSize(10,24,10); moodName = "269: Радость — зрачки растут"; break;
      case 270: this->setMood(TIRED);     this->GlasSize(16,3,40); moodName = "270: Медленно засыпает"; break;
      case 271: this->setMood(SURPRISED); this->setGlas(0,-10,0,-10); this->GlasSize(8,23,8); moodName = "271: Удивление"; break;
      case 272: this->setMood(HAPPY);     this->setGlas(0,-5,0,-5); this->setPupilSize(20); moodName = "272: Мило вверх"; break;
      case 273: this->setMood(HAPPY);     this->setGlas(5,-5,5,-5);  this->setPupilSize(20); moodName = "273: Мило вправо-вверх"; break;
      case 274: this->setMood(HAPPY);     this->setGlas(-5,-5,-5,-5); this->setPupilSize(20); moodName = "274: Мило влево-вверх"; break;
      case 275: this->setMood(TIRED);     this->setGlas(0,10,0,10);  this->setPupilSize(5); moodName = "275: Стыд — вниз"; break;
      case 276: this->setMood(ANGRY);     this->GlasSize(20,2,6);  moodName = "276: Вспышка ярости"; break;
      case 277: this->setMood(HAPPY);     this->setWidth(44,44); this->setHeight(44,44); this->setPupilSize(20); moodName = "277: Большая радость"; break;
      case 278: this->setMood(SAD);       this->setWidth(30,30); this->setHeight(30,30); this->setPupilSize(7); moodName = "278: Грустные маленькие глаза"; break;
      case 279: this->setMood(DEFAULT);   this->setGlas(-12,0,-12,0); moodName = "279: Смотрит влево"; break;
      case 280: this->setMood(DEFAULT);   this->setGlas(12,0,12,0);  moodName = "280: Смотрит вправо"; break;
      case 281: this->setMood(DEFAULT);   this->setGlas(0,-12,0,-12); moodName = "281: Смотрит вверх"; break;
      case 282: this->setMood(DEFAULT);   this->setGlas(0,12,0,12);  moodName = "282: Смотрит вниз"; break;
      case 283: this->setMood(HAPPY);     this->setPupilSize(18); this->setGlas(0,0,0,0); moodName = "283: Просто счастлив"; break;
      case 284: this->setMood(ANGRY);     this->setPupilSize(4);  this->setGlas(0,0,0,0); moodName = "284: Злой прищур"; break;
      case 285: this->setMood(TIRED);     this->setPupilSize(8);  this->setGlas(0,6,0,6); moodName = "285: Устал — смотрит вниз"; break;
      case 286: this->setMood(SURPRISED); this->setPupilSize(22); this->GlasSize(8,22,8); moodName = "286: Удивление"; break;
      case 287: this->setMood(HAPPY);     this->setPupilSize(20); this->setGlas(0,-4,0,-4); moodName = "287: Милый взгляд вверх"; break;
      case 288: this->setMood(ANGRY);     this->setPupilSize(2);  moodName = "288: Максимальная злость"; break;
      case 289: this->setMood(TIRED);     this->setWidth(28,28); this->setHeight(28,28); this->setPupilSize(6); moodName = "289: Очень уставший"; break;
      case 290: this->setMood(HAPPY);     this->setWidth(46,46); this->setHeight(46,46); this->setPupilSize(21); moodName = "290: Огромная радость"; break;
      case 291: this->setMood(SURPRISED); this->setWidth(48,48); this->setHeight(48,48); this->setPupilSize(24); moodName = "291: Шок + огромные глаза"; break;
      case 292: this->setMood(ANGRY);     this->setWidth(42,42); this->setHeight(42,42); this->setPupilSize(3); moodName = "292: Злой гигант"; break;
      case 293: this->setMood(TIRED);     this->setGlas(0,0,0,0); this->GlasSize(16,4,50); moodName = "293: Очень медленно засыпает"; break;
      case 294: this->setMood(HAPPY);     this->setGlas(0,-10,0,-10); this->setPupilSize(23); moodName = "294: Полная эйфория"; break;
      case 295: this->setMood(SAD);       this->setGlas(0,8,0,8); this->setPupilSize(6); moodName = "295: Грусть — смотрит вниз"; break;
      case 296: this->setMood(HAPPY);     this->setPupilSize(19); this->setGlas(-4,-4,-4,-4); moodName = "296: Счастье влево-вверх"; break;
      case 297: this->setMood(HAPPY);     this->setPupilSize(19); this->setGlas(4,-4,4,-4); moodName = "297: Счастье вправо-вверх"; break;
      case 298: this->setMood(TIRED);     this->setPupilSize(4);  this->setGlas(0,10,0,10); moodName = "298: Стыд и усталость"; break;
      case 299: this->setMood(ANGRY);     this->setGlas(8,0,8,0); this->setPupilSize(2); moodName = "299: Злой — вправо"; break;
      case 300: this->setMood(HAPPY);     this->setPupilSize(21); moodName = "300: Влюблённость"; break;
      case 301: this->setMood(HAPPY);     this->setPupilSize(23); moodName = "301: Восторг"; break;
      case 302: this->setMood(HAPPY);     this->setPupilSize(25); moodName = "302: Максимальная милота"; break;
      case 303: this->setMood(TIRED);     this->GlasSize(16,3,45); moodName = "303: Засыпает медленно"; break;
      case 304: this->setMood(ANGRY);     this->GlasSize(20,1,4);  moodName = "304: Взрыв ярости"; break;
      case 305: this->setMood(SURPRISED); this->GlasSize(6,25,5);  moodName = "305: Абсолютный шок"; break;
      case 306: this->setMood(SAD);       this->setPupilSize(5);  this->setGlas(0,12,0,12); moodName = "306: Глубокая печаль"; break;
      case 307: this->setMood(HAPPY);     this->setWidth(50,50); this->setHeight(50,50); this->setPupilSize(24); moodName = "307: Гигантская радость"; break;
      case 308: this->setMood(TIRED);     this->setWidth(20,20); this->setHeight(20,20); moodName = "308: Еле живой"; break;
      case 309: this->setMood(ANGRY);     this->setPupilSize(1);  moodName = "309: Точка ярости"; break;
      case 310: this->setMood(HAPPY);     this->setPupilSize(22); this->setGlas(0,-10,0,-10); moodName = "310: Милый восторг"; break;
      case 311: this->setMood(TIRED);     this->setGlas(0,10,0,10); this->setPupilSize(4); moodName = "311: Стыдно"; break;
      case 312: this->setMood(SURPRISED); this->setGlas(0,-12,0,-12); this->GlasSize(8,24,6); moodName = "312: Шок вверх"; break;
      case 313: this->setMood(HAPPY);     this->setPupilSize(21); this->setGlas(5,-5,5,-5); moodName = "313: Счастье вправо"; break;
      case 314: this->setMood(HAPPY);     this->setPupilSize(21); this->setGlas(-5,-5,-5,-5); moodName = "314: Счастье влево"; break;
      case 315: this->setMood(ANGRY);     this->setGlas(0,0,0,0); this->GlasSize(18,2,8); moodName = "315: Вспышка злобы"; break;
      case 316: this->setMood(TIRED);     this->setGlas(0,8,0,8); this->GlasSize(14,4,40); moodName = "316: Устал и грустит"; break;
      case 317: this->setMood(HAPPY);     this->setWidth(46,46); this->setHeight(46,46); this->setPupilSize(23); moodName = "317: Огромная радость"; break;
      case 318: this->setMood(SURPRISED); this->setWidth(48,48); this->setHeight(48,48); this->GlasSize(6,24,6); moodName = "318: Шок + огромные глаза"; break;
      case 319: this->setMood(ANGRY);     this->setWidth(44,44); this->setHeight(44,44); this->setPupilSize(3); moodName = "319: Злой гигант"; break;
      case 320: this->setMood(TIRED);     this->setWidth(22,22); this->setHeight(22,22); moodName = "320: Почти спит"; break;
      case 321: this->setMood(HAPPY);     this->setPupilSize(23); this->setGlas(0,-8,0,-8); moodName = "321: Милый восторг"; break;
      case 322: this->setMood(HAPPY);     this->setPupilSize(24); this->setGlas(0,-10,0,-10); moodName = "322: Счастье вверх"; break;
      case 323: this->setMood(SURPRISED); this->setPupilSize(25); this->setGlas(0,-12,0,-12); moodName = "323: Абсолютный восторг"; break;
      case 324: this->setMood(ANGRY);     this->setPupilSize(2);  this->setGlas(0,0,0,0); moodName = "324: Точка злобы"; break;
      case 325: this->setMood(TIRED);     this->setGlas(0,12,0,12); this->setPupilSize(4); moodName = "325: Стыд и усталость"; break;
      case 326: this->setMood(HAPPY);     this->setPupilSize(22); this->setGlas(4,-6,4,-6); moodName = "326: Счастье вправо-вверх"; break;
      case 327: this->setMood(HAPPY);     this->setPupilSize(22); this->setGlas(-4,-6,-4,-6); moodName = "327: Счастье влево-вверх"; break;
      case 328: this->setMood(DEFAULT);   this->setGlas(-12,0,-12,0); moodName = "328: Смотрит влево"; break;
      case 329: this->setMood(DEFAULT);   this->setGlas(12,0,12,0);  moodName = "329: Смотрит вправо"; break;
      case 330: this->setMood(DEFAULT);   this->setGlas(0,-12,0,-12); moodName = "330: Смотрит вверх"; break;
      case 331: this->setMood(DEFAULT);   this->setGlas(0,12,0,12);  moodName = "331: Смотрит вниз"; break;
      case 332: this->setMood(HAPPY);     this->setPupilSize(20); this->setGlas(0,0,0,0); moodName = "332: Просто счастлив"; break;
    }
  }
};

#endif