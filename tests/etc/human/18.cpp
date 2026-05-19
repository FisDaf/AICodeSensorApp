#pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>

using ll = long long;
using ld = double;
const ll INF = 1e16;
const ld EPS = 1e-7;
const ll MOD = 1e9 + 7;
using namespace std;
vector<vector<pair<ll, ll>>> g;
vector<unordered_set<ll>> gm;
vector<ll> used, tin, up, col_cnt;
ll ti = 0;
vector<bool> ans;
vector<ll> color;

void dfs(ll v, ll e = -1) {
    tin[v] = up[v] = ti++;
    used[v] = 1;
    for (auto [to, id]: g[v]) {
        if (e == id) continue;
        if (used[to]) up[v] = min(up[v], tin[to]);
        if (!used[to]) {
            dfs(to, id);
            up[v] = min(up[v], up[to]);
            if (tin[v] < up[to]) {
                ans[id] = true;
            }
        }

    }
    used[v] = 2;
}

ll col = 0;

void dfs2(ll v) {
    used[v] = 1;
    color[v] = col;
    for (auto [to, id]: g[v]) {
        if (!used[to] && !ans[id]) {
            dfs2(to);
        }
    }
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    g.assign(n, {});
    used.assign(n, 0);
    tin.assign(n, -1);
    up.assign(n, -1);
    ans.assign(m, false);
    for (ll i = 0; i < m; i++) {
        ll u, v;
        cin >> u >> v;
        g[u - 1].push_back({v - 1, i});
        g[v - 1].push_back({u - 1, i});
    }
    vector<ll> out;
    for (ll i = 0; i < n; i++) {
        if (!used[i]) dfs(i);
    }
    used.assign(n, 0);
    color.assign(n, -1);
    for (ll i = 0; i < n; i++) {
        if (!used[i]) dfs2(i), col++;
    }
    gm.assign(col, {});
    col_cnt.assign(col, 0);
    for (ll i = 0; i < n; i++) {
        col_cnt[color[i]]++;
        for (auto [to, id]: g[i]) {
            if (color[i] != color[to]) {
                gm[color[i]].insert(color[to]);
                gm[color[to]].insert(color[i]);
            }
        }
    }
    vector<ll> listiki;
    for (ll i = 0; i < col; i++) {
        if (gm[i].size() <= 1) listiki.push_back(i);
    }
    cout << listiki.size() << ' ';
    ll answer = 1;
    for (auto el: listiki) {
        answer *= col_cnt[el];
        answer %= MOD;
    }
    cout << answer;
}
