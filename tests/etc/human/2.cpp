#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long long
#define ld long double
const ll MOD = (ll) 1e9 + 7;
const ll INF = 1e16;
const ld EPS = 1e-7;
const ll sz = 131072;
using namespace std;


ll tree[2 * sz - 1] = {};


void upd(ll l, ll r, ll v, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return;
    if (lx >= l && rx <= r) {
        tree[x] += v;
        return;
    }
    ll mx = (rx + lx) >> 1;
    upd(l, r, v, 2 * x + 1, lx, mx);
    upd(l, r, v, 2 * x + 2, mx, rx);
}

ll ask(ll i, ll x = 0, ll lx = 0, ll rx = sz) {
    if (rx - lx == 1) {
        return tree[x];
    } else {
        ll mx = (rx + lx) >> 1;
        if (i < mx) return tree[x] + ask(i, 2 * x + 1, lx, mx);
        return tree[x] + ask(i, 2 * x + 2, mx, rx);
    }
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    cin >> n;
    for (ll i = 0; i < n; i++) cin >> tree[sz + i - 1];
    ll q;
    cin >> q;
    while (q--) {
        char t;
        cin >> t;
        if (t == 'g') {
            ll i;
            cin >> i;
            cout << ask(i - 1) << '\n';
        } else {
            ll l, r, v;
            cin >> l >> r >> v;
            upd(l - 1, r, v);
        }
    }
}
