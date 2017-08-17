#ifndef METEOR_H
#define METEOR_H

#include <arch/arch.h>

class Meteor {
  private:
    int id, x, y, r;
    bool mined = false;
  public:
    Meteor();
    ~Meteor();
    void setMined(bool m);
    void setId(int i);
    int getId();
    bool isMined();
    Object getObject(int xoffset, int yoffset);
    int getX();
    int getY();
    int getRotation();
    void setX(int x);
    void setY(int y);
    void setRotation(int r);
    Object obj;
};

#endif //METEOR_H
