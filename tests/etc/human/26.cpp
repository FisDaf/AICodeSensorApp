 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, cost;
    cin >> n;
    if (n == 1) {
        cin >> cost;
        cout << 0 << "\n" << 1;
        exit(0);
    }
    vector<unordered_set<ll>> g(n);
    vector<vector<ll>> d(n, vector<ll>(n, INF));
    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < n; j++) {
            cin >> cost;
            if (cost != 0 && i != j) {
                g[i].insert(j);
                d[i][j] = cost;
            }
        }
    }
    ll maxn = (1 << n);
    vector<vector<ll>> dp(maxn, vector<ll>(n, INF));
    dp[1][0] = 0;
    for (ll mask = 1; mask < maxn; ++mask) {
        for (ll i = 0; i < n; i++) {
            if (dp[mask][i] < INF) {
                for (auto j : g[i]) {
                    if (!(mask & (1 << j))) {
                        dp[mask | (1 << j)][j] = min(dp[mask | (1 << j)][j], dp[mask][i] + d[i][j]);
                    }
                }
            }
        }
    }
    ll min_len = INF, end = 1;
    for (ll i = 1; i < n; i++) {
        if (dp[(1 << n) - 1][i] < min_len) {
            min_len = dp[(1 << n) - 1][i];
            end = i;
        }
    }

    if (min_len < INF) {
        cout << min_len << "\n";
        vector<ll> path{end};
        ll i = end;
        ll mask = (1 << n) - 1;
        while (mask > 1) {
            for (ll j = 0; j < n; j++) {
                if (d[j][i] < INF && (mask >> j & 1)) {
                    if (dp[mask][i] == dp[mask - (1 << i)][j] + d[j][i]) {
                        mask -= (1 << i);
                        i = j;
                        path.push_back(i);
                    }
                }
            }
        }
        reverse(path.begin(), path.end());
        for (auto el : path) {
            cout << el + 1 << ' ';
        }
    } else {
        cout << -1 << ' ';
    }
}
