//main file for testing prime factors class
#include <iostream>
#include "PrimeEntry.h"
using namespace std;

int main() {
  PrimeEntry optimus;
  uint64_t bumblebee=10000;
  optimus.setOriginalVal(bumblebee);

  unsigned int received_index = 1;
  optimus.setReceivedIndex(received_index);

  optimus.setDone(optimus.factor(100));

  optimus.getReport();
  return 0;
}
