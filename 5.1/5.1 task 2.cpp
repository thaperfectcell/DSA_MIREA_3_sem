#include <iostream>
#include <bitset>
using namespace std;

int main() {
    //const int maxNum = 8;
    const int maxNum = 64;
    unsigned long long bitArray = 0;
    int count;

    cout << "How much numbers? ";
    cin >> count;
    if (count > maxNum) {
        cout << "Oops, too much";
        return 1;
    }


    for (int i = 0; i < count; i++) {
        int num;
        cout << "Enter your number(0-63)" << endl;
        cin >> num;
        if (num > maxNum - 1) {
            cout << "Sorry, can't do, now your number is 7" << endl;
            num = 7;
        }
        bitArray |= (1ULL << (maxNum - 1 - num));
    }

    cout << "bitArray: " << bitset<64>(bitArray) << endl;

    cout << "Sorted: ";
    for (int i = 0; i < maxNum; i++) {
        if (bitArray & (1ULL << (maxNum - 1 - i))) {
            cout << i << " ";
        }
    }
    cout << endl;

    return 0;
}
