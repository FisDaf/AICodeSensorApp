#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;

vector<unordered_set<ll>> g, tg;
vector<bool> used;
vector<ll> topsort, color;
ll cl = 0;
vector<ll> cl_cnt;
vector<unordered_set<ll>> gc;

void dfssort(ll v) {
    used[v] = true;
    for (auto el : g[v]) {
        if (!used[el]) dfssort(el);
    }
    topsort.push_back(v);
}


void dfscond(ll v) {
    color[v] = cl;
    cl_cnt[cl]++;
//    if (g[v].empty()) return
    for (auto el : tg[v]) {
        if (color[el] == -1) {
            dfscond(el);
        }
    }
}


void solve() {
    ll n, m, h;
    cin >> n >> m >> h;
    vector<ll> ch(n);
    for (auto &el: ch) cin >> el;
    g.assign(n, {});
    tg.assign(n, {});
    while (m--) {
        ll u1, u2;
        cin >> u1 >> u2;
        u1--;
        u2--;
        if ((ch[u1] + 1) % h == ch[u2]) g[u1].insert(u2), tg[u2].insert(u1);
        if ((ch[u2] + 1) % h == ch[u1]) g[u2].insert(u1), tg[u1].insert(u2);
    }
    used.assign(n, false);
//    for (ll i = 0; i < n; i++) {
//        cout << i + 1 << ": ";
//        for (auto el: g[i]) cout << el + 1 << ' ';
//        cout << '\n';
//    }
    for (ll i = 0; i < n; i++) if (!used[i]) dfssort(i);
    reverse(topsort.begin(), topsort.end());
    color.assign(n, -1);
    for (auto i : topsort) {
        if (color[i] == -1) {
            cl_cnt.push_back(0);
            dfscond(i);
            cl++;
        }
    }
    gc.assign(cl, {});
    for (ll i = 0; i < n; i++) {
        for (auto el : g[i]) gc[color[i]].insert(color[el]);
    }
    for (ll i = 0; i < n; i++) gc[color[i]].erase(color[i]);
    ll ans = 100000000;
    ll ansc = -1;
    for (ll i = 0; i < n; i++) {
        if (gc[color[i]].empty()) {
            if (cl_cnt[color[i]] < ans) {
                ans = cl_cnt[color[i]];
                ansc = color[i];
            }
        }
    }
    cout << cl_cnt[ansc] << '\n';
    for (ll i = 0; i < n; i++) {
        if (ansc == color[i]) {
            cout << i + 1 << ' ';
        }
    }
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
}
