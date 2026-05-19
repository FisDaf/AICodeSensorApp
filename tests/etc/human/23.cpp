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
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    if (n > m) swap(n, m);
    vector<vector<ll>> dp(m + 1, vector<ll>(1 << n, 0));
    for (ll mask = 0; mask < (1 << n); mask++) dp[1][mask] = 1;
    for (ll len = 1; len < m; len++) {
        for (ll mask = 0; mask < (1 << n); mask++) {
            for (ll next_mask = 0; next_mask < (1 << n); next_mask++) {
                bool f = false;
                for (ll i = 0; i < n - 1; i++) {
                    ll s = (mask >> i & 1)
                           + (mask >> (i + 1) & 1)
                           + (next_mask >> i & 1)
                           + (next_mask >> (i + 1) & 1);
                    f |= (s == 0 || s == 4);
                }
                if (!f) {
                    dp[len + 1][next_mask] += dp[len][mask];
                }
            }
        }
    }
    ll ans = 0;
    for (ll mask = 0; mask < (1 << n); mask++) ans += dp[m][mask];
    cout << ans;
}
