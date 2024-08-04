#ifndef FLIPFLOP_H
#define FLIPFLOP_H

#include <string>
#include <vector>

#include "Gate.h"

class Flipflop : public Gate {
private:
  bool used = false;
  Gate *inGate;
  int out = 0;
  std::string inName; 
  std::string outName;

public:
  Flipflop(Gate* newInGate, std::string newInName, std::string newOutName);
  virtual bool hasOutput(const std::string& str);
  virtual int getOutput(const std::string& str);
  virtual void evaluate();
  virtual bool isUsed();
  virtual void clearUsed();
};

#endif