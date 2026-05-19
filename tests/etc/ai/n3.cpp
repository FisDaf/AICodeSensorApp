#include <iostream>
#include <string>

int main() {
    int a, b, x;
    std::cin >> a >> b >> x;

    std::string s;
    if (a > b) {
        // Start with a '0' to balance the number of '0's and '1's
        s += '0';
        --a;
        for (int i = 0; i < x; ++i) {
            if (s.back() == '0') {
                s += '1';
                --b;
            } else {
                s += '0';
                --a;
            }
        }
        while (a > 0 || b > 0) {
            if (s.back() == '0') {
                s += '1';
                --b;
            } else {
                s += '0';
                --a;
            }
        }
    } else {
        // Start with a '1' to balance the number of '0's and '1's
        s += '1';
        --b;
        for (int i = 0; i < x; ++i) {
            if (s.back() == '1') {
                s += '0';
                --a;
            } else {
                s += '1';
                --b;
            }
        }
        while (a > 0 || b > 0) {
            if (s.back() == '1') {
                s += '0';
                --a;
            } else {
                s += '1';
                --b;
            }
        }
    }

    std::cout << s << std::endl;
    return 0;
}
