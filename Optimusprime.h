#include "Humanic.h"
#include <iostream>
#include <string>

using namespace std;

class Optimusprime : public Humanic { // Inherits from Humanic class

public:
  Optimusprime();
  Optimusprime(int newStrength, int newHit);
  int getDamage();
};