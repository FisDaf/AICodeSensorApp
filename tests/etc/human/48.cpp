 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("vpt")

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
constexpr ll INF = 1e9 + 52;
constexpr ld EPS = 1e-9;
constexpr ll MOD = 1e9;

using namespace std;

mt19937 rnd(1488);
constexpr int maxn = 3e5 + 52;
struct Node {
    int x, y, mn;
    Node* left;
    Node* right;
    Node() {}
    Node(int xx) : x(xx), y(rnd()), mn(xx), left(0), right(0) {}
};

int cur_id = 0;
Node a[maxn];

Node* new_node(int x) {
    a[cur_id] = Node(x);
    return &a[cur_id++];
}

Node* root = nullptr;
int get_min(Node* v) {
    return v ? v->mn : INF;
}
void relax(Node* v) {
    v->mn = min(v->x, min(get_min(v->left), get_min(v->right)));
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
int find(Node* v, int x) {
    if (!v) return 0;
    if (v->x == x) return 1;
    if (v->x < x) return find(v->right, x);
    return find(v->left, x);
}
void add(int x) {
    if (find(root, x))return;
    auto [l, r] = split(root, x);
    Node* v = new_node(x);
    root = merge(merge(l, v), r);
}


int lower(int x) {
    auto [l, r] = split(root, x - 1);
    int ans = get_min(r);
    root = merge(l, r);
    return ans == INF ? -1 : ans;
}

int pl(int x, int y) {
    return ((x + y) % MOD + MOD) % MOD;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    int gg = 0;
    while (q--)
    {
        char t;
        int x;
        cin >> t >> x;
        if (t == '+') {
            add(pl(x, gg));
            gg = 0;
        } else {
            gg = lower(x);
            cout << gg << '\n';
        }
    }

}
