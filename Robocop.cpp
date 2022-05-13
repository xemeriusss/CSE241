#include "Robocop.h"
#include "Humanic.h"
#include <iostream>
#include <string>

using namespace std;

Robocop::Robocop() {}

Robocop::Robocop(int newStrength, int newHit) {

  type = 1;
  strength = newStrength;
  hitpoint = newHit;
}

/**
 * The function getDamage() is a public function that returns an integer.
 *
 * No special attack.
 * 
 * @return The damage of the humanic.
 */
int Robocop::getDamage() {

  cout << "Robocop - normal attack -" << endl;
  return Humanic::getDamage();
}
