#include "bits.c"
#include "bits.h"
#include <bitset>
#include <iostream>
using namespace std;

int main() {
  bitset<32> x(~0);
  int y = ~0;
  int high = (y << 5) << 1;
  bitset<32> z(high);
  int low = y << 3;
  bitset<32> w(low);
  //cout << "high mask" <<x << "\n";
  cout << "high mask    "<<z << "\n";
  cout << "low mask     " << w << "\n";
  int answer = (high ^ low);
  bitset<32> a(answer);
  cout << "high ^ low   "<< a << "\n";
  bitset<32> b(answer & low);
  cout << "answer & low "<< b << "\n";
}