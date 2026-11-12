
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
    set<char> trailing[26];

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
            int last = rhs.length() - 1;

            if (!isupper(rhs[last])) {
                if (trailing[index].insert(rhs[last]).second)
                    changed = true;
            }
            else {
                char lastNT = rhs[last];
                int lastIndex = lastNT - 'A';

                if (last > 0 && !isupper(rhs[last - 1])
                    && rhs[last - 1] != '#') {
                    if (trailing[index].insert(rhs[last - 1]).second)
                        changed = true;
                }

                for (char c : trailing[lastIndex]) {
                    if (trailing[index].insert(c).second)
                        changed = true;
                }
            }
        }
    }

    cout << "\nTRAILING sets are:\n";

    for (char nt : nonTerminals) {
        cout << "TRAILING(" << nt << ") = { ";

        for (char terminal : trailing[nt - 'A'])
            cout << terminal << " ";

        cout << "}" << endl;
    }

    return 0;
}
