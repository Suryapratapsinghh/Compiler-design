#include <iostream>
#include <string>
using namespace std;

int main() {
    int inp;
    string str;

    cout << "Enter number 1 for grammar 1" << endl;
    cout << "Enter number 2 for grammar 2" << endl;
    cout << "Enter number 3 for grammar 3" << endl;
    cout << "Enter number 4 for grammar 4" << endl;

    cin >> inp;

    if (inp == 1) {
        cout << "Enter a string: ";
        cin >> str;

        int n = 0;

        while (n < (int)str.length() && str[n] == 'a')
            n++;

        int a = n;

        while (n < (int)str.length() && str[n] == 'b')
            n++;

        int b = n - a;

        if (n == (int)str.length() && a >= 1 && b >= 1)
            cout << "String is valid" << endl;
        else
            cout << "String does not belong to grammar" << endl;
    }

    else if (inp == 2) {
        cout << "Enter a string: ";
        cin >> str;

        bool valid = true;

        for (char ch : str) {
            if (ch != 'a' && ch != 'b')
                valid = false;
        }

        int len = str.length();

        if (valid && len % 2 == 1) {
            int s = 0, e = len - 1;

            while (s < e) {
                if (str[s] != str[e]) {
                    valid = false;
                    break;
                }
                s++;
                e--;
            }
        }
        else {
            valid = false;
        }

        if (valid)
            cout << "String belongs to grammar" << endl;
        else
            cout << "String does not belong to grammar" << endl;
    }

    else if (inp == 3) {
        cout << "Enter a string: ";
        cin >> str;

        int ca = 0, cb = 0, i = 0;

        while (i < (int)str.length() && str[i] == 'a') {
            ca++;
            i++;
        }

        while (i < (int)str.length() && str[i] == 'b') {
            cb++;
            i++;
        }

        if (i == (int)str.length() && ca >= 1 && cb == 2 * ca)
            cout << "String belongs to grammar" << endl;
        else
            cout << "String does not belong to grammar" << endl;
    }

    else if (inp == 4) {
        cout << "Enter a string: ";
        cin >> str;

        int ca = 0, cb = 0, i = 0;

        while (i < (int)str.length() && str[i] == 'a') {
            ca++;
            i++;
        }

        while (i < (int)str.length() && str[i] == 'b') {
            cb++;
            i++;
        }

        if (i == (int)str.length() && ca >= 1 && ca == cb)
            cout << "String belongs to grammar" << endl;
        else
            cout << "String does not belong to grammar" << endl;
    }

    else {
        cout << "Invalid grammar choice" << endl;
    }

    return 0;
}
