#include "Robot.h"
#include <iostream>
#include <string>

using namespace std;

class Bulldozer : public Robot { // Inherits from Robot class

public:
  Bulldozer();
  Bulldozer(int newStrength, int newHit);
  int getDamage();
};