#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long long
#define ld long double
const ll MOD = (ll) 1e9 + 7;
const ll INF = 1e16;
const ld EPS = 1e-7;
const ll maxr = 1e9 + 5;
const ll maxl = -maxr;

using namespace std;

void md(ll &x) {
    x %= MOD;
}

struct segtree {
    segtree *left = nullptr, *right = nullptr;
    pair<ll, ll> v = {0, 0};

    pair<ll, ll> relax(segtree *left, segtree *right) {
        pair<ll, ll> l, r;
        l = r = {0, 0};
        if (left) l = left->v;
        if (right) r = right->v;
        if (l.first == r.first)return {l.first, l.second + r.second};
        if (l.first > r.first) return l;
        return r;
    }

    void upd(ll i, pair<ll, ll> x, ll lx = maxl, ll rx = maxr) {
        if (rx - lx == 1)[[unlikely]] {
            if (v.first < x.first) v = x;
            else v.second += x.second;
            md(v.second);
        } else[[likely]] {
            ll mx = (rx + lx) >> 1;
            if (i < mx) {
                if (!left) left = new segtree;
                left->upd(i, x, lx, mx);
            } else {
                if (!right) right = new segtree;
                right->upd(i, x, mx, rx);
            }
            v = relax(left, right);
            md(v.second);
        }
    }

    pair<ll, ll> ask(ll r, ll lx = maxl, ll rx = maxr) {
        if (lx >= r) return {0, 0};
        if (rx <= r) return v;
        ll mx = (lx + rx) >> 1;
        pair<ll, ll> lf, rt;
        lf = rt = {0, 0};
        if (left) lf = left->ask(r, lx, mx);
        if (right) rt = right->ask(r, mx, rx);
        if (lf.first == rt.first)return {lf.first, lf.second + rt.second};
        if (lf.first > rt.first) return lf;
        return rt;
    }

};

segtree *root = new segtree;

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    cin >> n;
    ll mx = 0;
    ll ans = 0;
    for (ll i = 0; i < n; i++) {
        ll x;
        cin >> x;
        pair<ll, ll> p = root->ask(x);
        p.first++;
        if (p.second == 0)p.second = 1;
        md(p.second);
        if (p.first == mx) {
            ans += p.second;
        } else if (p.first > mx) {
            mx = p.first;
            ans = p.second;
        }
        md(ans);
        root->upd(x, p);
    }
    cout << ans;
}
