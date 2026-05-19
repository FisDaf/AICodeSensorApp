#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<bool> vb;

void bfs(const vvi& adj, vi& dist) {
    queue<int> q;
    vb visited(adj.size(), false);
    q.push(1);
    visited[1] = true;
    dist[1] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vvi adj(n + 1);
    vector<pair<int, int>> roads(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        roads[i] = {u, v};
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vi dist(n + 1, -1);
    bfs(adj, dist);

    vector<pair<int, int>> edges;
    for (int i = 0; i < m; ++i) {
        int u = roads[i].first;
        int v = roads[i].second;
        if (dist[u] < dist[v]) {
            edges.emplace_back(u, v);
        } else {
            edges.emplace_back(v, u);
        }
    }

    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());

    vector<string> result;
    int total_edges = edges.size();
    for (int mask = 0; mask < (1 << total_edges); ++mask) {
        if (__builtin_popcount(mask) != n - 1) continue;

        vvi adj_tmp(n + 1);
        for (int i = 0; i < total_edges; ++i) {
            if (mask & (1 << i)) {
                adj_tmp[edges[i].first].push_back(edges[i].second);
                adj_tmp[edges[i].second].push_back(edges[i].first);
            }
        }

        vi dist_tmp(n + 1, -1);
        bfs(adj_tmp, dist_tmp);

        bool valid = true;
        for (int i = 1; i <= n; ++i) {
            if (dist_tmp[i] == -1) {
                valid = false;
                break;
            }
        }

        if (valid) {
            string option(m, '0');
            for (int i = 0; i < total_edges; ++i) {
                if (mask & (1 << i)) {
                    option[i] = '1';
                }
            }
            result.push_back(option);
            if (result.size() == k) break;
        }
    }

    cout << result.size() << '\n';
    for (const string& s : result) {
        cout << s << '\n';
    }

    return 0;
}
