#include <iostream>
#include <bitset>
#include <vector>
using namespace std;

int main() {
    vector<unsigned char> bitArray  {0};
    int count;

    cout << "How much numbers? ";
    cin >> count;


    for (int i = 0; i < count; i++) {
        int num;
        cout << "Enter your number" << endl;
        cin >> num;
        if (bitArray.size() < num / 8 + 1) {
            int size = bitArray.size();
            for (int j = 0; j < num / 8 + 1 - size; j++) {
                bitArray.push_back(0);
            }
        }

        bitArray[num / 8] |= (1 << (7 - (num % 8)));
    }

    cout << "bitArray: ";
    for (int i = 0; i < bitArray.size(); i++) {
        cout << bitset<8>(bitArray[i]) << " ";
    }
    cout << endl;

    cout << "Sorted: ";
    for (int i = 0; i < bitArray.size(); i++) {
        for (int j = 0; j < 8; j++) {
            if (bitArray[i] & (1 << (7 - j))) {
                cout << i * 8 + j << " ";
            }
        }
    }

    return 0;
}
