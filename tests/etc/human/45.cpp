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
    int x, y, sz, mx, mn;
    Node* left;
    Node* right;
    Node() {}
    Node(int xx) : x(xx), y(rnd()), mx(xx), mn(xx), sz(1), left(0), right(0) {}
};

int cur_id = 0;
Node a[maxn];

Node* new_node(int x) {
    a[cur_id] = Node(x);
    return &a[cur_id++];
}

int get_min(Node* v) {
    return v ? v->mn : INF;
}
int get_max(Node* v) {
    return v ? v->mx : -INF;
}
int get_sz(Node* v) {
    return v ? v->sz : 0;
}
void relax(Node* v) {
    v->mn = min(v->x, min(get_min(v->left), get_min(v->right)));
    v->mx = max(v->x, max(get_max(v->left), get_max(v->right)));
    v->sz = get_sz(v->left) + get_sz(v->right) + 1;
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
bool find(Node* v, int x) {
    if (!v) return 0;
    if (v->x == x) return 1;
    if (v->x < x) return find(v->right, x);
    return find(v->left, x);
}
void add(int x) {
    if (find(root, x)) return;
    auto [l, r] = split(root, x);
    Node* v = new_node(x);
    root = merge(merge(l, v), r);
}

void rem(int x) {
    if (!find(root, x)) return;
    auto [l, r] = split(root, x);
    auto [l1, r1] = split(l, x - 1);
    root = merge(l1, r);
}
int next(int x) {
    auto [l, r] = split(root, x);
    int ans = get_min(r);
    root = merge(l, r);
    return  ans;
}
int prev(int x) {
    auto [l, r] = split(root, x - 1);
    int ans = get_max(l);
    root = merge(l, r);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int q, x;
    string s;
    while (cin >> s) {
        cin >> x;
        if (s[0] == 'i') {
            add(x);
        } else if (s[0] == 'd') {
            rem(x);
        } else if (s[0] == 'e') {
            cout << (find(root, x) ? "true\n" : "false\n");
        } else if (s[0] == 'n') {
            int ans = next(x);
            if (ans == INF) {
                cout << "none\n";
            } else {
                cout << ans << '\n';
            }
        } else {
            int ans = prev(x);
            if (ans == -INF) {
                cout << "none\n";
            } else {
                cout << ans << '\n';
            }
        }
    }

}
