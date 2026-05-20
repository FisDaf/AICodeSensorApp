 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("vpt")

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
constexpr ll INF = 1e9;
constexpr ld EPS = 1e-9;
constexpr ll MOD = 1e9 + 7;

using namespace std;

mt19937 rnd(1488);
constexpr int maxn = 2e5 + 52;
struct Node {
    int x, y, sz;
    Node* left;
    Node* right;
    Node() {}
    Node(int xx) : x(xx), y(rnd()), sz(1), left(0), right(0) {}
};

int cur_id = 0;
Node a[maxn];

Node* new_node(int x) {
    a[cur_id] = Node(x);
    return &a[cur_id++];
}

Node* root = nullptr;
int get_sz(Node* v) {
    return v ? v->sz : 0;
}
void relax(Node* v) {
    v->sz = get_sz(v->left) + get_sz(v->right) + 1;
}

pair<Node*, Node*> split(Node* v, int k) {
    if (!v) return { nullptr, nullptr };
    if (v->x <= k) {
        auto [l, r] = split(v->right, k);
        v->right = l;
        relax(v);
        return { v, r };
    }
    auto [l, r] = split(v->left, k);
    v->left = r;
    relax(v);
    return { l, v };
}

Node* merge(Node* a, Node* b) {
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

void add(int x) {
    auto [l, r] = split(root, x);
    Node* v = new_node(x);
    root = merge(merge(l, v), r);
}
void remv(int x) {
    auto [l, r] = split(root, x);
    auto [l1, r1] = split(l, x - 1);
    root = merge(l1, r);
}
int find(Node* v, int k) {
    if (get_sz(v->right) == k - 1) return v->x;
    if (get_sz(v->right) >= k) return find(v->right, k);
    return find(v->left, k - 1 - get_sz(v->right));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    while (q--)
    {
        int t, x;
        cin >> t >> x;
        if (t == 1) {
            add(x);
        } else if (t == -1) {
            remv(x);
        } else {
            cout << find(root, x) << '\n';
        }
    }

}
