#include "game.h"
#include "GameMechanics.h"
#include "meteor.h"
#include <stdlib.h>
#include <time.h> 

GameMechanics::GameMechanics() {
  srand(time(NULL));
}

GameMechanics::~GameMechanics() {

}

void GameMechanics::generateMeteors(Game* g) {
  amount = level*3;
  for(int i = 0; i < amount; i++) {
    int x = rand()%6000 + (-3000);
    int y = rand()%6000 + (-3000);
    int r = rand()%360;
    
    Meteor m;
    m.setId(rand()%2 + 1);
    m.setX(x);
    m.setY(y);
    m.setRotation(r);
    m.obj.setImage("res/meteor.png", g->e.getRenderer());
    m.obj.setDest(0,0,64,64);
    m.obj.setFrame(0, 0, 64, 64);
    g->meteors.push_back(m);
  }
}
