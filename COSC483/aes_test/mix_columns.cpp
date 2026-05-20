#include <cstdint>
#include <iostream>
using namespace std;

uint8_t xtime(uint8_t x) {
  return (x << 1) ^ ((x & 0x80) ? 0x1b : 0x00);
}

uint8_t ff_multiply(uint8_t a, uint8_t b) {
  uint8_t result = 0;
  uint8_t temp = b;

  for (int i = 0; i < 8; i++) {
    if (a & 0x01) {
      result ^= temp; // Add (XOR) temp to the result if the current bit in a is 1
    }
    temp = xtime(temp); // Multiply temp by x (left shift and reduce)
    // cout<<"intermediate temp: " << hex << +temp << "\n";

    a >>= 1; // Shift a right to process the next bit
    cout << "intermediate result: " << hex << +result << "\n";
  }
  return result;
}

uint8_t mul2(uint8_t value) {
  return (value << 1) ^ ((value & 0x80) ? 0x1b : 0x00);
}

uint8_t mul3(uint8_t value) {
  return mul2(value) ^ value;
}
void mixColumns(uint8_t state[4][4]) {
  uint8_t temp[4];
  for (int c = 0; c < 4; ++c) {
    temp[0] = (uint8_t)(mul2(state[0][c]) ^ mul3(state[1][c]) ^ state[2][c] ^ state[3][c]);
    temp[1] = (uint8_t)(state[0][c] ^ mul2(state[1][c]) ^ mul3(state[2][c]) ^ state[3][c]);
    temp[2] = (uint8_t)(state[0][c] ^ state[1][c] ^ mul2(state[2][c]) ^ mul3(state[3][c]));
    temp[3] = (uint8_t)(mul3(state[0][c]) ^ state[1][c] ^ state[2][c] ^ mul2(state[3][c]));
    for (int i = 0; i < 4; ++i) {
      state[i][c] = temp[i];
    }
  }
}


int main() {
  uint8_t state[4][4] = {
      {0xd4, 0xe0, 0xb8, 0x1e},
      {0xbf, 0xb4, 0x41, 0x27},
      {0x5d, 0x52, 0x11, 0x98},
      {0x30, 0xae, 0xf1, 0xe5}};

  mixColumns(state);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%02x ", state[i][j]);
    }
    printf("\n");
  }
  uint8_t a = 0x83; // Represents x^7 + x + 1
  uint8_t b = 0x57; // Represents x^6 + x^4 + x^2 + x + 1
  uint8_t result = ff_multiply(a, b);
  cout << "Result: 0x" << hex << +result << endl;
  return 0;
  return 0;
}
