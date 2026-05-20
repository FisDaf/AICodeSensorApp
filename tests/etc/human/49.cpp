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

int gcd(int x, int y) {
    if (x == 0) return y;
    if (y == 0) return x;
    while (y) swap(x = x % y, y);
    return x;
}
mt19937 rnd(1488);
constexpr int maxn = 3e5 + 52;
struct Node {
    int x, y, cnt, ggg;
    Node* left;
    Node* right;
    Node() {}
    Node(int xx) : x(xx), y(rnd()), ggg(xx), cnt(1), left(0), right(0) {}
};

int cur_id = 0;
Node a[maxn];

Node* new_node(int x) {
    a[cur_id] = Node(x);
    return &a[cur_id++];
}

int get_gcd(Node* v) {
    return v ? v->ggg : 0;
}

void relax(Node* v) {
    v->ggg = gcd(v->x, get_gcd(v->left));
    v->ggg = gcd(v->ggg, get_gcd(v->right));
}
Node* root = nullptr;

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
int find(Node* v, int x, int mod = 1) {
    if (!v) return -1;
    if (v->x == x) {
        v->cnt += mod;
        return v->cnt;
    }
    if (v->x < x) return find(v->right, x, mod);
    return find(v->left, x, mod);
}
void add(int x) {
    int gggg = find(root, x, 1);
    if (gggg != -1)return;
    auto [l, r] = split(root, x);
    Node* v = new_node(x);
    root = merge(merge(l, v), r);
}

void rem(int x) {
    int gggg = find(root, x, -1);
    if (gggg != 0) return;
    auto [l, r] = split(root, x);
    auto [l1, r1] = split(l, x - 1);
    root = merge(l1, r);
}





int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    while (q--)
    {
        char t;
        int x;
        cin >> t >> x;
        if (t == '+') {
            add(x);
        } else {
            rem(x);
        }
        cout << (get_gcd(root) ? get_gcd(root) : 1) << '\n';
    }

}
