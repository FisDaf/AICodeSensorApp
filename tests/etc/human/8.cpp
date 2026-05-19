#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long long
const ll MOD = (ll) 1e9 + 7;
const ll INF = 1e16;
using namespace std;
const ll sz = 131072;


ll tree[2 * sz - 1] = {}, fst_member[2 * sz - 1] = {}, step[2 * sz - 1] = {};
ll size;

ll get_sum(ll x, ll lx, ll rx) {
    return tree[x] + (2 * fst_member[x] + (rx - lx - 1) * step[x]) * (rx - lx) / 2;
}

void relax(ll x, ll lx, ll rx) {
    ll mid = (lx + rx) >> 1;
    tree[x] = get_sum(2 * x + 1, lx, mid) + get_sum(2 * x + 2, mid, rx);
}

void push(ll x, ll lx, ll rx) {
    ll mid = (lx + rx) >> 1;
    if (rx - lx > 1) {
        fst_member[2 * x + 1] += fst_member[x];
        fst_member[2 * x + 2] += fst_member[x] + step[x] * (mid - lx);
        step[2 * x + 1] += step[x];
        step[2 * x + 2] += step[x];
    }
    fst_member[x] = step[x] = 0;
}

void add(ll st, ll dif, ll l, ll r, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l || l >= r) return;
    if (lx >= l && rx <= r) {
        fst_member[x] += st + (lx - l) * dif;
        step[x] += dif;
        return;
    }
    ll mid = (lx + rx) >> 1;
    push(x, lx, rx);
    add(st, dif, l, r, 2 * x + 1, lx, mid);
    add(st, dif, l, r, 2 * x + 2, mid, rx);
    relax(x, lx, rx);
}

ll get(ll l, ll r, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return 0;
    if (lx >= l && rx <= r) return get_sum(x, lx, rx);
    ll mid = (lx + rx) >> 1;
    push(x, lx, rx);
    relax(x, lx, rx);
    return get(l, r, 2 * x + 1, lx, mid) + get(l, r, 2 * x + 2, mid, rx);
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    char t;
    ll l, r, i, x, q;
    while (m--) {
        cin >> t;
        if (t == '?') {
            cin >> l >> r;
            cout << get(l - 1, r) << "\n";
        } else if (t == 'R') {
            cin >> i >> q;
            add(q, -1, i - 1, min(i + q - 1, sz));
            add(max(1ll, q - i + 1), 1, max(0ll, i - q), i - 1);
        } else {
            cin >> l >> r >> x;
            if (x != 0) add(-x, 0, l - 1, r);
        }
    }
}
