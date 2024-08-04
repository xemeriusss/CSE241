#ifndef _HUMANIC_H
#define _HUMANIC_H
#include "Robot.h"
#include <iostream>
#include <string>

using namespace std;

class Humanic : public Robot { // Inherits from Robot class, also a Base class

public:
  Humanic();
  Humanic(int newStrength, int newHit);
  int getDamage();
};

#endif
