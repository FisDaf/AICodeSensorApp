 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
const ld PI = acosl(-1);
using namespace std;


int main() {
    vector<ll> ans;
    ll n, rn = 0, end = 1, st = 1;
    cin >> n;
    vector<vector<ll>> dp(61, vector<ll>(10));
    for (ll i = 1; i <= 9; i++) dp[1][i] = 1;
    for (ll i = 2; i <= 60; ++i) {
        for (ll j = 1; j <= 9; ++j) {
            for (ll k = j; k <= 9; k++) {
                dp[i][j] += dp[i - 1][k];
            }
        }
    }
    while (end != 0) {
        bool gg = false;
        for (ll i = end; i <= 60 && !gg; i++) {
            for (ll j = st; j <= 9 && !gg; j++) {
                rn += dp[i][j];
                if (rn - dp[i][j] < n && rn >= n) {
                    rn -= dp[i][j];
                    ans.push_back(j);
                    end = i - 1, st = j;
                    gg = true;
                }
            }
        }
    }
    for (auto el: ans) cout << el;
}
