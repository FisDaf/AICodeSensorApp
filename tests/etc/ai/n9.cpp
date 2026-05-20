#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // dp[i][j] will store the maximum profit if we distribute the first i problems into j days
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    // prev[i][j] will store the index of the last problem in the j-th day if we distribute the first i problems into j days
    vector<vector<int>> prev(n + 1, vector<int>(k + 1, -1));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= k; ++j) {
            dp[i][j] = dp[i - 1][j];
            for (int l = i; l >= j; --l) {
                if (dp[i][j] < dp[l - 1][j - 1] + a[i - 1]) {
                    dp[i][j] = dp[l - 1][j - 1] + a[i - 1];
                    prev[i][j] = l;
                }
            }
        }
    }

    // Reconstruct the solution
    vector<int> result(k);
    int current_day = k;
    int current_index = n;
    while (current_day > 0 && current_index > 0) {
        int next_index = prev[current_index][current_day];
        result[current_day - 1] = current_index - next_index + 1;
        current_index = next_index - 1;
        --current_day;
    }

    // Output the result
    cout << dp[n][k] << endl;
    for (int i = 0; i < k; ++i) {
        cout << result[i] << " ";
    }
    cout << endl;

    return 0;
}
