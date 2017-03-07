#include <Eigen/Dense>
#include <array>
#include <iostream>
#include <cmath>

class neural_network {
public:
  std::vector<float> activationFunc(std::vector<float> input);
  std::vector<float> layerCalc(
      std::vector<float> nodeOutputs,
      std::vector<std::vector<std::vector<float>>> weights, int layer);
};
