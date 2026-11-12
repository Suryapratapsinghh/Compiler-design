
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

// Check whether a symbol is a non-terminal
bool isNonTerminal(char c) {
    return isupper(static_cast<unsigned char>(c));
}

// Check the operator precedence grammar conditions
bool checkGrammar(const vector<string>& productions) {
    bool valid = true;

    for (string production : productions) {
        size_t pos = production.find('=');

        if (pos == string::npos) {
            cout << "Invalid production format: " << production << endl;
            valid = false;
            continue;
        }

        string rhs = production.substr(pos + 1);
        size_t start = 0;

        while (start <= rhs.size()) {
            size_t end = rhs.find('|', start);
            string alternative = rhs.substr(
                start, end == string::npos ? string::npos : end - start
            );

            if (alternative.empty() || alternative == "#" ||
                alternative == "epsilon" || alternative == "ε") {
                cout << "Epsilon production found: " << alternative << endl;
                valid = false;
            }

            for (int i = 0; i + 1 < (int)alternative.size(); i++) {
                if (isNonTerminal(alternative[i]) &&
                    isNonTerminal(alternative[i + 1])) {
                    cout << "Adjacent non-terminals found in: "
                         << alternative << endl;
                    valid = false;
                }
            }

            if (end == string::npos) break;
            start = end + 1;
        }
    }

    return valid;
}

// Convert id into i and tokenize the expression
vector<string> tokenize(string input) {
    vector<string> tokens;

    for (int i = 0; i < (int)input.size(); i++) {
        if (isspace(static_cast<unsigned char>(input[i])))
            continue;

        if (input.substr(i, 2) == "id") {
            tokens.push_back("i");
            i++;
        }
        else if (input[i] == 'i') {
            tokens.push_back("i");
        }
        else if (input[i] == '+' || input[i] == '*' ||
                 input[i] == '(' || input[i] == ')') {
            tokens.push_back(string(1, input[i]));
        }
        else {
            cout << "Invalid character: " << input[i] << endl;
            return {};
        }
    }

    tokens.push_back("$");
    return tokens;
}

// Return the nearest terminal from the top of the stack
string topTerminal(const vector<string>& st) {
    for (int i = (int)st.size() - 1; i >= 0; i--) {
        if (st[i] != "E")
            return st[i];
    }
    return "";
}

// Operator precedence relation table
char relation(string a, string b) {
    if (a == "+" && b == "+") return '>';
    if (a == "+" && b == "*") return '<';
    if (a == "+" && b == "(") return '<';
    if (a == "+" && b == ")") return '>';
    if (a == "+" && b == "i") return '<';
    if (a == "+" && b == "$") return '>';

    if (a == "*" && b == "+") return '>';
    if (a == "*" && b == "*") return '>';
    if (a == "*" && b == "(") return '<';
    if (a == "*" && b == ")") return '>';
    if (a == "*" && b == "i") return '<';
    if (a == "*" && b == "$") return '>';

    if (a == "(" && b == "+") return '<';
    if (a == "(" && b == "*") return '<';
    if (a == "(" && b == "(") return '<';
    if (a == "(" && b == ")") return '=';
    if (a == "(" && b == "i") return '<';

    if (a == ")" && b == "+") return '>';
    if (a == ")" && b == "*") return '>';
    if (a == ")" && b == ")") return '>';
    if (a == ")" && b == "$") return '>';

    if (a == "i" && b == "+") return '>';
    if (a == "i" && b == "*") return '>';
    if (a == "i" && b == ")") return '>';
    if (a == "i" && b == "$") return '>';

    if (a == "$" && b == "+") return '<';
    if (a == "$" && b == "*") return '<';
    if (a == "$" && b == "(") return '<';
    if (a == "$" && b == "i") return '<';

    return '?';
}

// Perform one reduction
bool reduceStack(vector<string>& st, string& action) {
    int n = st.size();

    if (n >= 1 && st[n - 1] == "i") {
        st.pop_back();
        st.push_back("E");
        action = "Reduce i to E";
        return true;
    }

    if (n >= 3 && st[n - 3] == "(" &&
        st[n - 2] == "E" && st[n - 1] == ")") {
        st.resize(n - 3);
        st.push_back("E");
        action = "Reduce (E) to E";
        return true;
    }

    if (n >= 3 && st[n - 3] == "E" &&
        st[n - 1] == "E" &&
        (st[n - 2] == "+" || st[n - 2] == "*")) {
        string op = st[n - 2];
        st.resize(n - 3);
        st.push_back("E");
        action = "Reduce E" + op + "E to E";
        return true;
    }

    return false;
}

// Display stack contents
string stackText(const vector<string>& st) {
    string result;
    for (string s : st) result += s;
    return result;
}

// Operator precedence parsing
void parseExpression(string input) {
    vector<string> tokens = tokenize(input);

    if (tokens.empty()) {
        cout << "Invalid expression.\n";
        return;
    }

    vector<string> st = {"$"};
    int index = 0;

    cout << "\nSTACK\t\tINPUT\t\tACTION\n";
    cout << "-----------------------------------------------\n";

    while (true) {
        string a = topTerminal(st);
        string b = tokens[index];

        if (a == "$" && b == "$" &&
            st.size() == 2 && st[1] == "E") {
            cout << stackText(st) << "\t\t$\t\tACCEPT\n";
            cout << "\nExpression is ACCEPTED.\n";
            return;
        }

        char rel = relation(a, b);

        if (rel == '<' || rel == '=') {
            cout << stackText(st) << "\t\t";
            for (int j = index; j < (int)tokens.size(); j++)
                cout << tokens[j];
            cout << "\t\tShift " << b << endl;

            st.push_back(b);
            index++;
        }
        else if (rel == '>') {
            string action;

            if (!reduceStack(st, action)) {
                cout << "\nExpression is REJECTED.\n";
                return;
            }

            cout << stackText(st) << "\t\t";
            for (int j = index; j < (int)tokens.size(); j++)
                cout << tokens[j];
            cout << "\t\t" << action << endl;
        }
        else {
            cout << "\nExpression is REJECTED.\n";
            return;
        }

        if (index >= (int)tokens.size()) {
            cout << "\nExpression is REJECTED.\n";
            return;
        }
    }
}

int main() {
    int n;

    cout << "OPERATOR PRECEDENCE GRAMMAR CHECKER\n";
    cout << "Enter number of productions: ";
    cin >> n;

    vector<string> productions(n);

    cout << "Enter productions (example: E=E+E|E*E|(E)|i):\n";
    for (int i = 0; i < n; i++) {
        cin >> productions[i];
    }

    cout << "\nChecking grammar...\n";

    if (checkGrammar(productions)) {
        cout << "The grammar satisfies the basic "
             << "operator precedence grammar conditions.\n";
    } else {
        cout << "The grammar is NOT an operator precedence grammar.\n";
    }

    cout << "\nEnter an expression to parse (use i or id): ";
    string expression;
    cin >> expression;

    parseExpression(expression);

    return 0;
}
