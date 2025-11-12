#include <iostream>
#include <bitset>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

int main() {
    unsigned int start = clock();
    ifstream in("input.txt");
    ofstream out("output.txt");
    vector<unsigned char> bitArray  {0};
    string line;

    while (getline(in, line)) {
        unsigned long long i = stoi(line);
        if (bitArray.size() < i / 8 + 1) {
            int size = bitArray.size();
            for (int j = 0; j < i / 8 + 1 - size; j++) {
                bitArray.push_back(0);
            }
        }

        bitArray[i / 8] |= (1 << (7 - (i % 8)));

    }

    for (int i = 0; i < bitArray.size(); i++) {
        for (int j = 0; j < 8; j++) {
            if (bitArray[i] & (1 << (7 - j))) {
                out << i * 8 + j << endl;
            }
        }
    }
    cout << (clock() - start) / 1000.0 << " seconds" << endl;
    cout << sizeof(unsigned char) * bitArray.size() / 1024.0 / 1024.0 << " MByte";


    return 0;
}
