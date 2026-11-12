
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
    set<char> follow[26];

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
        string rhs = "";

        for (char c : p.substr(pos + 1)) {
            if (c != ' ' && c != '\t')
                rhs += c;
        }

        size_t start = 0;

        while (start <= rhs.length()) {
            size_t end = rhs.find('|', start);

            string alternative = rhs.substr(
                start,
                end == string::npos ? string::npos : end - start
            );

            productions.push_back({lhs, alternative});

            if (end == string::npos)
                break;

            start = end + 1;
        }

        nonTerminals.insert(lhs);
    }

    // Calculate FIRST sets
    bool changed = true;

    while (changed) {
        changed = false;

        for (auto p : productions) {
            int index = p.lhs - 'A';
            string rhs = p.rhs;

            bool nullable = true;

            for (char symbol : rhs) {
                if (symbol == '#') {
                    if (first[index].insert('#').second)
                        changed = true;
                    break;
                }

                if (!isupper(symbol)) {
                    if (first[index].insert(symbol).second)
                        changed = true;

                    nullable = false;
                    break;
                }

                int si = symbol - 'A';

                for (char t : first[si]) {
                    if (t != '#') {
                        if (first[index].insert(t).second)
                            changed = true;
                    }
                }

                if (first[si].count('#') == 0) {
                    nullable = false;
                    break;
                }
            }

            if (nullable) {
                if (first[index].insert('#').second)
                    changed = true;
            }
        }
    }

    // Add $ to FOLLOW of start symbol
    char startSymbol = productions[0].lhs;
    follow[startSymbol - 'A'].insert('$');

    // Calculate FOLLOW sets
    changed = true;

    while (changed) {
        changed = false;

        for (auto p : productions) {
            char lhs = p.lhs;
            string rhs = p.rhs;

            for (int i = 0; i < (int)rhs.length(); i++) {
                char symbol = rhs[i];

                if (!isupper(symbol))
                    continue;

                int index = symbol - 'A';
                bool nullable = true;

                // Calculate FIRST of remaining symbols
                for (int j = i + 1; j < (int)rhs.length(); j++) {
                    char next = rhs[j];

                    if (next == '#') {
                        continue;
                    }

                    if (!isupper(next)) {
                        if (follow[index].insert(next).second)
                            changed = true;

                        nullable = false;
                        break;
                    }

                    int ni = next - 'A';

                    for (char t : first[ni]) {
                        if (t != '#') {
                            if (follow[index].insert(t).second)
                                changed = true;
                        }
                    }

                    if (first[ni].count('#') == 0) {
                        nullable = false;
                        break;
                    }
                }

                // If remaining symbols can derive epsilon,
                // add FOLLOW of the left-hand side
                if (nullable) {
                    for (char t : follow[lhs - 'A']) {
                        if (follow[index].insert(t).second)
                            changed = true;
                    }
                }
            }
        }
    }

    cout << "\nFOLLOW sets are:\n";

    for (char nt : nonTerminals) {
        cout << "FOLLOW(" << nt << ") = { ";

        for (char t : follow[nt - 'A'])
            cout << t << " ";

        cout << "}" << endl;
    }

    return 0;
}
