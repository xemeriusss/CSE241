#include "Flipflop.h"
#include <string>
#include <vector>

/**
 * The constructor for the FLIPFLOP class.
 * 
 * newIn1Gate: pointer to the input gate
 * newIn1Name: the name of the input gate
 * newOutName: the name of the output gate
 */
Flipflop::Flipflop(Gate *newInGate, std::string newInName, std::string newOutName)
    : inGate(newInGate), inName(newInName), outName(newOutName) {}

/**
 * If the name of the output is the same as the name of the input, then return true.
 */
bool Flipflop::hasOutput(const std::string &str) { return outName == str; }

/**
 * If the name of the output is the same as the name of the output that was passed in, 
 * then return the output. Otherwise, return -1.
 */
int Flipflop::getOutput(const std::string &str) {
  if (outName == str) {
    return out;
  } 
  else {
    return -1;
  }
}

/**
 * If the gate has not been used, evaluate the inputs, 
 * then set the output to the logical FLIPFLOP of the inputs.
 */
void Flipflop::evaluate() {
  if (isUsed()) {
    return;
  }

  inGate->evaluate();
  out = (out - inGate->getOutput(inName)) != 0;
  used = true;
}

bool Flipflop::isUsed() { return used; }

/**
 * It clears the used flag of the gate and its inputs.
 */
void Flipflop::clearUsed() {
  used = false;
  inGate->clearUsed();
}