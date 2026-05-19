 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = double;
const ld EPS = 1e-7;
const ld pi = 3.141592653;
using namespace std;

struct dot {
    ll x, y;
};

istream &operator>>(istream &inp, dot &v) {
    inp >> v.x >> v.y;
    return inp;
}

struct r {
    ll x, y;

    r(dot s, dot e) {
        x = e.x - s.x;
        y = e.y - s.y;
    }

    ld len() const {
        return sqrt(x * x + y * y);
    }
};

ll operator*(r a, r b) { return a.x * b.x + a.y * b.y; }

ll operator^(r a, r b) { return a.x * b.y - a.y * b.x; }


ld angle(r a, r b) {
    return atan2(a ^ b, a * b);
}


ll sign(ll x) {
    return x == 0 ? 0 : x > 0 ? 1 : -1;
}

ll sign(ld x) {
    return std::abs(x) <= 10.0 * EPS ? 0 : x > 0 ? 1 : -1;
}

bool is_dot_in_ray(dot p, dot a, dot b) {
    return sign(angle(r(a, p), r(a, b))) == 0;
}

bool is_dot_in_angle(dot p, dot a, dot o, dot b) {
    ll s1 = sign(r(o, a) ^ r(o, p));
    ll s2 = sign(r(o, p) ^ r(o, b));
    ll s3 = sign(r(o, a) ^ r(o, b));
    return (s1 == s2 && s1 == s3 && s1 != 0) || is_dot_in_ray(p, o, a) || is_dot_in_ray(p, o, b);
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.setf(ios::fixed);
    cout.precision(6);
    dot o, a, b, p;
    cin >> a >> o >> b >> p;
    cout << (is_dot_in_angle(p, a, o, b) ? "YES" : "NO");
}
