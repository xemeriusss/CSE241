#include "Robot.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

/**
 * The function getType() returns a string that is the name of the robot type
 * 
 * @return The type of robot.
 */
string Robot::getType() {

  switch (type) { // integer type

  case 0:
    return "optimusprime";
  case 1:
    return "robocop";
  case 2:
    return "roomba";
  case 3:
    return "bulldozer";
  }

  return "unknown";
}

Robot::Robot() {

  type = 3;
  strength = 10;
  hitpoint = 10;
}

Robot::Robot(int newType, int newStrength, int newHit)
    : type(newType), strength(newStrength), hitpoint(newHit) {}

int Robot::getStrength() { return this->strength; }

int Robot::getHitpoint() { return this->hitpoint; }

void Robot::setStrength(int new_strength) { this->strength = new_strength; }

void Robot::setHitpoint(int new_hitpoint) { this->hitpoint = new_hitpoint; }

/**
 * The getDamage() function returns a random number between 1 and the robot's strength.
 *
 * All robots inflict damage which is a random number up to their strength.
 * 
 * @return The damage that the robot inflicts.
 */
int Robot::getDamage() {

  int damage;

  damage = (rand() % strength) + 1;
  cout << getType() << " attacks for " << damage << " points!" << endl;

  return damage;
}
