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

const ll sz = 1 << 17;

struct node {
    ll s = 0, mn = INF, mx = 0, p = 0;

    ll get_sum(ll lx, ll rx) { return s + p * (rx - lx); }

    ll get_min() { return mn + p; }

    ll get_max() { return mx + p; }
};

node tree[2 * sz - 1];

void push(ll x) {
    tree[2 * x + 1].p += tree[x].p;
    tree[2 * x + 2].p += tree[x].p;
    tree[x].p = 0;
}

void relax(ll x, ll lx, ll rx) {
    ll mx = (rx + lx) >> 1;
    tree[x].s = tree[2 * x + 1].get_sum(lx, mx) + tree[2 * x + 2].get_sum(mx, rx);
    tree[x].mn = min(tree[2 * x + 1].get_min(), tree[2 * x + 2].get_min());
    tree[x].mx = max(tree[2 * x + 1].get_max(), tree[2 * x + 2].get_max());
}

void mod(ll l, ll r, ll md, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l || tree[x].get_max() < md) return;
    if (lx >= l && rx <= r && (tree[x].get_max() / md) == (tree[x].get_min() / md)) {
        tree[x].p -= (tree[x].get_max() / md) * md;
        return;
    }
    push(x);
    ll mx = (lx + rx) >> 1;
    mod(l, r, md, 2 * x + 1, lx, mx);
    mod(l, r, md, 2 * x + 2, mx, rx);
    relax(x, lx, rx);
}


void upd(ll i, ll v, ll x = 0, ll lx = 0, ll rx = sz) {
    if (rx - lx == 1) {
        tree[x].s = tree[x].mx = tree[x].mn = v;
        tree[x].p = 0;
        return;
    }
    ll mx = (lx + rx) >> 1;
    push(x);
    if (i < mx) {
        upd(i, v, 2 * x + 1, lx, mx);
    } else {
        upd(i, v, 2 * x + 2, mx, rx);
    }
    relax(x, lx, rx);
}

ll ask(ll l, ll r, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return 0;
    if (lx >= l && rx <= r) return tree[x].get_sum(lx, rx);
    ll mx = (lx + rx) >> 1;
    push(x);
    relax(x, lx, rx);
    return ask(l, r, 2 * x + 1, lx, mx) + ask(l, r, 2 * x + 2, mx, rx);
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n;
    for (ll i = 0; i < n; i++) {
        ll v;
        cin >> v;
        upd(i, v);
    }
    cin >> m;
    while (m--) {
        ll t, l, r, i, x;
        cin >> t;
        if (t == 1) {
            cin >> i >> x;
            upd(i - 1, x);
        } else if (t == 2) {
            cin >> l >> r >> x;
            mod(l - 1, r, x);
        } else {
            cin >> l >> r;
            cout << ask(l - 1, r) << '\n';

        }
    }
}
