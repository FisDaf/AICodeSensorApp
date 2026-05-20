#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> temperatures(n);
    for (int i = 0; i < n; ++i) {
        cin >> temperatures[i];
    }

    double max_avg = 0.0;
    for (int i = 0; i <= n - k; ++i) {
        for (int j = i + k - 1; j < n; ++j) {
            double sum = 0.0;
            for (int x = i; x <= j; ++x) {
                sum += temperatures[x];
            }
            double avg = sum / (j - i + 1);
            if (avg > max_avg) {
                max_avg = avg;
            }
        }
    }

    cout << fixed << setprecision(15) << max_avg << endl;
    return 0;
}
