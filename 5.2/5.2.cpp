#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdint>
#include <cstring>

using namespace std;

const string CITIES[] = {
    "Москва", "СПетербург", "Новосибирск", "Екатеринбург", "Казань",
    "ННовгород", "Челябинск", "Самара", "Омск", "РостовНаДону",
    "Уфа", "Красноярск", "Воронеж", "Пермь", "Волгоград"
};
const int N = 10000;
const int NUM_CITIES = 15;

struct TrainRecord {
    uint32_t trainNum;
    char departurePoint[35];
    char destinationPoint[35];
    uint32_t departureTime;

    bool operator<(const TrainRecord& other) const {
        return trainNum < other.trainNum;
    }
};

void createTextFile() {
    ofstream out("trains.txt");
    random_device rd;
    mt19937 gen(rd());

    vector<uint32_t> trainNumbers;
    for (uint32_t i = 1; i <= N; i++) {
        trainNumbers.push_back(i);
    }
    shuffle(trainNumbers.begin(), trainNumbers.end(), gen);

    for (uint32_t i = 0; i < N; i++) {
        string depCity = CITIES[gen() % NUM_CITIES];
        string destCity = CITIES[gen() % NUM_CITIES];

        while (destCity == depCity) {
            destCity = CITIES[gen() % NUM_CITIES];
        }

        uint32_t time = gen() % 1440;

        out << trainNumbers[i] << "|" << depCity << "|" << destCity << "|" << time << endl;
    }

    out.close();
    cout << "txt created with " << N << " records" << endl;
}

void convertToBinary() {
    ifstream in("trains.txt");
    vector<TrainRecord> records;

    string line;
    while (getline(in, line)) {
        TrainRecord record;
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);

        record.trainNum = stoul(line.substr(0, pos1));

        string depCity = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string destCity = line.substr(pos2 + 1, pos3 - pos2 - 1);

        strcpy(record.departurePoint, depCity.c_str());
        strcpy(record.destinationPoint, destCity.c_str());

        record.departureTime = stoul(line.substr(pos3 + 1));

        records.push_back(record);
    }
    in.close();
    // РАСКОММЕНТИРЙУ
    sort(records.begin(), records.end());

    ofstream out("trains.bin", ios::binary);
    for (const auto& record : records) {
        out.write(reinterpret_cast<const char*>(&record), sizeof(TrainRecord));
    }
    out.close();

    cout << "bin created" << endl;
}

int linearSearch(uint32_t trainNumber) {
    ifstream file("trains.bin", ios::binary);
    if (!file.is_open()) return -1;

    TrainRecord record;
    int position = 0;

    while (file.read(reinterpret_cast<char*>(&record), sizeof(TrainRecord))) {
        if (record.trainNum == trainNumber) {
            file.close();
            return position;
        }
        position++;
    }

    file.close();
    return -1;
}

int binarySearch(uint32_t trainNumber) {
    ifstream file("trains.bin", ios::binary);
    if (!file.is_open()) return -1;

    int left = 0;
    int right = N - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        file.seekg(mid * sizeof(TrainRecord));
        TrainRecord record;
        file.read(reinterpret_cast<char*>(&record), sizeof(TrainRecord));

        if (record.trainNum == trainNumber) {
            file.close();
            return mid;
        } else if (record.trainNum < trainNumber) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    file.close();
    return -1;
}



int main() {
    createTextFile();
    convertToBinary();

    uint32_t trainNumber;
    cout << "trainNum to search: ";
    cin >> trainNumber;

    /*auto start = chrono::steady_clock::now();
    int linearResult = linearSearch(trainNumber);
    auto end = chrono::steady_clock::now();
    cout << "linear search: pos " << linearResult
         << ", time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << endl;
    */
    auto start = chrono::steady_clock::now();
    int binaryResult = binarySearch(trainNumber);
    auto end = chrono::steady_clock::now();
    cout << "binary serach: pos " << binaryResult
         << ", time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << endl;

    return 0;
}
