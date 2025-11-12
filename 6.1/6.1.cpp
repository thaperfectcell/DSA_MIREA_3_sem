#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int INITIAL_SIZE = 10;
const double LOAD_FACTOR_THRESHOLD = 0.7;

struct UniversitySpecialization {
    string code;
    string name;

    UniversitySpecialization(string c = "", string n = "") : code(c), name(n) {}
    bool isEmpty() const { return code.empty(); }
    bool isDeleted() const { return code == "DELETED"; }
};

class HashTable {
private:
    vector<UniversitySpecialization> table;
    int size;
    int capacity;

    unsigned short getCodeValue(const string& code) {
        unsigned short value = 0;
        for (char c : code) {
            if (c >= '0' && c <= '9') {
                value = value * 10 + (c - '0');
            }
        }
        return value;
    }

    int hash1(unsigned short code) { return code % capacity; }
    int hash2(unsigned short code) { return code % (capacity - 1) + 1; }

    void rehash() {
        int oldCapacity = capacity;
        capacity *= 2;
        vector<UniversitySpecialization> newTable(capacity);

        for (const auto& item : table) {
            if (!item.isEmpty() && !item.isDeleted()) {
                unsigned short codeValue = getCodeValue(item.code);
                int index = hash1(codeValue);
                int step = hash2(codeValue);

                while (!newTable[index].isEmpty()) {
                    index = (index + step) % capacity;
                }
                newTable[index] = item;
            }
        }
        table = newTable;
        cout << "Table expanded. New size: " << capacity << endl;
    }

public:
    HashTable(int cap = INITIAL_SIZE) : size(0), capacity(cap) {
        table.resize(capacity);
        autoFill();
    }

    void autoFill() {
        insert("09.03.01", "MIREA");
        insert("09.03.02", "SPbSU");
        insert("09.03.03", "NSU");
        insert("09.03.04", "MIPT");
        insert("01.03.02", "MIFI");
        insert("38.03.05", "HSE");
        insert("27.03.04", "MSU");
    }

    void insert(string code, string name) {
        if (static_cast<double>(size) / capacity >= LOAD_FACTOR_THRESHOLD) {
            rehash();
        }

        unsigned short codeValue = getCodeValue(code);
        int index = hash1(codeValue);
        int step = hash2(codeValue);
        int startIndex = index;

        while (!table[index].isEmpty() && !table[index].isDeleted()) {
            if (table[index].code == code) {
                cout << "Record with code " << code << " already exists" << endl;
                return;
            }
            index = (index + step) % capacity;
            if (index == startIndex) {
                cout << "Table is full" << endl;
                return;
            }
        }

        table[index] = UniversitySpecialization(code, name);
        size++;
        cout << "Added: " << code << " - " << name << " (index: " << index << ")" << endl;
    }

    void search(string code) {
        unsigned short codeValue = getCodeValue(code);
        int index = hash1(codeValue);
        int step = hash2(codeValue);
        int startIndex = index;

        while (!table[index].isEmpty()) {
            if (!table[index].isDeleted() && table[index].code == code) {
                cout << "Found: " << code << " - " << table[index].name << endl;
                return;
            }
            index = (index + step) % capacity;
            if (index == startIndex) break;
        }
        cout << "Record with code " << code << " not found" << endl;
    }

    void remove(string code) {
        unsigned short codeValue = getCodeValue(code);
        int index = hash1(codeValue);
        int step = hash2(codeValue);
        int startIndex = index;

        while (!table[index].isEmpty()) {
            if (!table[index].isDeleted() && table[index].code == code) {
                table[index].code = "DELETED";
                table[index].name = "";
                size--;
                cout << "Record with code " << code << " deleted" << endl;
                return;
            }
            index = (index + step) % capacity;
            if (index == startIndex) break;
        }
        cout << "Record with code " << code << " not found" << endl;
    }

    void display() {
        cout << "Hash table (" << size << "/" << capacity << "):" << endl;
        for (int i = 0; i < capacity; i++) {
            cout << "[" << i << "]: ";
            if (table[i].isEmpty()) cout << "Empty";
            else if (table[i].isDeleted()) cout << "Deleted";
            else cout << table[i].code << " - " << table[i].name;
            cout << endl;
        }
    }
};

void ui(HashTable& ht) {
    int choice;
    cout << endl;
    cout << "Commands:" << endl;
    cout << "1. Show table" << endl;
    cout << "2. Find record" << endl;
    cout << "3. Add record" << endl;
    cout << "4. Delete record" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter command: ";
    cin >> choice;
    cin.ignore();
    cout << endl;

    switch (choice) {
        case 1:
            ht.display();
            break;
        case 2: {
            string code;
            cout << "Enter specialty code: ";
            getline(cin, code);
            ht.search(code);
            break;
        }
        case 3: {
            string code, name;
            cout << "Enter specialty code: ";
            getline(cin, code);
            cout << "Enter university name: ";
            getline(cin, name);
            ht.insert(code, name);
            break;
        }
        case 4: {
            string code;
            cout << "Enter code to delete: ";
            getline(cin, code);
            ht.remove(code);
            break;
        }
        case 5:
            exit(0);
        default:
            cout << "Invalid command!" << endl;
    }
}

int main() {
    HashTable ht;
    while (true) {
        ui(ht);
    }
    return 0;
}
