#include "meteor.h"

Meteor::Meteor() {

}

Meteor::~Meteor() {

}

void Meteor::setId(int i) {
  id = i;
}

int Meteor::getId() {
  return id;
}

void Meteor::setMined(bool m) {
  mined = m;
}

bool Meteor::isMined() {
  return mined;
}

int Meteor::getX() {
  return x;
}

int Meteor::getY() {
  return y;
}

int Meteor::getRotation() {
  return r;
}

void Meteor::setX(int xe) {
  x = xe;
}

void Meteor::setY(int ye) {
  y = ye;
}

void Meteor::setRotation(int re) {
  r = re;
}

Object Meteor::getObject(int xoffset, int yoffset) {
  Object f;
  f.setDest(getX() - xoffset, getY() - yoffset, 64, 64);
  return f;
}
