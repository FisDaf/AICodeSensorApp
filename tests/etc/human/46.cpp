 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#pragma GCC target("avx,avx2,fma")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;


mt19937 rnd(1488);

struct node {
    ll x, y, sum;
    node *left, *right;

    node(ll xx) : x(xx), y(rnd()), sum(0), left(nullptr), right(nullptr) {}
};

ll get_val(node *a) {
    if (!a) return 0;
    return a->sum;
}

void relax(node *a) {
    a->sum = a->x + get_val(a->left) + get_val(a->right);
}


bool exist(node *a, ll x) {
    if (!a) return false;
    if (a->x == x) return true;
    if (x < a->x) return exist(a->left, x);
    return exist(a->right, x);
}

pair<node *, node *> split(node *a, ll k) {
    if (a == nullptr) return {nullptr, nullptr};
    if (a->x <= k) {
        auto [l, r] = split(a->right, k);
        a->right = l;
        relax(a);
        return {a, r};
    }
    auto [l, r] = split(a->left, k);
    a->left = r;
    relax(a);
    return {l, a};
}

node *merge(node *a, node *b) {
    if (!a) return b;
    if (!b) return a;
    if (a->y > b->y) {
        a->right = merge(a->right, b);
        relax(a);
        return a;
    }
    b->left = merge(a, b->left);
    relax(b);
    return b;
}

struct Treap {
    node *root = nullptr;


    ll get_sum(node *a, ll l, ll r) {
        if (!a) return 0;
        auto [l1, r1] = split(root, r);
        auto [l2, r2] = split(l1, l - 1);
        ll ans = get_val(r2);
        root = merge(merge(l2, r2), r1);
        return ans;
    }

    ll get_sum(ll l, ll r) {
        return get_sum(root, l, r);
    }

    void insert(ll x) {
        if (exist(root, x)) return;
        node *X = new node(x);
        relax(X);
        auto [l, r] = split(root, x);
        root = merge(merge(l, X), r);
    }
};

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    Treap treap;
    ll p = 0;
    const ll MOD = 1e9;
    ll n;
    cin >> n;
    while (n--) {
        char t;
        cin >> t;
        if (t == '+') {
            ll x;
            cin >> x;
            treap.insert((x + p) % MOD);
            p = 0;
        } else {
            ll l, r;
            cin >> l >> r;
            cout << (p = treap.get_sum(l, r)) << '\n';
        }
    }
}
