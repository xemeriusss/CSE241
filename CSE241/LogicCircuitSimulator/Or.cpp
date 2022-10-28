#include "Or.h"
#include <string>
#include <vector>

/**
 * The constructor for the OR class.
 * 
 * newIn1Gate: pointer to the first input gate
 * newIn2Gate: pointer to the second input gate
 * newIn1Name: the name of the first input gate
 * newIn2Name: the name of the second input gate
 * newOutName: the name of the output gate
 */
Or::Or(Gate *newIn1Gate, Gate *newIn2Gate, std::string newIn1Name, std::string newIn2Name, std::string newOutName)
    : inGates{newIn1Gate, newIn2Gate}, inNames{newIn1Name, newIn2Name}, outName(newOutName) {}

/**
 * If the name of the output is the same as the name of the input, then return true.
 */
bool Or::hasOutput(const std::string &str) { return outName == str; }

/**
 * If the name of the output is the same as the name of the output that was passed in, 
 * then return the output. Otherwise, return -1.
 */
int Or::getOutput(const std::string &str) {
  if (outName == str) {
    return out;
  } 
  else {
    return -1;
  }
}

/**
 * If the gate has not been used, evaluate the inputs, 
 * then set the output to the logical OR of the inputs.
 */
void Or::evaluate() {
  if (isUsed()) {
    return;
  }

  inGates[0]->evaluate();
  inGates[1]->evaluate();
  out = inGates[0]->getOutput(inNames[0]) || inGates[1]->getOutput(inNames[1]);
  used = true;
}

bool Or::isUsed() {
  return used;
}

/**
 * It clears the used flag of the gate and its inputs.
 */
void Or::clearUsed() {
  used = false;
  inGates[0]->clearUsed();
  inGates[1]->clearUsed();
}