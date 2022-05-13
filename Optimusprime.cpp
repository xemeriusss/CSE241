#include "Optimusprime.h"
#include "Humanic.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Optimusprime::Optimusprime() {}

Optimusprime::Optimusprime(int newStrength, int newHit) {

  type = 0;
  strength = newStrength;
  hitpoint = newHit;
}

/**
 * If a random number between 0 and 100 is less than 15, then Optimusprime will do double damage.
 * Otherwise, it will do normal damage.
 * 
 * @return The damage of the attack.
 */
int Optimusprime::getDamage() {

  if ((rand() % 100) < 15) { // To determine 15% chance

    cout << "Optimusprime - special double attack -" << endl;
    return (Humanic::getDamage() * 2);
  }

  cout << "Optimusprime - normal attack -" << endl;
  return Humanic::getDamage();
}