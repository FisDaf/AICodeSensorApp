#pragma GCC optimize("Ofast,unroll-loops,unswitch-loops")

#include <bits/stdc++.h>

#pragma GCC target("avx2")

using namespace std;
using ll = long long;
using ull = unsigned int;
using ld = double;

constexpr ll INF = 1e18;

struct node {
    ll assign_val = INF;
    ll plus_val = 0;
    ll sum = 0;
    ll get_sum(int lx, int rx) {
        if (assign_val != INF) {
            return (assign_val + plus_val) * (rx - lx);
        }
        return sum + plus_val * (rx - lx);
    }
};

struct segtree {
    vector<node> tree;
    int sz;

    void init(int n) {
        sz = 1;
        while (sz < n) {
            sz *= 2;
        }
        tree.assign(2 * sz, {INF, 0, 0});
    }

    void push(int x) {
        if (tree[x].assign_val != INF) {
            tree[2 * x].assign_val = tree[2 * x + 1].assign_val =
                tree[x].assign_val;
            tree[2 * x].plus_val = tree[2 * x + 1].plus_val = 0;
            tree[x].assign_val = INF;
        }
        tree[2 * x].plus_val += tree[x].plus_val;
        tree[2 * x + 1].plus_val += tree[x].plus_val;
        tree[x].plus_val = 0;
    }

    void relax(int x, int lx, int rx) {
        int mx = (lx + rx) >> 1;
        tree[x].sum =
            tree[2 * x].get_sum(lx, mx) + tree[2 * x + 1].get_sum(mx, rx);
    }

    // [l, r)
    void seg_assign(int l, int r, int val, int x, int lx, int rx) {
        if (lx >= r || rx <= l)
            return;
        if (lx >= l && rx <= r) {
            tree[x].assign_val = val;
            tree[x].plus_val = 0;
            return;
        }
        push(x);
        int mx = (lx + rx) >> 1;
        seg_assign(l, r, val, 2 * x, lx, mx);
        seg_assign(l, r, val, 2 * x + 1, mx, rx);
        relax(x, lx, rx);
    }

    // [l, r]
    void seg_assign(int l, int r, int val) {
        seg_assign(l, r + 1, val, 1, 0, sz);
    }

    // [l, r)
    void seg_plus(int l, int r, int val, int x, int lx, int rx) {
        if (lx >= r || rx <= l)
            return;
        if (lx >= l && rx <= r) {
            tree[x].plus_val += val;
            return;
        }
        push(x);
        int mx = (lx + rx) >> 1;
        seg_plus(l, r, val, 2 * x, lx, mx);
        seg_plus(l, r, val, 2 * x + 1, mx, rx);
        relax(x, lx, rx);
    }
    // [l, r]
    void seg_plus(int l, int r, int val) { seg_plus(l, r + 1, val, 1, 0, sz); }

    // [l, r)
    ll seg_sum(int l, int r, int x, int lx, int rx) {
        if (lx >= r || rx <= l)
            return 0;
        if (lx >= l && rx <= r) {
            return tree[x].get_sum(lx, rx);
        }
        int mx = (lx + rx) >> 1;
        push(x);
        relax(x, lx, rx);
        return seg_sum(l, r, 2 * x, lx, mx) + seg_sum(l, r, 2 * x + 1, mx, rx);
    }
    // [l, r]
    ll seg_sum(int l, int r) { return seg_sum(l, r + 1, 1, 0, sz); }
};

void solve() {
    int n, m;
    cin >> n >> m;
    segtree t;
    t.init(n);
    while (m--) {
        char c;
        int l, r;
        cin >> c >> l >> r;
        if (c == '1') {
            int val;
            cin >> val;
            t.seg_assign(l, r - 1, val);
        } else if (c == '2') {
            int val;
            cin >> val;
            t.seg_plus(l, r - 1, val);
        } else {
            cout << t.seg_sum(l, r - 1) << '\n';
        }
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}
