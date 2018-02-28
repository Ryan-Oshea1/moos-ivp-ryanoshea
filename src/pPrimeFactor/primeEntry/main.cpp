//main file for testing prime factors class
#include <iostream>
#include "PrimeEntry.h"
using namespace std;

int main(int argc, char *argv[]) {
  PrimeEntry optimus;
  uint64_t bumblebee=atoi(argv[1]);
  optimus.setOriginalVal(bumblebee);

  unsigned int received_index = 1;
  optimus.setReceivedIndex(received_index);

  optimus.setDone(optimus.factor(atoi(argv[2])));

  optimus.setCalculatedIndex(received_index);

  optimus.getReport();
  return 0;
}
