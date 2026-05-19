#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;

const ll sz = 1 << 18;

struct node {
    vector<ll> a;
    vector<ll> pref = {0};
};

node t[2 * sz];

void mrg(node &v, vector<ll> &left, vector<ll> &right) {
    ll l = 0, r = 0;
    while (l != left.size() || r != right.size()) {
        if (l == left.size()) {
            v.a.push_back(right[r]);
            v.pref.push_back(right[r] + v.pref.back());
            r++;
        } else if (r == right.size()) {
            v.a.push_back(left[l]);
            v.pref.push_back(left[l] + v.pref.back());
            l++;
        } else {
            if (left[l] < right[r]) {
                v.a.push_back(left[l]);
                v.pref.push_back(left[l] + v.pref.back());
                l++;
            } else {
                v.a.push_back(right[r]);
                v.pref.push_back(right[r] + v.pref.back());
                r++;
            }
        }
    }
}


void build(ll x = 1, ll lx = 0, ll rx = sz) {
    if (rx - lx != 1) {
        ll mx = (lx + rx) >> 1;
        build(x << 1, lx, mx);
        build(x << 1 | 1, mx, rx);
        mrg(t[x], t[x << 1].a, t[x << 1 | 1].a);
    }
}


ll ask(ll l, ll r, ll v, ll x = 1, ll lx = 0, ll rx = sz) {
    if (lx >= r || rx <= l) return 0;
    if (lx >= l && rx <= r) {
        ll left = -1;
        ll right = (ll) t[x].a.size();
        while (right - left > 1) {
            ll mid = (left + right) >> 1;
            if (t[x].a[mid] <= v) {
                left = mid;
            } else {
                right = mid;
            }
        }
        return t[x].pref[right];
    }
    ll mx = (lx + rx) >> 1;
    return ask(l, r, v, x << 1, lx, mx) + ask(l, r, v, x << 1 | 1, mx, rx);
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    for (ll i = 0; i < n; i++) {
        ll el;
        cin >> el;
        t[sz + i].a = {el};
        t[sz + i].pref = {0, el};
    }
    build();
    while (m--) {
        ll al, ar;
        cin >> al >> ar;
        for (ll s = 0;;) {
            ll ans = ask(al - 1, ar, s + 1);
            if (ans == s) {
                cout << s + 1 << '\n';
                break;
            }
            s = ans;
        }
    }
}
