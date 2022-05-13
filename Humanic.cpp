#include "Humanic.h"
#include "Robot.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Humanic::Humanic() {}

Humanic::Humanic(int newStrength, int newHit) {

  strength = newStrength;
  hitpoint = newHit;
}

/**
 * If a random number between 0 and 100 is less than 10, then Humanic robots will do +50 damage.
 * Otherwise, it will do normal damage.
 * 
 * @return The damage of the robot.
 */
int Humanic::getDamage() {

  if ((rand() % 100) < 10) { // To determine 10% chance

    cout << getType() << " - special +50 attack - " << endl;

    return Robot::getDamage() + 50;
  }

  cout << "Humanic - normal attack -" << endl;
  return Robot::getDamage();
}