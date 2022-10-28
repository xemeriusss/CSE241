#ifndef GATE_H
#define GATE_H

#include <string>
#include <vector>

class Gate {
public:
  virtual bool isUsed() = 0;
  virtual void clearUsed() = 0;
  virtual bool hasOutput(const std::string& str) = 0;
  virtual int getOutput(const std::string& str) = 0;
  virtual void evaluate() = 0;
  virtual ~Gate() = default;
};

#endif