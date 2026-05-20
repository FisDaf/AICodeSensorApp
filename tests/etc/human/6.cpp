#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long long
#define ld long double
const ll MOD = (ll) 1e9 + 7;
const ll INF = 1e16;
const ld EPS = 1e-7;

const ll sz = 1 << 20;
using namespace std;

struct v {
    ll open = 0, close = 0, ans = 0;
};

v tree[2 * sz - 1];

void build(ll x = 0, ll lx = 0, ll rx = sz) {
    if (rx - lx != 1) {
        ll mx = (lx + rx) >> 1;
        build(2 * x + 1, lx, mx);
        build(2 * x + 2, mx, rx);
        ll d = min(tree[2 * x + 1].open, tree[2 * x + 2].close);
        tree[x].ans = tree[2 * x + 1].ans + tree[2 * x + 2].ans + 2 * d;
        tree[x].open = tree[2 * x + 1].open + tree[2 * x + 2].open - d;
        tree[x].close = tree[2 * x + 1].close + tree[2 * x + 2].close - d;
    }
}

v ask(ll l, ll r, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return {0, 0, 0};
    if (lx >= l && rx <= r) return tree[x];
    ll mx = (lx + rx) >> 1;
    v left = ask(l, r, 2 * x + 1, lx, mx);
    v right = ask(l, r, 2 * x + 2, mx, rx);
    v ans;
    ll d = min(left.open, right.close);
    ans.ans = left.ans + right.ans + 2 * d;
    ans.open = left.open + right.open - d;
    ans.close = left.close + right.close - d;
    return ans;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    ll n = (ll) s.size();
    for (ll i = 0; i < n; i++) (s[i] == '(' ? tree[sz - 1 + i].open : tree[sz - 1 + i].close) = 1;
    build();
    ll q;
    cin >> q;
    while (q--) {
        ll l, r;
        cin >> l >> r;
        cout << ask(l - 1, r).ans << '\n';
    }
}
