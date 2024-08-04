#ifndef OR_H
#define OR_H

#include <string>
#include <vector>

#include "Gate.h"

class Or : public Gate {
private:
  bool used = false;
  std::vector<Gate *>inGates;
  int out = 0;
  std::vector<std::string> inNames; 
  std::string outName;

public:
  Or(Gate* newIn1Gate, Gate* newIn2Gate, std::string newIn1Name, std::string newIn2Name, std::string newOutName);
  virtual bool hasOutput(const std::string& str);
  virtual int getOutput(const std::string& str);
  virtual void evaluate();
  virtual bool isUsed();
  virtual void clearUsed();
};

#endif