
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int n;

    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        string production;

        cout << "\nEnter production (Example: A=Aa|b): ";
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
            continue;
        }

        string lhs = clean.substr(0, pos);
        string rhs = clean.substr(pos + 1);

        vector<string> alpha, beta;

        // Separate alternatives
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

            if (part.find(lhs) == 0)
                alpha.push_back(part.substr(lhs.length()));
            else
                beta.push_back(part);

            if (end == string::npos)
                break;

            start = end + 1;
        }

        // Check left recursion
        if (alpha.empty()) {
            cout << "\nNo left recursion found in " << lhs << endl;
            cout << lhs << " -> " << rhs << endl;
        }
        else if (beta.empty()) {
            cout << "\nLeft recursion found in " << lhs << endl;
            cout << "Cannot eliminate it using the standard "
                 << "transformation because no beta alternative exists."
                 << endl;
        }
        else {
            cout << "\nLeft recursion found in " << lhs << endl;
            cout << "After eliminating left recursion:\n";

            // Print A -> beta A'
            cout << lhs << " -> ";

            for (int j = 0; j < (int)beta.size(); j++) {
                cout << beta[j] << lhs << "'";

                if (j != (int)beta.size() - 1)
                    cout << " | ";
            }

            cout << endl;

            // Print A' -> alpha A' | epsilon
            cout << lhs << "' -> ";

            for (int j = 0; j < (int)alpha.size(); j++) {
                cout << alpha[j] << lhs << "' | ";
            }

            cout << "#" << endl;
        }
    }

    return 0;
}
