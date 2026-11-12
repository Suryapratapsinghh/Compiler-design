
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    string production;

    cout << "Enter production (Example: A=ab|ac|ad): ";
    getline(cin, production);

    // Remove spaces
    string clean = "";
    for (char c : production) {
        if (c != ' ' && c != '\t')
            clean += c;
    }

    size_t pos = clean.find('=');

    if (pos == string::npos || pos == 0) {
        cout << "Invalid production format!" << endl;
        return 0;
    }

    string lhs = clean.substr(0, pos);
    string rhs = clean.substr(pos + 1);

    vector<string> alternatives;

    // Split alternatives
    size_t start = 0;

    while (start <= rhs.length()) {
        size_t end = rhs.find('|', start);

        string part = rhs.substr(
            start,
            end == string::npos ? string::npos : end - start
        );

        if (part.empty()) {
            cout << "Invalid empty alternative!" << endl;
            return 0;
        }

        alternatives.push_back(part);

        if (end == string::npos)
            break;

        start = end + 1;
    }

    // Find longest common prefix
    string prefix = alternatives[0];

    for (int i = 1; i < (int)alternatives.size(); i++) {
        int j = 0;

        while (j < (int)prefix.length() &&
               j < (int)alternatives[i].length() &&
               prefix[j] == alternatives[i][j]) {
            j++;
        }

        prefix = prefix.substr(0, j);
    }

    if (prefix.empty() || alternatives.size() < 2) {
        cout << "\nNo left factoring is required." << endl;
        cout << lhs << " -> " << rhs << endl;
        return 0;
    }

    cout << "\nLeft factoring found!" << endl;
    cout << "\nAfter removing left factoring:\n";

    cout << lhs << " -> " << prefix << lhs << "'" << endl;

    cout << lhs << "' -> ";

    for (int i = 0; i < (int)alternatives.size(); i++) {
        string suffix = alternatives[i].substr(prefix.length());

        if (suffix.empty())
            suffix = "#";

        cout << suffix;

        if (i != (int)alternatives.size() - 1)
            cout << " | ";
    }

    cout << endl;

    return 0;
}
