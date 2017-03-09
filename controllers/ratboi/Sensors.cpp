#include "Sensors.hpp"

// Sensors::Sensors() {
//   gps = getGPS("gps");
//   gps->enable(TIME_STEP);
//
//   std::string ps = "ps";
//
//   for (unsigned int i = 0; i < 8; i++) {
//     distanceSensors[i] = getDistanceSensor(ps.replace(2, 1, std::to_string(i)));
//     distanceSensors[i]->enable(TIME_STEP);
//   }
//   std::cout << "Constructed" << std::endl;
// }

// oldMin, oldMax, newMin, newMax
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

std::vector<float> getDistanceValues() {
  std::vector<float> current;
  float scaleParam[4] = {50, 1000, -2.5, 2.5};
  float value = 0;

  for (int i = 0; i < 8; i++) {
    value = distanceSensors[i]->getValue();
    std::cout << "value " << value << std::endl;
    float scaledVal = scaleVal(scaleParam, value);
    std::cout << "scaledVal " << scaledVal << std::endl;
    current.push_back(scaledVal);
    value = 0;
  }

  return current;
}

std::vector<float> getGPSValues(){
    std::vector<float> position;
    const double *dataGPS = gps->getValues();

    for (int i = 0; i < 3; i++) {
      position.push_back( (float) dataGPS[i]);
    }

    return position;
}
