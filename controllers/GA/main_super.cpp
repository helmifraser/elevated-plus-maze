#include <math.h>
#include <stdio.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#define TIME_STEP 32

class supervisor : public Supervisor {
private:
  Supervisor *supes;
  Node *clarissa;
  Field *translationField;
  double x;
  double z;
  double translation[3];

public:
  std::string name;

  supervisor() {
    name = getName();
    clarissa = getFromDef(name);
    translationField = clarissa->getField("translation");
    translation[0] = 0;
    translation[2] = 1;
  }

  void run() {
    while (step(timeStep) != -1) {
      translationField->setSFVec3f(translation);
    }
  }
};

int main(int argc, char const *argv[]) {
  Supervisor *test = new Supervisor;
  test->run();
  return 0;
}
