#include "Humanic.h"
#include <iostream>
#include <string>


using namespace std;

class Robocop : public Humanic { // Inherits from Humanic class

public:
  Robocop();
  Robocop(int newStrength, int newHit);
  int getDamage();
};