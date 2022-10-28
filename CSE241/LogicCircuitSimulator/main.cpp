#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Circuit.h"

int main(){

  /* Opening the file "circuit.txt" */
  std::fstream circuitfile("circuit.txt");
  if (!circuitfile.is_open()) {
    return 1;
  }

  /* Creating a Circuit object and passing the circuitfile to the constructor. */
  Circuit circuit(circuitfile);

  /* Opening the file "input.txt" */
  std::fstream inputfile("input.txt");
  if (!inputfile.is_open()) {
    return 1;
  }

  std::string line;
  std::vector<std::vector<int>> inputs;

  /* This is reading the input file and storing the inputs in a vector of vectors. */
  while (std::getline(inputfile, line)) {
    std::istringstream iss(line);
    std::vector<int> input;

    int in;
    while (iss >> in) { // input file
      input.push_back(in); // one by one
    }

    inputs.push_back(input); // line
  }

  /* Iterating through the inputs line by line and printing the output. */
  for (auto &input : inputs) {

    std::vector<int> output;

    /* Sending a line of inputs to the evaluateCircuit function. */
    output = circuit.evaluateCircuit(input); 

    /* Printing the output. */
    for (unsigned i = 0; i < output.size(); ++i) {
      if (i > 0) {
        std::cout << ' ';
      }
      std::cout << output[i];
    }
    std::cout << '\n';
    
  }
}