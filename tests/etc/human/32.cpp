 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = int;
using ld = double;
const ld EPS = 1e-9;
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


ld from_dot_to_line(dot d, dot a, dot b) {
    return std::abs((ld) (r(d, a) ^ r(d, b))) / r(a, b).len();
}

ld from_dot_to_segment(dot d, dot a, dot b) {
    ld ang1 = std::abs(angle(r(a, d), r(a, b)));
    ld ang2 = std::abs(angle(r(b, d), r(b, a)));
    ld ans = 1e10;
    if ((2.0 * ang1 <= pi + EPS) && (2.0 * ang2 <= pi + EPS)) ans = from_dot_to_line(d, a, b);
    return min(min(r(d, a).len(), r(d, b).len()), ans);
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.setf(ios::fixed);
    cout.precision(6);
    dot a, b, c;
    ld l;
    cin >> a >> b >> c;
    cin >> l;
    cout << max(from_dot_to_segment(c, a, b) - l, (ld) 0) << '\n';
    cout << max(max(r(a, c).len(), r(b, c).len()) - l, (ld) 0) << '\n';
}
