#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;

const ll maxn = 1e5;

vector<ll> tout(maxn, -1), tin(maxn, -1), up(maxn, -1);
vector<unordered_set<ll>> g(maxn);
ll timer = 0;
vector<bool> used(maxn, false);
map<pair<ll, ll>, ll> col;
ll maxColor = 0;

void dfs(ll v, ll p = -1) {
    used[v] = true;
    tin[v] = up[v] = timer++;
    int c = 0;
    for (auto to: g[v]) {
        if (to == p) continue;
        if (used[to]) {
            up[v] = min(up[v], tin[to]);
        } else {
            dfs(to, v);
            c++;
            up[v] = min(up[v], up[to]);
        }
    }
}

void paint(ll v, ll color, ll p) {
    used[v] = true;
    for (auto to: g[v]) {
        if (to == p) continue;
        if (!used[to]) {
            if (up[to] >= tin[v]) {
                ll newColor = ++maxColor;
                col[{min(to, v), max(to, v)}] = newColor;
                paint(to, newColor, v);
            } else {
                col[{min(to, v), max(to, v)}] = color;
                paint(to, color, v);
            }
        } else if (tin[to] < tin[v]) {
            col[{min(to, v), max(to, v)}] = color;
        }
    }
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    vector<pair<ll, ll>> edges;
    while (m--) {
        ll u, v;
        cin >> u >> v;
        edges.emplace_back(min(u, v) - 1, max(u, v) - 1);
        g[v - 1].insert(u - 1);
        g[u - 1].insert(v - 1);
    }
    for (ll i = 0; i < n; i++) {
        if (tin[i] < 0) {
            dfs(i);
        }
    }
    used.assign(maxn, false);
    for (ll i = 0; i < n; ++i) {
        if (!used[i]) {
            paint(i, maxColor, -1);
        }
    }
    unordered_map<ll, ll> z;
    ll rn = 1;
    vector<ll> ans;
    for (auto [x, y]: edges) {
        if (z.count(col[{x, y}]) == 0) {
            z[col[{x, y}]] = rn++;
        }
        ans.push_back(z[col[{x, y}]]);
    }
    cout << rn - 1 << '\n';
    for (auto el: ans) cout << el << ' ';
}