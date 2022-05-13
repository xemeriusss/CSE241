#include "Bulldozer.h"
#include "Robot.h"
#include <iostream>
#include <string>

using namespace std;

Bulldozer::Bulldozer() {}

Bulldozer::Bulldozer(int newStrength, int newHit) {

  type = 3;
  strength = newStrength;
  hitpoint = newHit;
}

/**
 * The function getDamage() is a public function that returns an integer.
 *
 * No special attack.
 * 
 * @return The damage of the Robot.
 */
int Bulldozer::getDamage() {

  cout << "Bulldozer - normal attack -" << endl;
  return Robot::getDamage();
}