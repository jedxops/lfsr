#include <iostream>
using namespace std;

int
main(int argc, char ** argv) {
  if (argc < 2) return -1;
  uint k = stoi(argv[1]);
  /*cout << "k = " << k << "\n";
  cout << "k >> 0 = " << (k >> 0) << "\n";*/

  uint16_t lfsr = 4; // 100
  uint16_t test_bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5));
  cout << "test_bit value = " << test_bit << ".\n";

  uint16_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
  lfsr = (lfsr >> 1) | (bit << 15);
  cout << "LFSR value = " << lfsr << ".\n";
  return 0;
}
