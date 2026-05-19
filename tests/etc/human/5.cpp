#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long long
#define ld long double

const ll maxx = 1e12 + 5;
ll timer = 1;


using namespace std;

struct segtree {
    segtree *left = nullptr, *right = nullptr;
    ll alloc = -1;
    ll t = -1;

    void upd(ll l, ll r, ll v, ll tm, ll lx = 0, ll rx = maxx) {
        if (lx >= r || rx <= l) return;
        if (lx >= l && rx <= r) {
            alloc = v;
            t = tm;
            return;
        }
        ll mx = (lx + rx) >> 1;
        if (!left) left = new segtree;
        if (!right) right = new segtree;
        left->upd(l, r, v, tm, lx, mx);
        right->upd(l, r, v, tm, mx, rx);
    }

    ll ask(ll i, ll v = 0, ll tm = 0, ll lx = 0, ll rx = maxx) {
        if (rx - lx == 1) return ((alloc != -1 && t > tm) ? alloc : v);
        if (!left) left = new segtree;
        if (!right) right = new segtree;
        ll mx = (lx + rx) >> 1;
        if (alloc != -1 && t > tm) v = alloc, tm = t;
        if (i < mx) return left->ask(i, v, tm, lx, mx);
        return right->ask(i, v, tm, mx, rx);
    }
};

segtree *root = new segtree;

struct quest {
    ld x, y;
    ll i;
};

bool comp(quest q1, quest q2) {
    return q1.y < q2.y;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll w, h, n;
    cin >> w >> h >> n;
    vector<quest> a(n);
    for (ll i = 0; i < n; i++) {
        cin >> a[i].x >> a[i].y;
        a[i].i = i;
    }
    vector<ll> answer(n);
    sort(a.begin(), a.end(), comp);
    for (auto [x, y, i]: a) {
        ld d = std::abs(y - (ld) root->ask((ll) x));
        answer[i] = (ll) (d * 2);
        root->upd((ll) (x - d), (ll) (x + d), (ll) (y + d), timer++);
    }
    for (auto el: answer) cout << el << '\n';
}
