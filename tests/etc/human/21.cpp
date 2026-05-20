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
    ll n, m;
    cin >> n;
    vector<ll> a(n);
    for (auto &el: a) cin >> el;
    cin >> m;
    vector<ll> b(m);
    for (auto &el: b) cin >> el;
    vector<vector<ll>> dp(n + 1, vector<ll>(m + 1, 0));
    for (ll i = 1; i <= n; i++) {
        for (ll j = 1; j <= m; j++) {
            dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
            if (a[i - 1] == b[j - 1]) dp[i][j] = max(dp[i - 1][j - 1] + 1, dp[i][j]);
        }
    }
    cout << dp[n][m];
}
