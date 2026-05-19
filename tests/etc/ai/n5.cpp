#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int n, d, k;
    cin >> n >> d >> k;

    if (d >= n) {
        cout << "NO" << endl;
        return 0;
    }

    vector<pair<int, int>> edges;

    if (k == 1) {
        if (d != 1) {
            cout << "NO" << endl;
            return 0;
        }
        for (int i = 2; i <= n; ++i) {
            edges.emplace_back(1, i);
        }
    } else {
        vector<int> level(n + 1, 0);
        queue<int> q;

        for (int i = 2; i <= d + 1; ++i) {
            edges.emplace_back(i - 1, i);
            level[i] = level[i - 1] + 1;
            q.push(i);
        }

        int current = d + 1;
        while (!q.empty() && current < n) {
            int u = q.front();
            q.pop();

            int max_new_nodes = min(k - 1, level[u]);
            for (int i = 1; i <= max_new_nodes && current < n; ++i) {
                ++current;
                edges.emplace_back(u, current);
                level[current] = level[u];
                q.push(current);
            }
        }

        if (current < n) {
            cout << "NO" << endl;
            return 0;
        }
    }

    cout << "YES" << endl;
    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }

    return 0;
}
