#ifndef GAME_H
#define GAME_H

#include <arch/arch.h>
#include "GameMechanics.h"
#include "meteor.h"
#include "base.h"

#define WIDTH 640
#define HEIGHT 480
#define FRAMERATE 60

class Game {
private:
  Object compass, minebar, minebarmax, timebar, timebarmax;
  Entity ship;
  Collision col;
  bool running, paused = false, rotater = false, rotatel = false, thrust = false, attach = false, docked = false, stationed = false;
  int shipspeed = 4, meteorscollected = 0, frame = 0, fcount = 0, fdelay = 5, time = 0, timemax = 100, tcount = 0, tdelay = 10;
  Input i;
  int dockedOn;
  Object screen;
  int storage, carrying;
  Base station;
public:
  Engine e;
  Game();
  ~Game();
  GameMechanics gm;
  void loop();
  void draw();
  void input();
  void update();
  void calcVelocity();
  void move();
  void updateMeteors();
  void drawCompass();
  void drawMeteors();
  void lookAtOb(Object o1, Object o2);
  void updateMinebar();
  void updateTimebar();
  void updateShip();
  void updateStation();
  void checkIfNear();
  vector<Meteor> meteors;
  int xoffset = 0, yoffset = 0;
  void allMined();
  double getDistance(Object o);
};

#endif //GAME_H
