#include "neural_network.hpp"

std::vector<float> v2 = {1, 1, 1, 1, 1, 1, 1, 1};
std::vector<float> v3 = {1, 1, 1, 1, 1, 1, 1, 1};
std::vector<float> v5 = {1, 1, 1, 1, 1, 1, 1, 1};
std::vector<float> v6 = {1, 1, 1, 1, 1, 1, 1, 1};
std::vector<float> v7 = {1, 1, 1, 1, 1, 1, 1, 1};

std::vector<float> v8 = {1, 1, 1, 1};
std::vector<float> v9 = {1, 1, 1, 1};
std::vector<float> v10 = {1, 1, 1, 1};
std::vector<float> v11 = {1, 1, 1, 1};

std::vector<std::vector<float>> layer1weights = {v3, v5, v6, v7};
std::vector<std::vector<float>> layer2weights = {v8, v9};
std::vector<std::vector<std::vector<float>>> bigWeights = {layer1weights,
                                                           layer2weights};

// layer1weights = {v3, v4};

// bigWeights = {layer1weights};

float scaleVal(float parameters[4], float value) {
  float out = (((parameters[3] - parameters[2]) * (value - parameters[0])) /
               (parameters[1] - parameters[0])) +
              parameters[2];

  if (out > 2.5) {
    out = 2.5;
  } else if (out < -2.5) {
    out = -2.5;
  }

  return out;
}

int main() {
  neural_network *myNet = new neural_network;

  std::vector<float> myNetOut = myNet->getOutputs(v2, bigWeights);

  std::cout << "myNetOut: " << myNetOut[0] << " " << myNetOut[0] << std::endl;

}
