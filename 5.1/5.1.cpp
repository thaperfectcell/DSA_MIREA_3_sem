#include <iostream>

using namespace std;

int main()
{
    unsigned char x = 255; // 8-разрядное двоичное число 11111111 (255)
    unsigned char mask = 1; // 1=00000001 – 8-разрядная маска
  //x &= (~(mask << 4)); // 5 бит в 0
  //cout << (int)x << endl; // должно быть 255 - 16 = 239

  //x = 17; // 8-разрядное двоичное число 00010001 (17)
  //x &= (~(mask << 4)); // 5 бит в 0
  //cout << (int)x << endl; // должно быть 1
    x = 17;
    x |= mask << 6; // 7 бит в 1
    cout << (int)x << endl; // 17 + 64 = 81
    return 0;
}
