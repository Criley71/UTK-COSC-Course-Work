#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;
uint32_t rcon[] = {0x00000000,
                   0x01000000, 0x02000000, 0x04000000, 0x08000000,
                   0x10000000, 0x20000000, 0x40000000, 0x80000000,
                   0x1B000000, 0x36000000, 0x6C000000, 0xD8000000,
                   0xAB000000, 0x4D000000, 0x9A000000, 0x2F000000,
                   0x5E000000, 0xBC000000, 0x63000000, 0xC6000000,
                   0x97000000, 0x35000000, 0x6A000000, 0xD4000000,
                   0xB3000000, 0x7D000000, 0xFA000000, 0xEF000000,
                   0xC5000000, 0x91000000, 0x39000000, 0x72000000,
                   0xE4000000, 0xD3000000, 0xBD000000, 0x61000000,
                   0xC2000000, 0x9F000000, 0x25000000, 0x4A000000,
                   0x94000000, 0x33000000, 0x66000000, 0xCC000000,
                   0x83000000, 0x1D000000, 0x3A000000, 0x74000000,
                   0xE8000000, 0xCB000000, 0x8D000000};
uint8_t Sbox[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}};
uint8_t InvSbox[16][16] = {
    {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
    {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
    {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
    {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
    {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
    {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
    {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
    {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
    {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
    {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
    {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
    {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
    {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
    {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
    {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}};

uint8_t ffAdd(uint8_t a, uint8_t b) { // not even used because its just xor
  return a ^ b;
}

uint8_t xtime(uint8_t a) { // use for gallois finite field multiplication mainly in mixColumns
  uint8_t b = (a << 1) ^ ((a & 0x80) ? 0x1B : 0x00);//will remove overflow
  return b;
}

uint8_t ffMultiply(uint8_t a, uint8_t b) {//finite field multiplication
  uint8_t result = 0;
  uint8_t temp = b;
  for (int i = 0; i < 8; i++) {
    if (a & 0x01) {
      result = result ^ temp;
    }
    temp = xtime(temp);
    a = a >> 1;
  }
  return result;
}

uint32_t subWord(uint32_t word) { //gets coordinates from words bytes and replaces with s-box value
  uint32_t result = 0;
  for (int i = 0; i < 4; i++) {
    uint8_t byte_of_word = (word >> (24 - 8 * i)) & 0xff; // get each byte by shifting and masking
    uint8_t row = (byte_of_word >> 4) & 0x0f;
    uint8_t col = (byte_of_word & 0x0f);
    uint8_t sub_byte = Sbox[row][col];
    result = result | sub_byte << (24 - 8 * i);
  }
  return result;
}

uint32_t rotWord(uint32_t word) { //move first 2 bytes to end
  /*word = 0x12345678 << 8 = 34567800
    word = 0x12345678 >> 24= 00000012
                      |(or)= 34567812
  */
  uint32_t result = (word << 8) | (word >> 24);
  return result;
}

void keyExpansion(uint8_t key[], uint32_t w[], int aes) {//key expansion
//takes the key, an array to store the expanded key and what form of aes we are using
  int num_words_per_key; // Nk
  int num_rounds;        // Nr
  int num_columns;       // Nb
  if (aes == 128) {
    num_words_per_key = 4; // Nk
    num_rounds = 10;       // Nr
    num_columns = 4;       // Nb
  } else if (aes == 192) {
    num_words_per_key = 6; // Nk
    num_rounds = 12;       // Nr
    num_columns = 4;       // Nb
  } else if (aes == 256) {
    num_words_per_key = 8; // Nk
    num_rounds = 14;       // Nr
    num_columns = 4;       // Nb
  }
  //gets the first word for the key
  for (int i = 0; i < num_words_per_key; i++) {
    w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8) | (key[4 * i + 3]);
  }
  //calculates remaining words for the expanded key
  //xors each column of the state, w, with a word from the key schedule
  int i = num_words_per_key;
  while (i < (num_columns * (num_rounds + 1))) {
    uint32_t temp = w[i - 1];
    if (i % num_words_per_key == 0) {
      temp = subWord(rotWord(temp)) ^ rcon[(i / num_words_per_key)];
    } else if (num_words_per_key > 6 && i % num_words_per_key == 4) {
      temp = subWord(temp);
    }
    w[i] = (w[i - num_words_per_key] ^ temp);
    i++;
  }
}

void subBytes(uint8_t state[4][4]) {//sbox replacement
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      uint8_t row = (state[i][j] >> 4) & 0x0f;
      uint8_t col = state[i][j] & 0x0f;
      state[i][j] = Sbox[row][col];
    }
  }
}

void shiftRows(uint8_t state[4][4]) { //shifts them rows
  uint8_t temp;
  temp = state[1][0];
  state[1][0] = state[1][1];
  state[1][1] = state[1][2];
  state[1][2] = state[1][3];
  state[1][3] = temp;

  temp = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = temp;
  temp = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = temp;

  temp = state[3][0];
  state[3][0] = state[3][3];
  state[3][3] = state[3][2];
  state[3][2] = state[3][1];
  state[3][1] = temp;
}

void mixColumns(uint8_t state[4][4]) { //mixes them columns
  for (int i = 0; i < 4; i++) {
    uint8_t s0 = state[0][i];
    uint8_t s1 = state[1][i];
    uint8_t s2 = state[2][i];
    uint8_t s3 = state[3][i];
    //call galoise field multiplication
    uint8_t temp0 = xtime(s0) ^ (xtime(s1) ^ s1) ^ s2 ^ s3;
    uint8_t temp1 = s0 ^ xtime(s1) ^ (xtime(s2) ^ s2) ^ s3;
    uint8_t temp2 = s0 ^ s1 ^ xtime(s2) ^ (xtime(s3) ^ s3);
    uint8_t temp3 = (xtime(s0) ^ s0) ^ s1 ^ s2 ^ xtime(s3);

    state[0][i] = temp0;
    state[1][i] = temp1;
    state[2][i] = temp2;
    state[3][i] = temp3;
  }
}

void addRoundKey(uint8_t state[4][4], uint32_t w[], int round) {
  for (int i = 0; i < 4; i++) {
    uint32_t roundKey = w[round + i];
    //round keys added to state
    state[0][i] = state[0][i] ^ (roundKey >> 24) & 0xff;

    state[1][i] = state[1][i] ^ (roundKey >> 16) & 0xff;
    state[2][i] = state[2][i] ^ (roundKey >> 8) & 0xff;
    state[3][i] = state[3][i] ^ roundKey & 0xff;
  }
}

//cipher function
//takes input array, output array, expanded key array, and aes
//goes through rounds depending on key size
void cipher(uint8_t in[], uint8_t out[], uint32_t w[], int aes) {
  int num_words_per_key; // Nk
  int num_rounds;        // Nr
  int num_columns;       // Nb
  if (aes == 128) {
    num_words_per_key = 4; // Nk
    num_rounds = 10;       // Nr
    num_columns = 4;       // Nb
  } else if (aes == 192) {
    num_words_per_key = 6; // Nk
    num_rounds = 12;       // Nr
    num_columns = 4;       // Nb
  } else if (aes == 256) {
    num_words_per_key = 8; // Nk
    num_rounds = 14;       // Nr
    num_columns = 4;       // Nb
  }
  uint8_t state[4][4];

  for (int i = 0; i < 16; i++) {
    state[i % 4][i / 4] = in[i];
  }
  // round[ 0].input
  cout << "round[" << dec << setw(2) << setfill(' ') << 0 << "].input     ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";
  // round[ 0].k_sch
  cout << "round[" << dec << setw(2) << setfill(' ') << 0 << "].k_sch     ";
  cout << hex << setw(8) << setfill('0') << w[0]+0 << hex << setw(8) << setfill('0') << w[1] + 0 << hex << setw(8) << setfill('0') << w[2] + 0 << hex << setw(8) << setfill('0') << w[3] + 0 << "\n";

  addRoundKey(state, w, 0);
  for (int i = 1; i <= num_rounds - 1; i++) {
    // round[i].start
    cout << "round[" << dec << setw(2) << setfill(' ') << i << "].start     ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";

    subBytes(state);
    // round[i].s_box
    cout << "round[" << dec << setw(2) << setfill(' ') << i << "].s_box     ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";

    shiftRows(state);
    // round[i].s_row
    cout << "round[" << dec << setw(2) << setfill(' ') << i << "].s_row     ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";

    mixColumns(state);
    // round[i].m_col
    cout << "round[" << dec << setw(2) << setfill(' ') << i << "].m_col     ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";
    addRoundKey(state, w, i * 4);
    // round[i].k_sch
    cout << "round[" << dec << setw(2) << setfill(' ') << i << "].k_sch     ";
    cout << hex << setw(8) << setfill('0') << w[i * 4] + 0 << hex << setw(8) << setfill('0') << w[(i * 4) + 1] + 0 << hex << setw(8) << setfill('0') << w[(i * 4) + 2] + 0 << hex << setw(8) << setfill('0') << w[(i * 4) + 3] + 0 << "\n";
  }

  // round[nr].start
  cout << "round[" << dec << setw(2) << setfill(' ') << num_rounds << "].start     ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";

  subBytes(state);
  // round[nr].s_box
  cout << "round[" << dec << setw(2) << setfill(' ') << num_rounds << "].s_box     ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";

  shiftRows(state);
  // round[nr].s_row
  cout << "round[" << dec << setw(2) << setfill(' ') << num_rounds << "].s_row     ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";

  addRoundKey(state, w, num_rounds * 4);
  // round[nr].k_sch
  cout << "round[" << dec << setw(2) << setfill(' ') << num_rounds << "].k_sch     ";
  cout << hex << setw(8) << setfill('0') << w[num_rounds * 4] + 0 << hex << setw(8) << setfill('0') << w[(num_rounds * 4) + 1] + 0 << hex << setw(8) << setfill('0') << w[(num_rounds * 4) + 2] + 0 << hex << setw(8) << setfill('0') << w[(num_rounds * 4) + 3] + 0 << "\n";

  // round[nr].output
  cout << "round[" << dec << setw(2) << setfill(' ') << num_rounds << "].output    ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";

  for (int i = 0; i < 16; i++) {
    out[i] = state[i % 4][i / 4];
  }
}
//subBytes function but inverted to decrypt
void invSubBytes(uint8_t state[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      uint8_t row = (state[i][j] >> 4) & 0x0f;
      uint8_t col = state[i][j] & 0x0f;
      state[i][j] = InvSbox[row][col];
    }
  }
}
//shiftRow function but inverted to decrypt
void invShiftRows(uint8_t state[4][4]) {
  uint8_t temp;
  temp = state[1][3];
  state[1][3] = state[1][2];
  state[1][2] = state[1][1];
  state[1][1] = state[1][0];
  state[1][0] = temp;

  temp = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = temp;
  temp = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = temp;

  temp = state[3][0];
  state[3][0] = state[3][1];
  state[3][1] = state[3][2];
  state[3][2] = state[3][3];
  state[3][3] = temp;
}
//mixColumns function but inverted to decrypt
void invMixColumns(uint8_t state[4][4]) {
  for (int i = 0; i < 4; i++) {
    uint8_t s0 = state[0][i];
    uint8_t s1 = state[1][i];
    uint8_t s2 = state[2][i];
    uint8_t s3 = state[3][i];

    state[0][i] = ffMultiply(s0, 0x0e) ^ ffMultiply(s1, 0x0b) ^ ffMultiply(s2, 0x0d) ^ ffMultiply(s3, 0x09);
    state[1][i] = ffMultiply(s0, 0x09) ^ ffMultiply(s1, 0x0e) ^ ffMultiply(s2, 0x0b) ^ ffMultiply(s3, 0x0d);
    state[2][i] = ffMultiply(s0, 0x0d) ^ ffMultiply(s1, 0x09) ^ ffMultiply(s2, 0x0e) ^ ffMultiply(s3, 0x0b);
    state[3][i] = ffMultiply(s0, 0x0b) ^ ffMultiply(s1, 0x0d) ^ ffMultiply(s2, 0x09) ^ ffMultiply(s3, 0x0e);
  }
}
//cipher function but guess what its inverted 
void invCipher(uint8_t in[], uint8_t out[], uint32_t w[], int aes) {
  int num_words_per_key; // Nk
  int num_rounds;        // Nr
  int num_columns;       // Nb
  if (aes == 128) {
    num_words_per_key = 4; // Nk
    num_rounds = 10;       // Nr
    num_columns = 4;       // Nb
  } else if (aes == 192) {
    num_words_per_key = 6; // Nk
    num_rounds = 12;       // Nr
    num_columns = 4;       // Nb
  } else if (aes == 256) {
    num_words_per_key = 8; // Nk
    num_rounds = 14;       // Nr
    num_columns = 4;       // Nb
  }
  uint8_t state[4][4];
  for (int i = 0; i < 16; i++) {
    state[i % 4][i / 4] = in[i];
  }
  // round[0].input
  cout << "round[" << dec << setw(2) << setfill(' ') << 0 << "].iinput    ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";
  cout << "round[" << dec << setw(2) << setfill(' ') << 0 << "].ik_sch    ";
  cout << hex << setw(8) << setfill('0') << w[num_rounds * 4] + 0 << hex << setw(8) << setfill('0') << w[(num_rounds * 4) + 1] + 0 << hex << setw(8) << setfill('0') << w[(num_rounds * 4) + 2] + 0 << hex << setw(8) << setfill('0') << w[(num_rounds * 4) + 3] + 0 << "\n";
  addRoundKey(state, w, num_rounds * 4);

  for (int i = num_rounds - 1; i >= 1; i--) {
    // round[i].istart
    cout << "round[" << dec << setw(2) << setfill(' ') << (i - num_rounds) * -1 << "].istart    ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";

    invShiftRows(state);
    // round[i].is_row
    cout << "round[" << dec << setw(2) << setfill(' ') << (i - num_rounds) * -1 << "].is_row    ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";

    invSubBytes(state);
    // round[i].s_box
    cout << "round[" << dec << setw(2) << setfill(' ') << (i - num_rounds) * -1 << "].is_box    ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";

    addRoundKey(state, w, i * 4);

    // round[i].ik_sch
    cout << "round[" << dec << setw(2) << setfill(' ') << (i - num_rounds) * -1 << "].ik_sch    ";
    cout << hex << setw(8) << setfill('0') << w[i * 4] + 0 << hex << setw(8) << setfill('0') << w[(i * 4) + 1] + 0 << hex << setw(8) << setfill('0') << w[(i * 4) + 2] + 0 << hex << setw(8) << setfill('0') << w[(i * 4) + 3] + 0 << "\n";

    // round[i].ik_add
    cout << "round[" << dec << setw(2) << setfill(' ') << (i - num_rounds) * -1 << "].ik_add    ";
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
      }
    }
    cout << "\n";
    invMixColumns(state);
  }
  // round[nr].istart
  cout << "round[" << dec << setw(2) << setfill(' ') << num_rounds << "].istart    ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";

  invShiftRows(state);
  // round[nr].is_row
  cout << "round[" << dec << setw(2) << setfill(' ') << (num_rounds) << "].is_row    ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";
  invSubBytes(state);
  // round[nr].s_box
  cout << "round[" << dec << setw(2) << setfill(' ') << (num_rounds) << "].is_box    ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";
  addRoundKey(state, w, 0);
  // round[ 0].k_sch
  cout << "round[" << dec << setw(2) << setfill(' ') << num_rounds << "].ik_sch    ";
  cout << hex << setw(8) << setfill('0') << static_cast<int>(w[0]) << hex << setw(8) << setfill('0') << w[1] + 0 << hex << setw(8) << setfill('0') << w[2] + 0 << hex << setw(8) << setfill('0') << w[3] + 0 << "\n";

  for (int i = 0; i < 16; i++) {
    out[i] = state[i % 4][i / 4];
  }
  // round[nr].ioutput
  cout << "round[" << dec << setw(2) << setfill(' ') << (num_rounds) << "].ioutput   ";
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j < 4; j++) {
      cout << hex << setw(2) << setfill('0') << state[j][k] + 0;
    }
  }
  cout << "\n";
}
//quality of life function to convert input text and key to an array of hex
void convertHexStringToArray(string &hexString, uint8_t out[]) {
  for (int i = 0; i < hexString.length(); i += 2) {
    string stringBytes = hexString.substr(i, 2);
    out[i / 2] = static_cast<uint8_t>(strtol(stringBytes.c_str(), nullptr, 16));
  }
}

int main() {

  cout << "C.1   AES-128 (Nk=4, Nr=10)\n\n";
  cout << "PLAINTEXT:          00112233445566778899aabbccddeeff\n";
  cout << "KEY:                000102030405060708090a0b0c0d0e0f\n\n";
  cout << "CIPHER (ENCRYPT):\n";

  uint32_t w[44];
  string p = "00112233445566778899aabbccddeeff";
  uint8_t pArray[16];
  convertHexStringToArray(p, pArray);
  string keyString = "000102030405060708090a0b0c0d0e0f";
  uint8_t kArray[16];
  convertHexStringToArray(keyString, kArray);
  uint8_t output[16];
  keyExpansion(kArray, w, 128);
  cipher(pArray, output, w, 128);

  cout << "\n";
  cout << "INVERSE CIPHER (DECRYPT):\n";
  uint8_t decrypted[16];
  invCipher(output, decrypted, w, 128);

  cout << "\nC.2   AES-192 (Nk=6, Nr=12)\n\n";
  cout << "PLAINTEXT:          00112233445566778899aabbccddeeff\n";
  cout << "KEY:                000102030405060708090a0b0c0d0e0f1011121314151617\n\n";
  cout << "CIPHER (ENCRYPT):\n";
  string f = "00112233445566778899aabbccddeeff";
  uint8_t fArray[16];
  string longKey = "000102030405060708090a0b0c0d0e0f1011121314151617";
  uint8_t keyArray[24];
  uint32_t w2[52];
  convertHexStringToArray(f, fArray);
  convertHexStringToArray(longKey, keyArray);
  keyExpansion(keyArray, w2, 192);
  cipher(fArray, output, w2, 192);
  cout << "\n";
  cout << "INVERSE CIPHER (DECRYPT):\n";
  invCipher(output, decrypted, w2, 192);

  cout << "\nC.3   AES-256 (Nk=8, Nr=14)\n\n";
  cout << "PLAINTEXT:          00112233445566778899aabbccddeeff\n";
  cout << "KEY:                000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f\n\n";
  cout << "CIPHER (ENCRYPT):\n";
  uint32_t w3[60];
  string g = "00112233445566778899aabbccddeeff";
  uint8_t gArray[16];
  convertHexStringToArray(g, gArray);
  string mKey = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f";
  uint8_t massiveKey[32];
  convertHexStringToArray(mKey, massiveKey);
  keyExpansion(massiveKey, w3, 256);
  cipher(gArray, output, w3, 256);
  cout << "\n";
  cout << "INVERSE CIPHER (DECRYPT):\n";
  invCipher(output, decrypted, w3, 256);
}
