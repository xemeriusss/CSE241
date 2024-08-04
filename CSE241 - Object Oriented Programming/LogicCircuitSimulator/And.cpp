#include "And.h"
#include <string>
#include <vector>

/**
 * The constructor for the AND class.
 * 
 * newIn1Gate: pointer to the first input gate
 * newIn2Gate: pointer to the second input gate
 * newIn1Name: the name of the first input gate
 * newIn2Name: the name of the second input gate
 * newOutName: the name of the output gate
 */
And::And(Gate *newIn1Gate, Gate *newIn2Gate, std::string newIn1Name, std::string newIn2Name, std::string newOutName)
    : inGates{newIn1Gate, newIn2Gate}, inNames{newIn1Name, newIn2Name}, outName(newOutName) {}

/**
 * If the name of the output is the same as the name of the input, then return true.
 */
bool And::hasOutput(const std::string &str) { 

  return outName == str; 
}

/**
 * If the name of the output is the same as the name of the output that was passed in, 
 * then return the output. Otherwise, return -1.
 */
int And::getOutput(const std::string &str) {
  if (outName == str) {
    return out;
  } 
  else {
    return -1;
  }
}

/**
 * If the gate has not been used, evaluate the inputs, 
 * then set the output to the logical AND of the inputs.
 */
void And::evaluate() {
  if (isUsed()) {
    return;
  }

  inGates[0]->evaluate();
  inGates[1]->evaluate();
  out = inGates[0]->getOutput(inNames[0]) && inGates[1]->getOutput(inNames[1]);
  used = true;
}

bool And::isUsed() {
  return used;
}

/**
 * It clears the used flag of the gate and its inputs.
 */
void And::clearUsed() {
  used = false;
  inGates[0]->clearUsed();
  inGates[1]->clearUsed();
}