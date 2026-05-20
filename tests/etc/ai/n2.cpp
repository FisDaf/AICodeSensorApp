#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int q;
    cin >> q;
    vector<pair<int, int>> queries(q);
    for (int i = 0; i < q; ++i) {
        cin >> queries[i].first >> queries[i].second;
    }

    // Precompute the first and last occurrence of each number
    vector<unordered_map<int, int>> first_last(n);
    unordered_map<int, int> last_occurrence;
    for (int i = 0; i < n; ++i) {
        if (first_last[i].find(a[i]) == first_last[i].end()) {
            first_last[i][a[i]] = i;
        }
        last_occurrence[a[i]] = i;
    }

    // Process queries
    for (const auto& query : queries) {
        int l = query.first - 1;
        int r = query.second - 1;
        bool found = false;

        for (const auto& entry : first_last[r]) {
            int num = entry.first;
            int last = last_occurrence[num];
            if (first_last[r][num] >= l && last <= r) {
                cout << num << '\n';
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "0\n";
        }
    }

    return 0;
}
