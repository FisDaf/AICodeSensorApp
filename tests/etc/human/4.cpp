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
const ll sz = maxr - maxl;
const ll maxn = 5e6;
ll rn = 1;
using namespace std;

struct Node {
    int left = -1, right = -1;
    int m = 0, p = 0, cnt;
};

Node tree[maxn];

ll get(ll x) { return tree[x].m + tree[x].p; }

void upd(ll l, ll r, ll s, ll i = 0, ll lx = maxl, ll rx = maxr) {
    if (lx >= r || rx <= l) return;
    if (lx >= l && rx <= r) {
        tree[i].p += s;
        return;
    }
    ll mx = (lx + rx) >> 1;
    if (tree[i].left == -1) tree[i].left = rn++, tree[tree[i].left].cnt = mx - lx;
    if (tree[i].right == -1) tree[i].right = rn++, tree[tree[i].right].cnt = rx - mx;
    upd(l, r, s, tree[i].left, lx, mx);
    upd(l, r, s, tree[i].right, mx, rx);
    if (get(tree[i].left) == get(tree[i].right)) tree[i].cnt = tree[tree[i].left].cnt + tree[tree[i].right].cnt;
    else if (get(tree[i].left) < get(tree[i].right)) tree[i].cnt = tree[tree[i].left].cnt;
    else tree[i].cnt = tree[tree[i].right].cnt;
    tree[i].m = min(get(tree[i].left), get(tree[i].right));
}


struct qur {
    int y, l, r, s;
};


bool comp(qur q1, qur q2) {
    return q2.y > q1.y;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    tree[0].cnt = sz;
    int n;
    cin >> n;
    qur a[2 * n];
    for (ll i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        a[2 * i + 1] = {y1, x1, x2, 1};
        a[2 * i] = {y2, x1, x2, -1};
    }
    sort(a, a + 2 * n, comp);
    ll ans = 0;
    int last = 0;
    for (auto [y, l, r, s]: a) {
        ans += (y - last) * (sz - tree[0].cnt);
        upd(l, r, s);
        last = y;
    }
    cout << ans;
}
