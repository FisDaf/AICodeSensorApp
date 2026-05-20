 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-6;
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

    r operator+=(r b) const { return {x + b.x, y + b.y}; }

    r operator-(r b) const { return {x - b.x, y - b.y}; }

    r operator-=(r b) const { return {x - b.x, y - b.y}; }

    r operator*(ll c) const { return {x * c, y * c}; }

    r operator+(ll c) const { return {x + c, y + c}; }

    r operator-(ll c) const { return {x - c, y - c}; }

    r operator*=(ll c) const { return {x * c, y * c}; }

    r operator+=(ll c) const { return {x + c, y + c}; }

    r operator-=(ll c) const { return {x - c, y - c}; }

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

bool eq(ld x, ld y) {
    return std::abs(x - y) <= EPS;
}

ll sign(ll x) {
    return x == 0 ? 0 : x > 0 ? 1 : -1;
}

ll sign(ld x) {
    return std::abs(x) <= EPS ? 0 : x > 0 ? 1 : -1;
}


ld polygon_area(vector<r> &points) {
    ll s = 0;
    for (ll i = 0; i < points.size(); i++) {
        s += points[i] ^ (points[(i + 1) % points.size()]);
    }
    s = std::abs(s);
    return (ld) s * 0.5;
}

ll manhat_dist(r r1, r r2) {
    return abs(r1.x - r2.x) + abs(r1.y - r2.y);
}

vector<r> jarvis_convex_hull(vector<r> &points) {
    r start = *min_element(points.begin(), points.end());
    vector<r> hull = {start};
    while (true) {
        r next = hull.back();
        for (auto pt: points) {
            ll cross_product = (next - hull.back()) ^ (pt - hull.back());
            ll manh_d1 = manhat_dist(hull.back(), pt);
            ll manh_d2 = manhat_dist(hull.back(), next);
            if (next == hull.back() || cross_product < 0) {
                next = pt;
            } else if (cross_product == 0 && manh_d1 > manh_d2) {
                next = pt;
            }
        }
        if (next == start) break;
        hull.push_back(next);
    }
    return hull;
}


vector<r> graham_convex_hull(vector<r> points) {
    r start = *min_element(points.begin(), points.end());
    sort(points.begin(), points.end(), [&](r a, r b) {
        return ((a - start) ^ (b - start)) > 0 || (((a - start) ^ (b - start)) == 0 &&
                                                   manhat_dist(r(0, 0), a - start) < manhat_dist(r(0, 0), b - start));
    });
    vector<r> hull;
    for (r pt: points) {
        while (hull.size() >= 2) {
            r new_vector = pt - hull.back();
            r last_vector = hull.back() - hull[hull.size() - 2];
            if ((new_vector ^ last_vector) < 0) break;
            hull.pop_back();
        }
        hull.push_back(pt);
    }
    return hull;
}
