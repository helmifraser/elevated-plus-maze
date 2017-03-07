#include "neural_network.hpp"

std::vector<float> neural_network::activationFunc(std::vector<float> input) {
  std::vector<float> out;

  for (int i = 0; i < input.size(); i++) {
    out.push_back(tanh(input[i]));
  }

  return out;
}

std::vector<float>
neural_network::layerCalc(std::vector<float> nodeOutputs,
          std::vector<std::vector<std::vector<float>>> weights, int layer) {

  std::vector<std::vector<float>> current = weights[layer];
  std::vector<float> layerOutput;
  float sum = 0;
  for (int i = 0; i < current.size(); i++) {
    sum = 0;
    for (int j = 0; j < nodeOutputs.size(); j++) {
      sum += nodeOutputs[j] * current[i][j];
    }
    std::cout << sum << std::endl;
    layerOutput.push_back(sum);
  }
  return layerOutput;
}
