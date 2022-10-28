#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <istream>
#include <string>
#include "Gate.h"

class Circuit {

private:
  std::vector<int> inputs; 
  Gate *outputDecoder;
  std::vector<std::string> inNames;
  std::vector<std::string> outNames;
  std::vector<Gate*> gatePool;

public:
  Circuit(std::istream &is);
  ~Circuit();
  std::vector<int> evaluateCircuit(const std::vector<int> &input);

};

#endif