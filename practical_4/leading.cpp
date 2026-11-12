
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cctype>
using namespace std;

int main() {
    int n;
    vector<pair<char, string>> productions;
    set<char> nonTerminals;
    set<char> leading[26];

    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter productions (Example: E=E+T):" << endl;

    for (int i = 0; i < n; i++) {
        string p;
        getline(cin, p);

        char lhs = p[0];
        size_t arrow = p.find("->");

        string rhs;

        if (arrow != string::npos)
            rhs = p.substr(arrow + 2);
        else {
            size_t equal = p.find('=');
            if (equal == string::npos) {
                cout << "Invalid production format!" << endl;
                return 0;
            }
            rhs = p.substr(equal + 1);
        }

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
            char lhs = p.first;
            string rhs = p.second;

            if (rhs.empty() || rhs[0] == '#')
                continue;

            int index = lhs - 'A';

            if (!isupper(rhs[0])) {
                if (leading[index].insert(rhs[0]).second)
                    changed = true;
            }
            else {
                char first = rhs[0];
                int firstIndex = first - 'A';

                if (rhs.length() > 1 && !isupper(rhs[1])
                    && rhs[1] != '#') {
                    if (leading[index].insert(rhs[1]).second)
                        changed = true;
                }

                for (char c : leading[firstIndex]) {
                    if (leading[index].insert(c).second)
                        changed = true;
                }
            }
        }
    }

    cout << "\nLEADING sets are:\n";

    for (char nt : nonTerminals) {
        cout << "LEADING(" << nt << ") = { ";

        for (char terminal : leading[nt - 'A'])
            cout << terminal << " ";

        cout << "}" << endl;
    }

    return 0;
}
