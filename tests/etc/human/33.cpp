 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("vpt")

#include<bits/stdc++.h>

#pragma GCC target("avx", "avx2")

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-9;
const ld PI = atan2l(0, -1);
using namespace std;

ll sign(ll x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

ll sign(ld x) {
    if (x > EPS) return 1;
    if (x < -EPS) return -1;
    return 0;
}

struct r {
    ll x, y;

    r() : x(0ll), y(0ll) {}

    explicit r(ll xy) : x(xy), y(xy) {}

    r(ll xx, ll yy) : x(xx), y(yy) {}

    r(r s, r e) : x(e.x - s.x), y(e.y - s.y) {}

    friend istream &operator>>(istream &in, r &a) {
        return in >> a.x >> a.y;
    }

    friend ostream &operator<<(ostream &out, r &a) {
        return out << a.x << ' ' << a.y;
    }

    ld length() const { return sqrtl(x * x + y * y); }

    ll square_length() const { return x * x + y * y; }

    ll manhattan_length() const { return abs(x) + abs(y); }

    ll hp_pos() const {
        return (y > 0 || (y == 0 && x >= 0)) ? 1 : -1;
    }

    r operator+(r &b) const { return {x + b.x, y + b.y}; }

    r operator-(r &b) const { return {x - b.x, y - b.y}; }

    r operator+(ll b) const { return {x + b, y + b}; }

    r operator-(ll b) const { return {x - b, y - b}; }

    r operator*(ll b) const { return {x * b, y * b}; }

    ll operator*(r b) const { return x * b.x + y * b.y; }

    ll operator^(r b) const { return x * b.y - y * b.x; }

    void operator+=(r &b) { x += b.x, y += b.y; }

    void operator-=(r &b) { x -= b.x, y -= b.y; }

    void operator+=(ll b) { x += b, y += b; }

    void operator-=(ll b) { x -= b, y -= b; }

    bool operator==(r b) const { return x == b.x && y == b.y; }

    bool operator!=(r b) const { return !(r(x, y) == b); }

    bool operator>(r b) const { return y > b.y || (y == b.y && x > b.x); }

    bool operator>=(r b) const { return r(x, y) == b || r(x, y) > b; }

    bool operator<(r b) const { return y < b.y || (y == b.y && x < b.x); }

    bool operator<=(r b) const { return r(x, y) == b || r(x, y) < b; }

};


ld from_dot_to_dot(r a, r b) {
    return r(a, b).length();
}

ld angle(r a, r b) {
    return atan2l(a ^ b, a * b);
}


bool intersection_box(ll a, ll b, ll c, ll d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}

bool is_segments_intersect(r a, r b, r c, r d) {
    if (!intersection_box(a.x, b.x, c.x, d.x)) return false;
    if (!intersection_box(a.y, b.y, c.y, d.y)) return false;
    return sign(r(a, c) ^ r(a, b)) * sign(r(a, d) ^ r(a, b)) <= 0 &&
           sign(r(c, a) ^ r(c, d)) * sign(r(c, b) ^ r(c, d)) <= 0;
}

bool is_dot_on_ray(r p, r a, r b) {
    return sign(r(a, b) ^ r(a, p)) == 0 && sign(r(a, b) * r(a, p)) >= 0;
}


bool is_dot_on_segment(r p, r a, r b) {
    if (sign(r(p, a) ^ r(p, b)) != 0) return false;
    bool f1 = min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x);
    bool f2 = min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
    return f1 && f2;
}

bool is_rays_intersect(r a, r b, r c, r d) {
    ll s1 = sign(r(a, c) ^ r(a, b)) * sign(r(c, d) ^ r(a, b));
    ll s2 = sign(r(c, a) ^ r(c, d)) * sign(r(a, b) ^ r(c, d));
    ll s3 = is_dot_on_ray(a, c, d) || is_dot_on_ray(b, c, d);
    ll s4 = is_dot_on_ray(c, a, b) || is_dot_on_ray(d, a, b);
    return (s1 < 0 && s2 < 0) || s3 || s4;
}

bool is_segment_intersect_ray(r a, r b, r c, r d) {
    return is_rays_intersect(a, b, c, d)
           && is_rays_intersect(b, a, c, d);
}

bool is_segment_intersect_line(r a, r b, r c, r d) {
    return is_segment_intersect_ray(a, b, c, d)
           || is_segment_intersect_ray(a, b, d, c);
}

bool is_ray_intersect_line(r a, r b, r c, r d) {
    return is_rays_intersect(a, b, c, d)
           || is_rays_intersect(a, b, d, c);
}

bool is_lines_intersect(r a, r b, r c, r d) {
    return is_ray_intersect_line(a, b, c, d)
           || is_ray_intersect_line(b, a, c, d);
}
