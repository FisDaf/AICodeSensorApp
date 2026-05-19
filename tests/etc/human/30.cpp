 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

#define ll long double
#define ld long double
const ll MOD = (ll) 1e9 + 7;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;
struct r {
    ll x, y;
};

struct dot {
    ll x, y;
};

ll operator^(r a, r b) { return a.x * b.y - b.x * a.y; }

ll operator*(r a, r b) { return a.x * b.x + a.y * b.y; }

ld angle(r a, r b) {
    return atan2(a ^ b, a * b);
}

using namespace std;

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    dot a1{}, a2{}, b1{}, b2{};
    cin >> a1.x >> a1.y >> a2.x >> a2.y;
    cin >> b1.x >> b1.y >> b2.x >> b2.y;
    cout.setf(ios::fixed);
    cout.precision(6);
    if (a1.x == a2.x || b1.x == b2.x) {
        if (a1.x == a2.x && b1.x == b2.x) {
            a1.x == b1.x ? cout << 2 : cout << 0;
            exit(0);
        }
        // РЅР°Р№С‚Рё РїРµСЂРµСЃРµС‡РµРЅРёСЏ
        if (a1.x == a2.x) {
            ld k2 = (b1.y - b2.y) / (b1.x - b2.x);
            ld d2 = b1.y - b1.x * k2;
            cout << 1 << ' ' << a1.x << ' ' << a1.x * k2 + d2;
        } else {
            ld k1 = (a1.y - a2.y) / (a1.x - a2.x);
            ld d1 = a1.y - a1.x * k1;
            cout << 1 << ' ' << b1.x << ' ' << b1.x * k1 + d1;
        }
        exit(0);
    }
    if (a1.y == a2.y || b1.y == b2.y) {
        if (a1.y == a2.y && b1.y == b2.y) {
            a1.y == b1.y ? cout << 2 : cout << 0;
            exit(0);
        }
        // РЅР°Р№С‚Рё РїРµСЂРµСЃРµС‡РµРЅРёСЏ
        if (a1.y == a2.y) {
            ld k2 = (b1.y - b2.y) / (b1.x - b2.x);
            ld d2 = b1.y - b1.x * k2;
            cout << 1 << ' ' << (a1.y - d2) / k2 << ' ' << a1.y;
        } else {
            ld k1 = (a1.y - a2.y) / (a1.x - a2.x);
            ld d1 = a1.y - a1.x * k1;
            cout << 1 << ' ' << (b1.y - d1) / k1 << ' ' << b1.y;
        }

        exit(0);
    }

    ld k1 = (a1.y - a2.y) / (a1.x - a2.x);
    ld k2 = (b1.y - b2.y) / (b1.x - b2.x);
    ld d1 = a1.y - a1.x * k1;
    ld d2 = b1.y - b1.x * k2;
    if (abs(k1 - k2) <= EPS) {
        d1 == d2 ? cout << 2 : cout << 0;
        exit(0);
    }
    ld ansx = (d2 - d1) / (k1 - k2);
    cout << 1 << ' ' << ansx << ' ' << ansx * k1 + d1;
}
