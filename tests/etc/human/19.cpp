#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = long double;
const ll INF = 1e16;
const ld EPS = 1e-7;
using namespace std;

const ll maxn = 5e5 + 5;

vector<ll> tin, up;
vector<unordered_multiset<ll>> g;
vector<unordered_set<ll>> cg;
ll timer = 0;
vector<ll> color;
set<pair<ll, ll>> most;

void dfs(ll v) {
    up[v] = tin[v] = timer++;
    for (auto u: g[v]) {
        if (tin[u] < 0) {
            g[u].erase(g[u].find(v));
            dfs(u);
            g[u].insert(v);
            up[v] = min(up[v], up[u]);
            if (up[u] > tin[v]) {
                most.emplace(min(u, v), max(u, v));
            }
        } else {
            up[v] = min(up[v], up[u]);
        }
    }
}

ll cl = 0;

void dfs2(ll v) {
    color[v] = cl;
    for (auto u: g[v]) {
        if (color[u] < 0 && most.count({min(u, v), max(u, v)}) == 0) {
            dfs2(u);
        }
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    tin.assign(n, -1);
    up.assign(n, -1);
    g.assign(n, {});
    color.assign(n, -1);
    while (m--) {
        ll u, v;
        cin >> u >> v;
        g[v - 1].insert(u - 1);
        g[u - 1].insert(v - 1);
    }
    for (ll i = 0; i < n; i++) {
        if (tin[i] < 0) {
            dfs(i);
        }
    }
    for (ll i = 0; i < n; i++) {
        if (color[i] < 0) {
            dfs2(i);
            cl++;
        }
    }
    cg.assign(cl, {});
    for (ll i = 0; i < n; i++) {
        for (auto to: g[i]) {
            if (color[to] != color[i]) {
                cg[color[i]].insert(color[to]);
                cg[color[to]].insert(color[i]);
            }
        }
    }
    ll out = INF;
    queue<ll> q;
    vector<ll> dist1(cl, INF), dist2(cl, INF);
    vector<ll> ep(cl, -1);
    ll e = -1;
    for (ll j = 0; j < cl; j++) {
        if (ep[j] != -1) continue;
        e++;
        q.push(j);
        dist1[q.front()] = 0;
        while (!q.empty()) {
            ll v = q.front();
            ep[v] = e;
            q.pop();
            for (auto u: cg[v]) {
                if (dist1[u] > dist1[v] + 1) {
                    dist1[u] = dist1[v] + 1;
                    q.push(u);
                }
            }
        }
        q.push(j);
        for (ll i = 0; i < cl; i++) {
            if (ep[i] == e && dist1[i] > dist1[q.front()]) {
                q.pop();
                q.push(i);
            }
        }
        dist2[q.front()] = 0;
        while (!q.empty()) {
            ll v = q.front();
            q.pop();
            for (auto u: cg[v]) {
                if (dist2[u] > dist2[v] + 1) {
                    dist2[u] = dist2[v] + 1;
                    q.push(u);
                }
            }
        }
        ll ans = 0;
        for (ll i = 0; i < cl; i++) {
            if (ep[i] == e) {
                ans = max(ans, dist2[i]);
            }
        }
        out = min(out, abs((ll) most.size() - ans));
    }
    cout << out;
}
