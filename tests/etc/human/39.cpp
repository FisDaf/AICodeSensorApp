 

ld polygon_area(vector<dot> &points) {
    ll s = 0;
    for (ll i = 0; i < points.size(); i++) {
        s += ((points[i] ^ points[(i + 1) % points.size()]));
    }
    return std::abs((ld) s * 0.5);
}

vector<vector<dot>> polygons;

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(20);
    ll n;
    cin >> n;
    polygons.assign(n, {});
    for (ll i = 0; i < n; i++) {
        ll m;
        cin >> m;
        polygons[i].assign(m, {});
        for (auto &el: polygons[i]) {
            cin >> el.x >> el.y;
        }
        ll p0 = 0;
        for (ll j = 0; j < m; j++) {
            if (polygons[i][p0].x > polygons[i][j].x) {
                p0 = j;
            }
        }
        vector<dot> b;
        for (ll j = 0; j < m; j++) {
            b.push_back(polygons[i][(j + p0) % m]);
        }
        polygons[i] = b;
    }
    sort(polygons.begin(), polygons.end(), cmp);
    ll m;
    cin >> m;
    vector<bool> used(n, false);
    while (m--) {
        dot p;
        cin >> p;
        ll l = -1;
        ll r = n;
        while (r - l > 1) {
            ll mx = (l + r) >> 1;
            if (is_dot_in_polgon(p, polygons[mx])) {
                r = mx;
            } else {
                l = mx;
            }
        }
        if (r != n) {
            used[r] = true;
        }
    }
    vector<ld> areas;
    for (auto el: polygons) {
        areas.push_back(polygon_area(el));
//        cout << areas.back() << '\n';
    }
    ld ans = 0;
    for (ll i = 0; i < n; i++) {
        if (used[i]) {
//            cout << i << '\n';
//            cout << areas[i] - (i == 0 ? 0.0 : areas[i - 1]) << '\n';
            ans += areas[i] - (i == 0 ? 0.0 : areas[i - 1]);
        }
    }
    cout << ans;
}
