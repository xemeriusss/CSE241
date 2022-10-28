#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>

#include "Gate.h"

class Input : public Gate {
private:
  bool used = false;
  int *input = nullptr;
  std::string inputName;

public:
  Input(int *newInput, std::string newInputName);
  virtual bool hasOutput(const std::string &str);
  virtual int getOutput(const std::string &str);
  virtual void evaluate();
  virtual bool isUsed();
  virtual void clearUsed();
};

#endif