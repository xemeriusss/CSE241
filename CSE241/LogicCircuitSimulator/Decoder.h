#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>

#include "Gate.h"

class Decoder : public Gate {
private:
  bool used = false;
  std::vector<Gate*> inGates;
  std::vector<std::string> inNames;
  std::vector<std::string> outNames;
  std::vector<int> outs;

public:
  Decoder(Gate *newIn1Gate, Gate *newIn2Gate, std::string newIn1Name,
          std::string newIn2Name, std::string newOut1Name,
          std::string newOut2Name, std::string newOut3Name,
          std::string newOut4Name);
  virtual bool hasOutput(const std::string &str);
  virtual int getOutput(const std::string &str);
  virtual void evaluate();
  virtual bool isUsed();
  virtual void clearUsed();
};

#endif