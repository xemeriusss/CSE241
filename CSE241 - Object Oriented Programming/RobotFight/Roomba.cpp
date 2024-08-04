#include "Roomba.h"
#include "Robot.h"
#include <iostream>
#include <string>

using namespace std;

Roomba::Roomba() {}

Roomba::Roomba(int newStrength, int newHit) {

  type = 2;
  strength = newStrength;
  hitpoint = newHit;
}

/**
 * The function getDamage() is a public function that returns an integer. It is a virtual function that
 * is overridden in the Roomba class. It is a member function of the Roomba class. 
 *
 * Roomba robots are very fast, so they get to attack twice.
 * 
 * @return The damage of the robot is being returned.
 */
int Roomba::getDamage() {

  cout << "Roomba - special twice attack -" << endl;
  return (Robot::getDamage() + Robot::getDamage());
}
