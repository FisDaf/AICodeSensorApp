 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-6;
const ld PI = 3.141592653;


using namespace std;

struct dot {
    ll x, y;

    friend istream &operator>>(istream &inp, dot &v) {
        inp >> v.x >> v.y;
        return inp;
    }
};


struct r {
    ll x{}, y{};

    r(dot s, dot e) {
        x = e.x - s.x;
        y = e.y - s.y;
    }

    r() = default;

    ld len() const {
        return sqrtl(x * x + y * y);
    }

    friend istream &operator>>(istream &inp, r &v) {
        inp >> v.x >> v.y;
        return inp;
    }

    friend ofstream &operator<<(ofstream &inp, r &v) {
        inp << v.x << ' ' << v.y << '\n';
        return inp;
    }
};


ll operator*(r a, r b) { return a.x * b.x + a.y * b.y; }

ll operator^(r a, r b) { return a.x * b.y - a.y * b.x; }

ll operator*(dot a, dot b) { return a.x * b.x + a.y * b.y; }

ll operator^(dot a, dot b) { return a.x * b.y - a.y * b.x; }

bool operator==(r a, r b) { return a.x == b.x && b.y == a.y; }

r operator-(r a, r b) {
    r out({0, 0}, {0, 0});
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}


ld angle(r a, r b) {
    return atan2(a ^ b, a * b);
}

ll sign(ll x) {
    return x == 0 ? 0 : x > 0 ? 1 : -1;
}

ll sign(ld x) {
    return std::abs(x) <= EPS ? 0 : x > 0 ? 1 : -1;
}

bool intersect_box(ll x1, ll x2, ll x3, ll x4) {
    return max(min(x1, x2), min(x3, x4)) <= min(max(x1, x2), max(x3, x4));
}


bool is_segment_intersect(dot a, dot b, dot c, dot d) {
    if (!intersect_box(a.x, b.x, c.x, d.x) ||
        !intersect_box(a.y, b.y, c.y, d.y))
        return false;
    return sign(r(a, c) ^ r(a, b)) * sign(r(a, d) ^ r(a, b)) <= 0
           && sign(r(c, a) ^ r(c, d)) * sign(r(c, b) ^ r(c, d)) <= 0;
}

ld from_dot_to_dot(dot d1, dot d2) {
    return sqrtl((d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y));
}

ld from_dot_to_line(dot d, dot a, dot b) {
    return std::abs((ld) (r(d, a) ^ r(d, b))) / r(a, b).len();
}

ld from_dot_to_segment(dot d, dot a, dot b) {
    ld ang1 = std::abs(angle(r(a, d), r(a, b)));
    ld ang2 = std::abs(angle(r(b, d), r(b, a)));
    ld ans = 1e10;
    if ((2.0 * ang1 <= PI + EPS) && (2.0 * ang2 <= PI + EPS)) ans = from_dot_to_line(d, a, b);
    return min(min(r(d, a).len(), r(d, b).len()), ans);
}

ld from_segment_to_segment(dot a, dot b, dot c, dot d) {
    if (is_segment_intersect(a, b, c, d)) return 0;
    return min(min(from_dot_to_segment(a, c, d), from_dot_to_segment(b, c, d)),
               min(from_dot_to_segment(d, a, b), from_dot_to_segment(c, a, b)));
}


ld from_segment_to_line(dot a, dot b, dot c, dot d) {
    if (sign(r(c, a) ^ r(c, d)) * sign(r(c, b) ^ r(c, d)) <= 0) return 0;
    return min(from_dot_to_line(a, c, d), from_dot_to_line(b, c, d));
}

ld from_line_to_line(dot a, dot b, dot c, dot d) {
    if ((r(a, c) ^ r(a, d)) == 0) return 0;
    if ((r(a, b) ^ r(c, d)) != 0) return 0;
    return from_dot_to_line(a, c, d);
}

bool is_dot_in_ray(dot p, dot a, dot b) {
    return sign(angle(r(a, p), r(a, b))) == 0;
}

bool is_dot_in_angle(dot p, dot a, dot o, dot b) {
    if (sign(r(o, a) ^ r(o, b)) < 0) {
        swap(a, b);
    }
    ll s1 = sign(r(o, a) ^ r(o, p));
    ll s2 = sign(r(o, p) ^ r(o, b));
    return s1 >= 0 && s2 >= 0;
}

bool is_dot_in_polgon(dot p, vector<dot> &a) {
    if (!is_dot_in_angle(p, a[1], a[0], a.back())) {
        return false;
    }
    ll n = a.size();
    ll l = 1;
    ll r = n - 1;
    while (r - l > 1) {
        ll mx = (r + l) >> 1;
        if (is_dot_in_angle(p, a[l], a[0], a[mx])) {
            r = mx;
        } else {
            l = mx;
        }
    }
    struct r a1(a[l], a[r]);
    struct r a2(a[l], p);
    if ((a1 ^ a2) >= 0) {
        return true;
    }
    return false;
}

bool cmp(vector<dot> v1, vector<dot> v2) {
    return v1[0].x > v2[0].x;
}
