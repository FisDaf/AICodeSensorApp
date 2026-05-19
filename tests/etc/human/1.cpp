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


vector<pair<ll, ll>> tree(2 * sz - 1, {-INF, -1});

void upd(pair<ll, ll> v, ll x = 0, ll lx = 0, ll rx = sz) {
    if (rx - lx == 1)[[unlikely]] {
        tree[x] = v;
    } else[[likely]] {
        ll mx = (lx + rx) >> 1;
        if (v.second < mx) {
            upd(v, 2 * x + 1, lx, mx);
        } else {
            upd(v, 2 * x + 2, mx, rx);
        }
        tree[x] = max(tree[2 * x + 1], tree[2 * x + 2]);
    }
}

pair<ll, ll> ask(ll l, ll r, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return {-INF, -1};
    if (lx >= l && rx <= r) return tree[x];
    ll mx = (lx + rx) >> 1;
    return max(ask(l, r, 2 * x + 1, lx, mx), ask(l, r, 2 * x + 2, mx, rx));
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, el;
    cin >> n;
    for (ll i = 0; i < n; i++) cin >> el, upd({el, i});
    ll q;
    cin >> q;
    while (q--) {
        ll l, r;
        cin >> l >> r;
        cout << ask(l - 1, r).second + 1 << '\n';
    }
}
