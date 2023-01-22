#include "TEST.h"

int TEST_CLASS::getOne() {return 1;}
int TEST_CLASS::getTwo() {return 2;}
int TEST_CLASS::Nothing() {return 0;}

void TEST_CLASS::Debug() {
    std::cout << this->getOne() << " " << this->getTwo() << " " << this->Nothing();
}