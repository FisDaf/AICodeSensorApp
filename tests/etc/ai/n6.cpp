#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> segments(m);
    for (int i = 0; i < m; ++i) {
        cin >> segments[i].first >> segments[i].second;
    }

    // Initialize the result with all roses
    vector<char> result(n, '0');

    // Try to maximize the beauty for each segment
    for (int i = 0; i < m; ++i) {
        for (int j = segments[i].first; j <= segments[i].second; ++j) {
            if (result[j - 1] == '0') {
                result[j - 1] = '1'; // Try to put a lily in this position
            } else {
                result[j - 1] = '0'; // If already a lily, put a rose
            }
        }
    }

    // Output the result
    for (char c : result) {
        cout << c;
    }
    cout << endl;

    return 0;
}
