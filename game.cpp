#include "game.h"

Game::Game() {
  e.debugMode(true);
  e.init("Space Miner", WIDTH, HEIGHT, 0);
  e.setColor(9, 13, 20);
  e.enableTransparency();
  running = true;
  e.setFrameRate(FRAMERATE);
  
  carrying=0;
  storage=20;

  //INITIATE STUFF HERE
  ship.setImage("res/shipsprite.png", e.getRenderer());
  ship.setFrame(0, 0, 64, 64);
  ship.setDest((WIDTH/2) - 32, (HEIGHT/2) - 32, 64, 64);
  ship.setMaxHealth(100);
  ship.setHealth(100);
  ship.setAngle(270);
  ship.setSpeed(0);

  compass.setImage("res/arrow.png", e.getRenderer());
  compass.setDest(0, 0, 16, 16);
  compass.setFrame(0, 0, 16, 16);

  minebar.setDest(32, 20, 256, 8);
  minebar.setFrame(minebar.getDest());
  minebar.setColor(66, 244, 95);

  minebarmax.setDest(32, 28, 258, 2);
  minebarmax.setFrame(minebarmax.getDest());
  minebarmax.setColor(66, 244, 95);

  timebar.setDest(32, 40, 256, 8);
  timebar.setFrame(minebar.getDest());
  timebar.setColor(244, 232, 66);

  timebarmax.setDest(32, 48, 258, 2);
  timebarmax.setFrame(minebarmax.getDest());
  timebarmax.setColor(244, 232, 66);

  station.obj.setImage("res/spacestation.png", e.getRenderer());
  station.obj.setDest((WIDTH/2)-(204/2), (HEIGHT/2)-(166/1.65), 204, 166);
  station.obj.setFrame(0, 0, 204, 166);

  station.barmax.setDestSize(202, 2);
  station.barmax.setFrame(station.barmax.getDest());
  station.barmax.setColor(10, 10, 150);

  station.bar.setDestSize(0, 8);
  station.bar.setFrame(station.bar.getDest());
  station.bar.setColor(10, 10, 150);

  gm.generateMeteors(this);
  updateMinebar();

  loop();
}

Game::~Game() {}

void Game::loop() {
  while(running) {
    input();
    if(!paused) update();
    draw();
    e.loop();
  }
}

void Game::draw() {
  drawMeteors();
  e.draw(station.obj);
  e.draw(station.barmax);
  e.draw(station.bar);
  //e.draw(station.col);
  e.draw(ship);
  drawCompass();
  e.draw(minebar);
  e.draw(minebarmax);
  if(docked) {e.draw(timebar); e.draw(timebarmax);}
  //DRAW OBJECTS HERE USING e.draw(object);
}

void Game::input() {
  i.logPress();
  //INPUT CHECKING
  if(i.checkKey(i.a) && !rotatel && !docked && !stationed) {rotatel = true;rotater = false;}
  if(!i.checkKey(i.a) && rotatel) {rotatel = false;}
  if(i.checkKey(i.d) && !rotater && !docked && !stationed) {rotater = true;rotatel = false;}
  if(!i.checkKey(i.d) && rotater) {rotater = false;}
  if(i.checkKey(i.w) && !thrust && !docked) {thrust = true;frame = 1;if(stationed) {stationed=false; thrust=true;ship.setSpeed(shipspeed);}}
  if(i.checkKey(i.e) && !attach) {attach = true;if(stationed) {stationed=false; thrust=true;ship.setSpeed(shipspeed);} else {checkIfNear();}}
  if(!i.checkKey(i.e) && attach) {attach = false;}
  if(i.checkKey(i.q)) {
    cout << "Size: ";
    cout << meteors.size();
    cout << "\nValues: ";
    for (int i = 0; i < meteors.size(); i++) {
      cout << meteors[i].getId();
      cout << " ";
    }
    cout << "\n";
  }

  //Allows game to exit
  if(i.checkKey(i.quit) || i.checkKey(i.esc)) {running = false;}
  
}

void Game::update() {
  //UPDATE METHODS HERE
  if(rotatel && !docked && !stationed) ship.setAngle(ship.getAngle() - 2);
  if(rotater && !docked && !stationed) ship.setAngle(ship.getAngle() + 2);
  calcVelocity();
  if(thrust && !docked && !stationed) {ship.setSpeed(shipspeed);}
  move();
  updateStation();
  screen.setDest(0, 0, WIDTH, HEIGHT);
  //updateMinebar();
  updateTimebar();
  updateShip();
}

void Game::calcVelocity() {
  double c = cos((ship.getAngle() * PI/180)) * ship.getSpeed();
  double s = sin((ship.getAngle() * PI/180)) * ship.getSpeed();
  ship.setVelX(c);
  ship.setVelY(s);
}

void Game::move() {
  xoffset += ship.getVelX();
  yoffset += ship.getVelY();
}

