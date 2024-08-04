#include "Robot.h"
#include <iostream>
#include <string>

using namespace std;

class Roomba : public Robot { // Inherits from Robot class

public:
  Roomba();
  Roomba(int newStrength, int newHit);
  int getDamage();
};
