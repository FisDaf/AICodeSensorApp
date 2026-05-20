 #pragma GCC optimize("O3,unroll-loops,unswitch-loops")

#include <bits/stdc++.h>

#pragma GCC target("avx2")

using ll = long long;
using ld = long double;
using namespace std;

struct pt {
    ll x = 0, y = 0;

    pt() = default;
    pt(ll xx, ll yy) : x(xx), y(yy) {}
    pt(pt s, pt e) : x(e.x - s.x), y(e.y - s.y) {}
    pt(const pt &s) : x(s.x), y(s.y) {}

    ld len() { return sqrtl(len2()); }
    ll len2() { return x * x + y * y; }
    ll manhat_len() { return abs(x) + abs(y); }
    ll hp_pos() { return (y > 0 || (y == 0 && x >= 0)) ? 1 : -1; }
    ll operator*(pt p) { return x * p.x + y * p.y; }
    ll operator^(pt p) { return x * p.y - y * p.x; }
    pt operator+(pt p) { return pt(p.x + x, p.y + y); }
    pt operator-(pt p) { return pt(x - p.x, y - p.y); }
    bool operator==(pt p) { return x == p.x && y == p.y; }
    bool operator!=(pt p) { return !(*this == p); }
    friend istream &operator>>(istream &in, pt &p) { return in >> p.x >> p.y; }
    friend ostream &operator<<(ostream &out, pt &p) {
        return out << p.x << ' ' << p.y;
    }
};

ld angle(pt p1, pt p2) {
    return std::abs(atan2(p1 ^ p2, p1 * p2));
}

ll sign(ll x) {
    return (x > 0) - (x < 0);
}

bool polar_angle_cmp(pt a, pt b) {
    if (a.hp_pos() != b.hp_pos())
        return a.hp_pos() > b.hp_pos();
    ll s = sign(a ^ b);
    if (s != 0)
        return s > 0;
    return a.manhat_len() < b.manhat_len();
}

ll polygon_area(vector<pt> points) {
    ll ans = 0;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        ans += points[i] ^ points[(i + 1) % n];
    }
    return abs(ans);
}

vector<pt> jarvis(vector<pt> points) {
    pt st = points[0];
    for (auto p : points) {
        if ((p.x < st.x) || (p.x == st.x && p.y < st.y)) {
            st = p;
        }
    }
    vector<pt> hull = {st};
    while (true) {
        pt par = hull.back();
        pt next = hull.back();
        for (auto p : points) {
            ll cp = pt(par, next) ^ pt(par, p);
            ll manh1 = pt(par, next).manhat_len();
            ll manh2 = pt(par, p).manhat_len();
            if (cp < 0) {
                next = p;
            } else if (cp == 0 && manh1 < manh2) {
                next = p;
            }
        }
        if (next == hull[0])
            break;
        hull.push_back(next);
    }
    return hull;
}

vector<pt> graham(vector<pt> points) {
    pt st = points[0];
    for (auto p : points) {
        if ((p.y < st.y) || (p.y == st.y && p.x < st.x)) {
            st = p;
        }
    }
    for (auto &el : points)
        el = el - st;
    sort(points.begin(), points.end(), polar_angle_cmp);
    for (auto &el : points)
        el = el + st;
    vector<pt> hull;
    for (auto p : points) {
        while (hull.size() >= 2) {
            pt cur = pt(hull[hull.size() - 2], hull.back());
            pt cond = pt(hull.back(), p);
            ll cp = cur ^ cond;
            if (cp > 0)
                break;
            hull.pop_back();
        }
        hull.push_back(p);
    }
    return hull;
}

void solve() {
    int n;
    cin >> n;
    vector<pt> a(n);
    for (auto &el : a) {
        cin >> el;
    }
    vector<pt> res = graham(a);
    cout << res.size() << '\n';
    for (auto el : res) {
        cout << el << '\n';
    }
    ll s = polygon_area(res);
    if (s & 1) {
        cout << s / 2 << ".5\n";
    } else {
        cout << s / 2 << ".0\n";
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