void Game::drawCompass() {
  if(!docked) {
    int nearest;
    int distance=3000;
    for (int i = 0; i < meteors.size(); i++) {
      if(!meteors[i].isMined()) {
	int dist=getDistance(meteors[i].obj);
	if(dist<distance) {distance=dist; nearest=i;}
      }
    }
    for (int i = 0; i < meteors.size(); i++) {
      if(!meteors[i].isMined()) {
        compass.setDest((WIDTH-8)/2, (HEIGHT-8)/2, 16, 16);
        compass.lookAt(meteors[i].getObject(xoffset, yoffset));
        double c = cos((compass.getAngle() * PI/180)) * 64;
        double s = sin((compass.getAngle() * PI/180)) * 64;
        compass.setDest(((WIDTH-8)/2) + c, ((HEIGHT-8)/2) + s, 16, 16);
	if(i==nearest) {compass.setFrameX(16);} else {compass.setFrameX(0);}
	if(col.overlaps(meteors[i].obj, screen)) {compass.setFrameY(16);} else {compass.setFrameY(0);}
	e.draw(compass);
      }
    }
    compass.setDest((WIDTH-8)/2, (HEIGHT-8)/2, 16, 16);
    compass.lookAt(station.col);
    double c = cos((compass.getAngle() * PI/180)) * 64;
    double s = sin((compass.getAngle() * PI/180)) * 64;
    compass.setDest(((WIDTH-8)/2) + c, ((HEIGHT-8)/2) + s, 16, 16);
    compass.setFrameX(32);
    if(col.overlaps(station.obj, screen)) {compass.setFrameY(16);} else {compass.setFrameY(0);}
    e.draw(compass);
  }
}

void Game::drawMeteors() {
  for(int i = 0; i < meteors.size(); i++) {
      meteors[i].obj.setDestX(meteors[i].getX() - xoffset);
      meteors[i].obj.setDestY(meteors[i].getY() - yoffset);
    if(col.overlaps(meteors[i].obj, screen)) {
      meteors[i].obj.setAngle(meteors[i].getRotation());
      meteors[i].setRotation(meteors[i].getRotation() + 3);
      if(meteors[i].getRotation() >= 360){meteors[i].setRotation(0);}
      if(meteors[i].isMined()) meteors[i].obj.setImage("res/shipsprite.png", e.getRenderer());
      e.draw(meteors[i].obj);
    } else if(meteors[i].isMined()) {
      meteors.erase(meteors.begin()+i);
    }
  }
}

void Game::allMined() {
 bool all=1;
 for(int i = 0; i < meteors.size(); i++) {
   if(!meteors[i].isMined()) {all=0; break;}
 }
 if(all) { gm.level++; gm.generateMeteors(this); }
}

void Game::updateMinebar() {
  if(carrying > storage) carrying=storage;
  double prog = 0;
  if(carrying!=0) prog = (256*carrying)/storage;
  minebar.setDestW(prog);
}

void Game::updateTimebar() {
  if(docked) {
    tcount++;
    if(tcount >= tdelay) {
      tcount = 0;
      time++;
      if(time >= timemax) {
        docked = false;
        time = 0;
	carrying+=(2*meteors[dockedOn].getId());
	updateMinebar();
        meteors[dockedOn].setMined(true);
      }
    }
    double prog = ((double)time / timemax) * 256.0;
    timebar.setDestW(prog + (2*meteors[dockedOn].getId()));
    allMined();
  }
}

void Game::updateShip() {
  if(ship.getSpeed() > 0) {
    fcount++;
    if(fcount >= fdelay) {
      fcount = 0;
      frame++;
      if(frame >= 3) {
        frame = 1;
      }
    }
  }
  ship.setFrame(64*frame, 0, 64, 64);
}

void Game::checkIfNear() {
  if(!(carrying > storage)) {
    for(int i = 0; i < meteors.size(); i++) {
      if(col.overlaps(ship, meteors[i].getObject(xoffset, yoffset))) {
        if(!meteors[i].isMined()) {
          frame = 0;
          ship.setSpeed(0);
          thrust = false;
          docked = true;
	  dockedOn = i;
        }
      }
    }
  }
  if(col.overlaps(ship, station.col)) {
    xoffset=yoffset=0;
    stationed = true;
    thrust = false;
    ship.setSpeed(0);
    station.holding=carrying;
    carrying=0;
    if(station.holding>station.max) station.holding=0;
    double prog = 0;
    if(station.holding!=0) prog = (101*station.holding)/station.max;
    station.bar.setDestW(prog);
    minebar.setDestW(0);
    frame=0;
  }
}

double Game::getDistance(Object o) {
  return sqrt(abs(((o.getDestX() - xoffset) ^ 2)+((o.getDestY()-yoffset) ^ 2)));
}

void Game::updateStation() {
  station.obj.setDestCoord((WIDTH/2)-(204/2)-xoffset, (HEIGHT/2)-(166/1.65)-yoffset);
  station.barmax.setDestCoord((WIDTH/2)-101-xoffset, (HEIGHT/2)+113-yoffset);
  station.bar.setDestCoord((WIDTH/2)-101-xoffset, (HEIGHT/2)+105-yoffset);
  station.col.setDest((WIDTH/2)-20-xoffset, (HEIGHT/2)-20-yoffset, 40, 40);
}







