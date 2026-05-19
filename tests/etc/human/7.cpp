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

struct v {
    ll l = -2, r = -2, p = 0, pref = 1, suf = 1, ans = 1, lx = -1, rx = -1;

    ll left() const {
        return l + p;
    }

    ll right() const {
        return r + p;
    }
};

v tree[2 * sz - 1];

void build(ll x = 0, ll lx = 0, ll rx = sz) {
    tree[x].lx = lx;
    tree[x].rx = rx;
    if (rx - lx != 1) {
        ll mx = (lx + rx) >> 1;
        build(2 * x + 1, lx, mx);
        build(2 * x + 2, mx, rx);
        tree[x].l = tree[2 * x + 1].left();
        tree[x].r = tree[2 * x + 2].right();
        tree[x].pref = tree[2 * x + 1].pref;
        tree[x].suf = tree[2 * x + 2].suf;
        tree[x].ans = max(tree[2 * x + 1].ans, tree[2 * x + 2].ans);
        if (tree[2 * x + 1].right() + 1 == tree[2 * x + 2].left()) {
            tree[x].ans = max(tree[x].ans, tree[2 * x + 1].suf + tree[2 * x + 2].pref);
            if (mx - lx == tree[2 * x + 1].pref) {
                tree[x].pref += tree[2 * x + 2].pref;
            }
            if (rx - mx == tree[2 * x + 2].suf) {
                tree[x].suf += tree[2 * x + 1].suf;
            }
        }
        tree[x].ans = max(tree[x].ans, tree[x].suf);
        tree[x].ans = max(tree[x].ans, tree[x].pref);
    }
}


void upd(ll l, ll r, ll p, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return;
    if (lx >= l && rx <= r) {
        tree[x].p += p;
        return;
    }
    ll mx = (lx + rx) >> 1;
    upd(l, r, p, 2 * x + 1, lx, mx);
    upd(l, r, p, 2 * x + 2, mx, rx);
    tree[x].l = tree[2 * x + 1].left();
    tree[x].r = tree[2 * x + 2].right();
    tree[x].pref = tree[2 * x + 1].pref;
    tree[x].suf = tree[2 * x + 2].suf;
    tree[x].ans = max(tree[2 * x + 1].ans, tree[2 * x + 2].ans);
    if (tree[2 * x + 1].right() + 1 == tree[2 * x + 2].left()) {
        tree[x].ans = max(tree[x].ans, tree[2 * x + 1].suf + tree[2 * x + 2].pref);
        if (mx - lx == tree[2 * x + 1].pref) {
            tree[x].pref += tree[2 * x + 2].pref;
        }
        if (rx - mx == tree[2 * x + 2].suf) {
            tree[x].suf += tree[2 * x + 1].suf;
        }
    }
    tree[x].ans = max(tree[x].ans, tree[x].suf);
    tree[x].ans = max(tree[x].ans, tree[x].pref);
}

v ask(ll l, ll r, ll x = 0, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return {-2, -2, 0, 0, 0, 0, l, r};
    if (lx >= l && rx <= r) return tree[x];
    ll mx = (lx + rx) >> 1;
    v left = ask(l, r, 2 * x + 1, lx, mx);
    v right = ask(l, r, 2 * x + 2, mx, rx);
    v ans;
    ans.p += tree[x].p;
    ans.lx = left.lx;
    ans.rx = right.rx;
    ans.l = left.left();
    ans.r = right.right();
    ans.pref = left.pref;
    ans.suf = right.suf;
    ans.ans = max(left.ans, right.ans);
    if (left.right() + 1 == right.left()) {
        ans.ans = max(ans.ans, left.suf + right.pref);
        if (mx - left.lx == left.pref) {
            ans.pref += right.pref;
        }
        if (right.rx - mx == right.suf) {
            ans.suf += left.suf;
        }
    }
    ans.ans = max(ans.ans, ans.suf);
    ans.ans = max(ans.ans, ans.pref);
    return ans;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    cin >> n;
    for (ll i = 0; i < n; i++) {
        cin >> tree[sz - 1 + i].l;
        tree[sz - 1 + i].r = tree[sz - 1 + i].l;
    }
    build();
//    for (auto el: tree) {
//        cout << el.lx << ' ' << el.rx << '\n';
//    }
    ll q;
    cin >> q;
    while (q--) {
        char t;
        ll l, r, d;
        cin >> t >> l >> r;
        if (t == '+') {
            cin >> d;
            upd(l - 1, r, d);
        } else {
            v ans = ask(l - 1, r);
            cout << ans.ans << '\n';
        }
    }
}
