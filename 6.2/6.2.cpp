#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

void find_words_with_substring(string text, string substring) {
    vector<string> words;
    string word;

    for (char c : text) {
        if (!ispunct(c) && c != ' ') {
            word += c;
        } else if (!word.empty()) {
            if (word.find(substring) != string::npos) {
                words.push_back(word);
            }
            word.clear();
        }
    }
    if (!word.empty() && word.find(substring) != string::npos) {
        words.push_back(word);
    }

    cout << "Words with substring '" << substring << "': ";
    for (const auto& w : words) {
        cout << w << " ";
    }
    if (words.empty()) cout << "not found";
    cout << endl;
}

void task1() {
    cout << "Task 1" << endl;
    cin.ignore();

    string text, substring;
    cout << "Enter text: ";
    getline(cin, text);
    cout << "Enter substring: ";
    cin >> substring;

    find_words_with_substring(text, substring);
}

class BoyerMooreHorspool {
private:
    unordered_map<char, int> bad_char_table;
    string pattern;

    void build_table(const string& pat) {
        pattern = pat;
        int m = pattern.length();
        for (int i = 0; i < m - 1; i++) {
            bad_char_table[pattern[i]] = m - 1 - i;
        }
    }

public:
    int search(const string& text) {
        build_table(pattern);
        int n = text.length();
        int m = pattern.length();

        if (m == 0) return 0;
        if (n < m) return -1;

        int i = m - 1;
        while (i < n) {
            int j = m - 1;
            int k = i;

            while (j >= 0 && text[k] == pattern[j]) {
                j--;
                k--;
            }

            if (j < 0) return i - m + 1;

            char bad_char = text[k];
            i += (bad_char_table.find(bad_char) != bad_char_table.end()) ?
                 bad_char_table[bad_char] : m;
        }

        return -1;
    }

    bool is_palindrome(const string& s) {
        string reversed = s;
        reverse(reversed.begin(), reversed.end());
        build_table(s);
        return search(reversed) == 0;
    }
};

string find_max_palindrome(const string& s) {
    BoyerMooreHorspool bmh;
    string max_palindrome = "";

    for (int length = s.length(); length >= 1; length--) {
        for (int i = 0; i <= s.length() - length; i++) {
            string substr = s.substr(i, length);

            if (bmh.is_palindrome(substr)) {
                if (substr.length() > max_palindrome.length()) {
                    max_palindrome = substr;
                }

                if (max_palindrome.length() == s.length()) {
                    return max_palindrome;
                }
            }
        }

        if (!max_palindrome.empty()) {
            return max_palindrome;
        }
    }

    return string(1, s[0]);
}

void task2() {
    cout << "Task 2" << endl;
    cin.ignore();

    string text;
    cout << "Enter text to find max palindrome: ";
    getline(cin, text);

    string palindrome = find_max_palindrome(text);
    cout << "Max palindrome: " << palindrome << endl;
}

int main() {
    int choice;
    cout << "Enter task (1 or 2): ";
    cin >> choice;

    if (choice == 1) task1();
    else if (choice == 2) task2();
    else cout << "Wrong task number" << endl;

    return 0;
}
