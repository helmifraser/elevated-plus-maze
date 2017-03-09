#include "GA.hpp"

int main(int argc, char const *argv[]) {

  GA *test = new GA;
  int map[15];
  for (size_t i = 0; i < 15; i++) {
    map [i] = 0;
  }

  float sumPoints = 0;
  std::vector<float> empty = {0, 1, -1, 1, 0, 0.5, -0.5, 2};

  while (1) {
    std::cout << "Enter x, then z" << std::endl;
    float z_coord = 0;
    float x_coord = 0;
    std::cin >> x_coord;
    std::cout << "z" << std::endl;
    std::cin >> z_coord;

    std::vector<float> test_coords = {x_coord, 0, z_coord};

    int currentPos = test->position(test_coords);
    std::cout << "position " << currentPos << std::endl;

    test->updatePosition(map, currentPos);

    sumPoints = test->fitnessEval(currentPos, sumPoints, empty);

    std::cout << "Sum fitness: " << sumPoints << std::endl;

  }

  return 0;
}
