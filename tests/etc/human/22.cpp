 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;
vector<ll> p, dp, ans;
ll n, m;

void get_ans(ll mask, ll col = 1) {
    if (p[mask] == -1) {
        for (ll i = 0; i < n; i++) {
            if (mask >> i & 1) ans[i] = col;
        }
        return;
    }
    ll left = p[mask];
    ll right = mask ^ left;
    get_ans(left, col);
    get_ans(right, dp[left] + col);
}

void solve() {
    cin >> n >> m;
    vector<ll> a(n);
    for (ll i = 0; i < n; i++) a[i] = (1 << i);
    for (ll i = 0; i < m; i++) {
        ll u, v;
        cin >> u >> v;
        u--, v--;
        a[u] |= (1 << v);
        a[v] |= (1 << u);
    }
    dp.assign(1 << n, n + 1);
    p.assign(1 << n, -1);
    ans.assign(n, -1);
    for (ll mask = 0; mask < (1 << n); mask++) {
        bool f = true;
        for (ll i = 0; i < n && f; i++) {
            if (!(mask >> i & 1)) continue;
            for (ll j = i + 1; j < n && f; j++) {
                if ((mask >> j & 1) && (a[i] >> j & 1)) {
                    f = false;
                }
            }
        }
        if (f) dp[mask] = 1;
    }
    dp[0] = 0;
    for (ll mask = 1; mask < (1 << n); mask++) {
        for (ll submask = mask; submask; submask = (submask - 1) & mask) {
            if (dp[mask] > dp[submask] + dp[mask ^ submask]) {
                dp[mask] = dp[submask] + dp[mask ^ submask];
                p[mask] = submask;
            }
        }
    }
    cout << dp[(1 << n) - 1] << '\n';
    get_ans((1 << n) - 1);
    for (auto el: ans) cout << el << ' ';
    cout << '\n';
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    ll t;
    cin >> t;
    while (t--) solve();
}
