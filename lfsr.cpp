// Libraries:

// Need to replace with AVRlib version:
#include <stdint.h>
#include <time.h>

// For testing:
#include <bitset>
#include <iostream>

/* TODO

  -Test performance of implementation on AVR -- how many assembly instructions
   does it compile to?

  -Probably could use proper constructors, although omitting them saves
   program memory.

  -Omitting classes altogether would also probably improve performance (vtable
   lookups have constant time performance penalty -- may be optimized away
   by the compiler though if no virtual methods exist)

  -In general it's probably better to optimize for time complexity at the
   expense of space -- use LUT's when possible, etc.

  -Might be able to use concatenated 8-bit registers to save stack space

*/

// LSB = least significant bit = [which in this case] is the output bit!
//
//
// Want to generate random sequences of numbers of length:
//   (2^k) - 1 | k is an element of {2, 3, ... , 16}
// For each k, there exists a corresponding feedback polynomial consisting of k bits and
// tap numbers
// unsigned lsb = lfsr & 1u;  /* Get LSB (i.e., the output bit). */
//
/*
 If the tap sequence in an n-bit LFSR is [n, A, B, C, 0], where the 0 corresponds
 to the x^0 = 1 term, then the corresponding "mirror" sequence is
 [n, n − C, n − B, n − A, 0]. So the tap sequence [32, 22, 2, 1, 0] has as its
 counterpart [32, 31, 30, 10, 0]. Both give a maximum-length sequence.

 Taken from: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
*/

/* LFSR form (fibonacci):
     
     Given an unsigned 16 bit integer k, a set of Tap Numbers
     tap_numbers = {A, B, C, D, ... , Z}, and initial lfsr value (lfsr), the output bit,
     and resulting lfsr (lfsr) values are defined as follows:

     output_bit =
       ((lfsr >> (k - A)) ^ (lfsr >> (k - B)) ^ (lfsr >> (k - C)) ^ (lfsr >> (k - D)) ^ ... (lfsr >> (k - Z)) & 1u

     lfsr = (lfsr >> 1) | (bit << (k - 1));
     return lfsr;
*/

class LFSR 
{
  public:

    // Check to make sure this function isn't consuming too much
    // program memory once complete.
      
    uint16_t evaluate()
    {
      // Temporary variable
      uint16_t bit;

      // Handle each bit count as a seperate case.
      switch (size) {

        // Above are the tap numbers which correspond to exponents of primitive polynomials
        // See https://en.wikipedia.org/wiki/Linear-feedback_shift_register

        // This appears to work but should definitely check the math,
        // make sure sequence is being shifted appropriately
         
        case 2:
          // 2 1
          bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 1);
          return lfsr;

        case 3:
          // 3 2
          bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 2);
          return lfsr;

        case 4:
          // 4 3
          bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 3);
          return lfsr;

        case 5:
          // 5 3
          bit = ((lfsr >> 0) ^ (lfsr >> 2)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 4);
          return lfsr;

        case 6:
          // 6 5
          bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 5);
          return lfsr;

        case 7:
          // 7 6
          bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 6);
          return lfsr;

        case 8:
          // 8 6 5 4
          bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 4)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 7);
          return lfsr;

        case 9:
          // 9 5
          bit = ((lfsr >> 0) ^ (lfsr >> 4)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 8);
          return lfsr;

        case 10:
          // 10 7
          bit = ((lfsr >> 0) ^ (lfsr >> 3)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 9);
          return lfsr;

        case 11:
          // 11 9
          bit = ((lfsr >> 0) ^ (lfsr >> 2)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 10);
          return lfsr;

        case 12:
          // 12 11 10 4
          bit = ((lfsr >> 0) ^ (lfsr >> 1) ^ (lfsr >> 2) ^ (lfsr >> 8)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 11);
          return lfsr;

        case 13:
          // 13 12 11 8
          bit = ((lfsr >> 0) ^ (lfsr >> 1) ^ (lfsr >> 2) ^ (lfsr >> 5)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 14);
          return lfsr;

        case 14:
          // 14 13 12 2
          bit = ((lfsr >> 0) ^ (lfsr >> 1) ^ (lfsr >> 2) ^ (lfsr >> 12)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 13);
          return lfsr;

        case 15:
          // 15 14
          bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 14);
          return lfsr;

        case 16:
          // 16 15 13 4
          bit = ((lfsr >> 0) ^ (lfsr >> 1) ^ (lfsr >> 3) ^ (lfsr >> 12)) & 1u;

          lfsr = (lfsr >> 1) | (bit << 15);
          return lfsr;
      }

      return lfsr;
    }

    uint16_t lfsr;

    // This could probably be a template parameter
    uint8_t size;
};

int
main(int argc, char ** argv) {
  
  // Check command line arguments
  if (argc < 2) return -1;

  uint16_t bitsz = std::stoi(argv[1]);

  // Does this allocate space on the stack for the evaluate function code?
  // If so, evaluate should be not be implemented as a class method
  LFSR lfsr;

  // Bit count
  lfsr.size = bitsz;

  // Seed (any nonzero initial value will do according to wiki)
  // - use current system time as seed
  //  We want an unsigned integer between 0 and 2^16 -1 = 65535
  //

  srand(time(NULL));
  int r = rand() % 65535;
  if (r < 0) r *= -1;
  
  // JAT:
  std::cout << "Seed = " << r << "\n";

  lfsr.lfsr = r;

  // Ultimately the best way to test isn't to print output,
  // but test how long the generator runs before repeating itself.
  // it should equal 2^k - 1 for bit count k
  
  for (uint8_t i = 0; i < 20; ++i) {
    std::cout << lfsr.evaluate() << '\n';

    // Print 16-bit binary representation
    // Alter the template parameter for other sequence bit-lengths
    //

    std::cout << std::bitset<16>(lfsr.evaluate()) << '\n';
  }
  return 0;
}
