#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Node {
    string value;
    vector<Node*> children;

    Node(string v) {
        value = v;
    }
};

string input;
int pos = 0;
bool error = false;

char peek() {
    if (pos < (int)input.length())
        return input[pos];
    return '\0';
}

Node* parseE();
Node* parseEPrime();
Node* parseT();
Node* parseTPrime();
Node* parseF();

Node* parseE() {
    Node* node = new Node("E");

    Node* t = parseT();
    if (!t) return nullptr;

    Node* ep = parseEPrime();
    if (!ep) return nullptr;

    node->children.push_back(t);
    node->children.push_back(ep);

    return node;
}

Node* parseEPrime() {
    Node* node = new Node("E'");

    if (peek() == '+') {
        pos++;
        node->children.push_back(new Node("+"));

        Node* t = parseT();
        if (!t) return nullptr;

        Node* ep = parseEPrime();
        if (!ep) return nullptr;

        node->children.push_back(t);
        node->children.push_back(ep);
    }
    else {
        node->children.push_back(new Node("epsilon"));
    }

    return node;
}

Node* parseT() {
    Node* node = new Node("T");

    Node* f = parseF();
    if (!f) return nullptr;

    Node* tp = parseTPrime();
    if (!tp) return nullptr;

    node->children.push_back(f);
    node->children.push_back(tp);

    return node;
}

Node* parseTPrime() {
    Node* node = new Node("T'");

    if (peek() == '*') {
        pos++;
        node->children.push_back(new Node("*"));

        Node* f = parseF();
        if (!f) return nullptr;

        Node* tp = parseTPrime();
        if (!tp) return nullptr;

        node->children.push_back(f);
        node->children.push_back(tp);
    }
    else {
        node->children.push_back(new Node("epsilon"));
    }

    return node;
}

Node* parseF() {
    Node* node = new Node("F");

    if (peek() == '(') {
        pos++;
        node->children.push_back(new Node("("));

        Node* e = parseE();
        if (!e) return nullptr;

        if (peek() != ')') {
            error = true;
            return nullptr;
        }

        pos++;
        node->children.push_back(e);
        node->children.push_back(new Node(")"));
    }
    else if (input.compare(pos, 2, "id") == 0) {
        pos += 2;
        node->children.push_back(new Node("id"));
    }
    else {
        error = true;
        return nullptr;
    }

    return node;
}

void printTree(Node* node, string prefix = "", bool last = true) {
    if (!node) return;

    cout << prefix;

    if (last)
        cout << "`-- ";
    else
        cout << "|-- ";

    cout << node->value << endl;

    for (int i = 0; i < (int)node->children.size(); i++) {
        printTree(node->children[i],
                  prefix + (last ? "    " : "|   "),
                  i == (int)node->children.size() - 1);
    }
}

int main() {
    cout << "Grammar:" << endl;
    cout << "E  -> T E'" << endl;
    cout << "E' -> + T E' | epsilon" << endl;
    cout << "T  -> F T'" << endl;
    cout << "T' -> * F T' | epsilon" << endl;
    cout << "F  -> (E) | id" << endl;

    cout << "\nEnter an expression (use id for identifiers): ";
    cin >> input;

    Node* root = parseE();

    if (root != nullptr && !error && pos == (int)input.length()) {
        cout << "\nString is successfully parsed." << endl;
        cout << "\nParse Tree:\n";
        printTree(root);
    }
    else {
        cout << "\nString is not valid according to the grammar." << endl;
    }

    return 0;
}
