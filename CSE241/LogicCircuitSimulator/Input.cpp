#include "Input.h"
#include <string>
#include <vector>

/**
 * The constructor for the INPUT class.
 */
Input::Input(int *newInput, std::string newInputName)
    : input(newInput), inputName(newInputName) {}

/**
 * If the name of the output is the same as the name of the input, then return true.
 */
bool Input::hasOutput(const std::string &str) { return inputName == str; }

/**
 * If the input name is the same as the string passed in, return the value of the input.
 */
int Input::getOutput(const std::string &str) {
  if (inputName == str) {
    return *input;
  } 
  else {
    return -1;
  }
}

bool Input::isUsed() {
  return used;
}

/**
 * It clears the used flag of the gate.
 */
void Input::clearUsed() {
  used = false;
}

/**
 * If the input is used, then evaluate it.
 */
void Input::evaluate() { used = true; }