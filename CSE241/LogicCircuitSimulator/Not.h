#ifndef NOT_H
#define NOT_H

#include <vector>
#include <string>

#include "Gate.h"

class Not : public Gate {
private:
  bool used = false;
  Gate *inGate;
  int out = 0;
  std::string inName; 
  std::string outName;

public:
  Not(Gate* newInGate, std::string newInName, std::string newOutName);
  virtual bool hasOutput(const std::string& str);
  virtual int getOutput(const std::string& str);
  virtual void evaluate();
  virtual bool isUsed();
  virtual void clearUsed();
};

#endif