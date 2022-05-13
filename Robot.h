#ifndef _ROBOT_H
#define _ROBOT_H
#include <iostream>
using namespace std;

class Robot { // Base class

public:
  Robot();
  Robot(int newType, int newStrength, int newHit);

  int getStrength();
  void setStrength(int new_strength);
  int getHitpoint();
  void setHitpoint(int new_hitpoint);

  virtual int getDamage(); // virtual -> to make getDamage override

protected: // It's protected because of accessing from other robot classes.
  int type;
  int strength;
  int hitpoint;

  string getType();
};

#endif