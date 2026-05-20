#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long long
#define ld long double
const ll MOD = (ll) 1e9 + 7;
const ll INF = 1e16;
const ld EPS = 1e-7;

using namespace std;

const ll maxx = 140, maxy = 140, maxz = 140;

ll t[maxx][maxy][maxz];

ll sum(ll x, ll y, ll z) {
    ll result = 0;
    for (ll i = x; i >= 0; i = (i & (i + 1)) - 1)
        for (ll j = y; j >= 0; j = (j & (j + 1)) - 1)
            for (ll k = z; k >= 0; k = (k & (k + 1)) - 1)
                result += t[i][j][k];
    return result;
}

void upd(ll x, ll y, ll z, ll delta) {
    for (ll i = x; i < maxx; i = (i | (i + 1)))
        for (ll j = y; j < maxy; j = (j | (j + 1)))
            for (ll k = z; k < maxz; k = (k | (k + 1)))
                t[i][j][k] += delta;
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    cin >> n;
    ll q;
    cin >> q;
    while (q != 3) {
        if (q == 1) {
            ll x, y, z, k;
            cin >> x >> y >> z >> k;
            upd(x, y, z, k);
        } else {
            ll x, y, z, x0, y0, z0;
            cin >> x0 >> y0 >> z0 >> x >> y >> z;
            ll ans = sum(x, y, z) - sum(x0 - 1, y, z) - sum(x, y0 - 1, z) + sum(x0 - 1, y0 - 1, z) -
                     (sum(x, y, z0 - 1) - sum(x0 - 1, y, z0 - 1) - sum(x, y0 - 1, z0 - 1) +
                      sum(x0 - 1, y0 - 1, z0 - 1));
            cout << ans << '\n';
        }
        cin >> q;
    }
}
