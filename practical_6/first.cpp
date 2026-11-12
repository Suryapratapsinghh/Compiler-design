
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cctype>
using namespace std;

struct Production {
    char lhs;
    string rhs;
};

int main() {
    int n;
    vector<Production> productions;
    set<char> nonTerminals;
    set<char> first[26];

    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter productions (Example: E=E+T):" << endl;

    for (int i = 0; i < n; i++) {
        string p;
        getline(cin, p);

        size_t pos = p.find('=');

        if (pos == string::npos || pos == 0) {
            cout << "Invalid production format!" << endl;
            return 0;
        }

        char lhs = p[0];
        string rhs = p.substr(pos + 1);

        string clean = "";

        for (char c : rhs) {
            if (c != ' ' && c != '\t')
                clean += c;
        }

        productions.push_back({lhs, clean});
        nonTerminals.insert(lhs);
    }

    bool changed = true;

    while (changed) {
        changed = false;

        for (auto p : productions) {
            char lhs = p.lhs;
            int lhsIndex = lhs - 'A';

            // Split alternatives using |
            string rhs = p.rhs;
            size_t start = 0;

            while (start <= rhs.length()) {
                size_t end = rhs.find('|', start);

                string alternative = rhs.substr(
                    start,
                    end == string::npos ? string::npos : end - start
                );

                bool nullable = true;

                for (char symbol : alternative) {

                    if (symbol == '#') {
                        if (first[lhsIndex].insert('#').second)
                            changed = true;
                        break;
                    }

                    if (!isupper(symbol)) {
                        if (first[lhsIndex].insert(symbol).second)
                            changed = true;

                        nullable = false;
                        break;
                    }

                    int index = symbol - 'A';

                    for (char terminal : first[index]) {
                        if (terminal != '#') {
                            if (first[lhsIndex].insert(terminal).second)
                                changed = true;
                        }
                    }

                    if (first[index].count('#') == 0) {
                        nullable = false;
                        break;
                    }
                }

                if (nullable) {
                    if (first[lhsIndex].insert('#').second)
                        changed = true;
                }

                if (end == string::npos)
                    break;

                start = end + 1;
            }
        }
    }

    cout << "\nFIRST sets are:\n";

    for (char nt : nonTerminals) {
        cout << "FIRST(" << nt << ") = { ";

        for (char terminal : first[nt - 'A'])
            cout << terminal << " ";

        cout << "}" << endl;
    }

    return 0;
}
