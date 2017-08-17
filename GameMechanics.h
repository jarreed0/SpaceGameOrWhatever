#ifndef GAMEMECHANICS_H
#define GAMEMECHANICS_H

#include <arch/arch.h>

class Game;

class GameMechanics {
private:
  
public:
  GameMechanics();
  ~GameMechanics();
  void generateMeteors(Game* g);
  int level = 1;
  int amount;
};

#endif //GAMEMECHANICS_H
