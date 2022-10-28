#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "And.h"
#include "Circuit.h"
#include "Decoder.h"
#include "Flipflop.h"
#include "Gate.h"
#include "Input.h"
#include "Not.h"
#include "Or.h"

/**
 * It reads a line from the input stream,
 * parses it into a gate type and a list of operands, and returns
 * the gate type and operands
 */
static bool getOperands(std::istream &is, std::string &gateType, std::vector<std::string> &operands) {

  std::string line;

  if (!std::getline(is, line)) {
    return false;
  }

  std::istringstream iss(line);

  gateType.clear();

  iss >> gateType;

  operands.clear();

  std::string operand;

  /* Reading the input line and pushing the operands into the vector. */
  while (iss >> operand) {
    operands.push_back(operand);
  }
  return true;
}

/**
 * It searches through a vector of Gates and returns the gate that 
 * has an output with the given name
 */
static Gate *poolFindGateByOutName(std::vector<Gate *> pool, const std::string &outName) {

  for (unsigned i = 0; i < pool.size(); ++i) {

    if (pool[i]->hasOutput(outName)) {
      return pool[i];
    }
  }
  return nullptr;
}

/**
 * It reads the input file and creates the gates.
 */
Circuit::Circuit(std::istream &is) {

  std::string gateType;
  std::vector<std::string> operands;

  getOperands(is, gateType, operands);
  inNames = operands;

  /* Initializing the inputs vector to all 0. */
  for (unsigned i = 0; i < inNames.size(); ++i) {
    inputs.push_back(0);
  }

  getOperands(is, gateType, operands);
  outNames = operands;

  /* Creating a new Input gate for each input name. */
  for (unsigned i = 0; i < inNames.size(); ++i) {
    gatePool.push_back(new Input(&inputs[i], inNames[i]));
  }

  /* Reading the input file and creating the gates. */
  while (getOperands(is, gateType, operands)) {

    if (gateType == "AND") {
      /* Finding the gate that has an output with the given name. */
      Gate *in1 = poolFindGateByOutName(gatePool, operands[1]);
      Gate *in2 = poolFindGateByOutName(gatePool, operands[2]);
      
      /* Creating a new And gate with the given inputs and output name. */
      gatePool.push_back(new And(in1, in2, operands[1], operands[2], operands[0]));
    } 
    
    else if (gateType == "OR") {

      Gate *in1 = poolFindGateByOutName(gatePool, operands[1]);
      Gate *in2 = poolFindGateByOutName(gatePool, operands[2]);

      /* Creating a new Or gate with the given inputs and output name. */
      gatePool.push_back(new Or(in1, in2, operands[1], operands[2], operands[0]));
    } 
    
    else if (gateType == "NOT") {
      Gate *in = poolFindGateByOutName(gatePool, operands[1]);

      /* Creating a new Not gate with the given inputs and output name. */
      gatePool.push_back(new Not(in, operands[1], operands[0]));
    } 
    
    else if (gateType == "FLIPFLOP") {
      Gate *in = poolFindGateByOutName(gatePool, operands[1]);

      /* Creating a new Flipflop gate with the given inputs and output name. */
      gatePool.push_back(new Flipflop(in, operands[1], operands[0]));
    } 
    
    else if (gateType == "DECODER") {
      Gate *in1 = poolFindGateByOutName(gatePool, operands[4]);
      Gate *in2 = poolFindGateByOutName(gatePool, operands[5]);

      /* Creating a new Decoder gate with the given inputs and output name. */
      gatePool.push_back(new Decoder(in1, in2, operands[4], operands[5], operands[0], operands[1], operands[2], operands[3]));
    }

  }
  /* Finding the gate that has an output with the given name. */
  outputDecoder = poolFindGateByOutName(gatePool, outNames[0]);
}

/**
 * It deletes all the gates in the circuit
 */
Circuit::~Circuit() {
  for (auto v : gatePool) {
    delete v;
  }
}

/**
 * It takes in a vector of inputs, sets the inputs of the circuit, evaluates the circuit, 
 * and returns the outputs vector.
 */
std::vector<int> Circuit::evaluateCircuit(const std::vector<int> &input) {

  /* Setting the inputs of the circuit. */
  for (unsigned i = 0; i < inputs.size(); ++i) {
    inputs[i] = input[i];
  }

  outputDecoder->evaluate();

  std::vector<int> outputs;
  /* Pushing the output of the output decoder into the outputs vector. */
  for (const auto &v : outNames) {
    outputs.push_back(outputDecoder->getOutput(v));
  }

  outputDecoder->clearUsed();

  return outputs;
}