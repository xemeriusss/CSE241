#include "Decoder.h"
#include <string>
#include <vector>
#include <iostream>

/**
 * The constructor for the DECODER class.
 * 
 * newIn1Gate: pointer to the first input gate
 * newIn2Gate: pointer to the second input gate
 * newIn1Name: the name of the first input gate
 * newIn2Name: the name of the second input gate
 * newOutName: the name of the output gates
 */
Decoder::Decoder(Gate *newIn1Gate, Gate *newIn2Gate, std::string newIn1Name, std::string newIn2Name, std::string newOut1Name,
                 std::string newOut2Name, std::string newOut3Name, std::string newOut4Name)
    : inGates{newIn1Gate, newIn2Gate}, inNames{newIn1Name, newIn2Name}, 
      outNames{newOut1Name, newOut2Name, newOut3Name, newOut4Name}, outs(4) {}

/**
 * If the name of the outputs are the same as the name of the input, then return true
 */
bool Decoder::hasOutput(const std::string &str) {
  return outNames[0] == str || outNames[1] == str || outNames[2] == str || outNames[3] == str;
}

/**
 * If the name of the outputs are the same as the name of the output that was passed in, 
 * then return the output. Otherwise, return -1.
 */
int Decoder::getOutput(const std::string &str) {
  if (outNames[0] == str) {
    return outs[0];
  } 
  else if (outNames[1] == str) {
    return outs[1];
  } 
  else if (outNames[2] == str) {
    return outs[2];
  } 
  else if (outNames[3] == str) {
    return outs[3];
  } 
  else {
    return -1;
  }
}

/**
 * If the gate has not been used, evaluate the inputs, 
 * then set the output to the logical DECODER of the inputs.
 */
void Decoder::evaluate() {
  if (isUsed()) {
    return;
  }

  inGates[0]->evaluate();
  inGates[1]->evaluate();
  int in1 = inGates[0]->getOutput(inNames[0]);
  int in2 = inGates[1]->getOutput(inNames[1]);

  outs[0] = !in1 && !in2;
  outs[1] = !in1 && in2;
  outs[2] = in1 && !in2;
  outs[3] = in1 && in2;
  used = true;
}

bool Decoder::isUsed() {
  return used;
}

/**
 * It clears the used flag of the gate and its inputs.
 */
void Decoder::clearUsed() {
  used = false;
  inGates[0]->clearUsed();
  inGates[1]->clearUsed();
}