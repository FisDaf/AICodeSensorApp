#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long long
#define ld long double
const ll MOD = (ll) 1e9 + 7;
const ll INF = 1e16;
const ld EPS = 1e-7;
const ll sz = 524288;
using namespace std;

struct vert {
    ll pref = 0, suf = 0, mx = 0;
};

vert tree[2 * sz - 1];

vert relax(ll x, ll lx, ll rx, vert v1, vert v2) {
    vert v;
    ll mx = (rx + lx) >> 1;
    v.pref = v1.pref;
    v.suf = v2.suf;
    v.mx = max(v1.mx, v2.mx);
    if (mx - lx == v.pref) v.pref += v2.pref;
    if (rx - mx == v.suf) v.suf += v1.suf;
    v.mx = max(v.mx, v.pref);
    v.mx = max(v.mx, v.suf);
    v.mx = max(v.mx, v1.suf + v2.pref);
    return v;
}

void upd(ll i, ll v, ll x = 0, ll lx = 0, ll rx = sz) {
    if (rx - lx == 1) {
        tree[x].mx = tree[x].suf = tree[x].pref = v == 0;
    } else {
        ll mx = (rx + lx) >> 1;
        if (i < mx) {
            upd(i, v, 2 * x + 1, lx, mx);
        } else {
            upd(i, v, 2 * x + 2, mx, rx);
        }
        tree[x] = relax(x, lx, rx, tree[2 * x + 1], tree[2 * x + 2]);
    }
}

vert ask(ll l, ll r, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return {0, 0, 0};
    if (lx >= l && rx <= r) return tree[x];
    ll mx = (lx + rx) >> 1;
    vert left = ask(l, r, 2 * x + 1, lx, mx);
    vert right = ask(l, r, 2 * x + 2, mx, rx);
    return relax(x, lx, rx, left, right);
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    cin >> n;
    for (ll i = 0; i < n; i++) {
        ll el;
        cin >> el;
        upd(i, el);
    }
    ll q;
    cin >> q;
    while (q--) {
        string s;
        cin >> s;
        if (s[0] == 'Q') {
            ll l, r;
            cin >> l >> r;
            cout << ask(l - 1, r).mx << "\n";
        } else {
            ll i, x;
            cin >> i >> x;
            upd(i - 1, x);
        }
    }
}
