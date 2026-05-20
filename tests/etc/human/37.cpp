 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
const ld PI = acosl(-1);
using namespace std;

struct r {
    ll x = 0, y = 0;

    r(r s, r e) {
        x = e.x - s.x;
        y = e.y - s.y;
    }


    r(ll xx, ll yy) {
        x = xx;
        y = yy;
    }

    r() = default;

    ld length() const {
        return sqrtl(x * x + y * y);
    }

    ll squar_length() const {
        return x * x + y * y;
    }

    friend istream &operator>>(istream &in, r &v) {
        return in >> v.x >> v.y;
    }

    friend ostream &operator<<(ostream &out, const r &v) {
        return out << v.x << ' ' << v.y;
    }


    ll operator*(r b) const { return x * b.x + y * b.y; }

    ll operator^(r b) const { return x * b.y - y * b.x; }

    r operator+(r b) const { return {x + b.x, y + b.y}; }

    r operator-(r b) const { return {x - b.x, y - b.y}; }

    r operator*(ll c) const { return {x * c, y * c}; }

    r operator+(ll c) const { return {x + c, y + c}; }

    r operator-(ll c) const { return {x - c, y - c}; }

    bool operator==(r b) const { return x == b.x && y == b.y; }

    bool operator!=(r b) const { return !(r(x, y) == b); }

    bool operator>(r b) const { return x > b.x || (x == b.x && y > b.y); }

    bool operator>=(r b) const { return r(x, y) == b || r(x, y) > b; }

    bool operator<(r b) const { return x < b.x || (x == b.x && y < b.y); }

    bool operator<=(r b) const { return r(x, y) == b || r(x, y) < b; }
};

ld angle(r a, r b) {
    return atan2(a ^ b, a * b);
}

bool dot_in_segment(r d, r a, r b) {
    if (min(a.x, b.x) > d.x || d.x > max(a.x, b.x)) return false;
    if (min(a.y, b.y) > d.y || d.y > max(a.y, b.y)) return false;
    return ((a - d) ^ (b - d)) == 0;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    r d;
    cin >> n >> d;
    vector<r> a(n);
    for (auto &el: a) cin >> el;
    ld ans = 0;
    for (ll i = 0; i < n; i++) {
        if (dot_in_segment(d, a[i], a[(i + 1) % n])) {
            cout << "YES";
            return 0;
        }
        ans += angle(a[i] - d, a[(i + 1) % n] - d);
    }
    cout << (abs(ans) > PI ? "YES" : "NO");
}
