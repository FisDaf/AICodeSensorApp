#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int main() {
    int t;
    cin >> t;
    vector<int> a(t);
    for (int i = 0; i < t; ++i) {
        cin >> a[i];
    }

    unordered_map<int, int> freq;
    for (int num : a) {
        freq[num]++;
    }

    int n = 0, m = 0;
    int zero_pos = -1;
    for (const auto& p : freq) {
        if (p.second == 1) {
            zero_pos = p.first;
            break;
        }
    }

    if (zero_pos == -1) {
        cout << -1 << endl;
        return 0;
    }

    for (const auto& p : freq) {
        if (p.second == 1) {
            n = 1;
            m = t;
            break;
        } else if (p.second == 2) {
            n = 2;
            m = t / 2;
            break;
        } else if (p.second == 3) {
            n = 3;
            m = t / 3;
            if (t % 3 == 0) {
                m = t / 3;
            } else if (t % 3 == 1) {
                n = 4;
                m = (t + 2) / 3;
            } else if (t % 3 == 2) {
                n = 3;
                m = (t + 1) / 3;
            }
            break;
        }
    }

    cout << n << " " << m << endl;
    cout << (find(a.begin(), a.end(), zero_pos) - a.begin()) / m + 1 << " " << ((find(a.begin(), a.end(), zero_pos) - a.begin()) % m) + 1 << endl;

    return 0;
}
