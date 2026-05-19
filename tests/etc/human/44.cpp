 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("vpt")

#include <bits/stdc++.h>

using namespace std;

struct Node {
    int pref, suf, ans;
    bool f;
};

constexpr int sz = 1ll << 21;
constexpr Node neutral = { 0 ,0,0, 1 };
Node tree[2 * sz];
Node relax(Node& lx, Node& rx) {
    Node x;
    x.pref = lx.pref;
    x.suf = rx.suf;
    if (lx.f) x.pref += rx.pref;
    if (rx.f) x.suf += lx.suf;
    x.f = lx.f && rx.f;
    x.ans = max(max(x.suf, x.pref), max(lx.ans, rx.ans));
    x.ans = max(x.ans, lx.suf + rx.pref);
    return x;
}

void build(int x = 1, int lx = 0, int rx = sz) {
    tree[x].ans = tree[x].pref = tree[x].suf = rx - lx;
    tree[x].f = 1;
    if (rx - lx != 1) {
        int mx = (lx + rx) >> 1;
        build(x << 1, lx, mx);
        build(x << 1 | 1, mx, rx);
    }
}

void clear(int x = 1, int lx = 0, int rx = sz) {
    if (tree[x].f) return;
    tree[x].ans = tree[x].pref = tree[x].suf = rx - lx;
    tree[x].f = true;
    if (rx - lx != 1) {
        int mx = (lx + rx) >> 1;
        clear(x << 1, lx, mx);
        clear(x << 1 | 1, mx, rx);
    }
}

void upd(int i, int v) {
    int x = i + sz;
    tree[x].ans = tree[x].f = tree[x].pref = tree[x].suf = v;
    for (x >>= 1; x;x >>= 1) tree[x] = relax(tree[x << 1], tree[x << 1 | 1]);
}

int ask(int k, int x = 1, int lx = 0, int rx = sz) {
    while (rx - lx != 1) {
        int mx = (lx + rx) >> 1;
        if (tree[x << 1].ans >= k) {
            x <<= 1;
            rx = mx;
        } else if (tree[x << 1].suf + tree[x << 1 | 1].pref >= k) {
            return mx - tree[x << 1].suf;
        } else {
            x = x << 1 | 1;
            lx = mx;
        }
    }
    return lx;
}

void solve() {
    clear();
    int n, x;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        upd(x - 1, 0);
    }
    int q;
    cin >> q;
    while (q--) {
        char t;
        cin >> t >> x;
        if (t == '+') {
            upd(x - 1, 0);
        } else if (t == '-') {
            upd(x - 1, 1);
        } else {
            if (tree[1].ans < x) {
                cout << sz - tree[1].suf + 1 << ' ';
            } else {
                cout << ask(x) + 1 << ' ';
            }
        }
    }
    cout << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    build();
    int t;
    cin >> t;
    while (t--) solve();
}
