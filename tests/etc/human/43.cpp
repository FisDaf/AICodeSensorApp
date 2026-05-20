 

bool is_dot_in_polygon(r p, vector<r> &a) {
    if (!is_dot_in_angle(p, a[1], a[0], a.back())) {
        return false;
    }
    ll n = a.size();
    ll l = 1;
    ll r = n - 1;
    while (r - l > 1) {
        ll mx = (r + l) >> 1;
        if (is_dot_in_angle(p, a[l], a[0], a[mx])) {
            r = mx;
        } else {
            l = mx;
        }
    }
    struct r a1(a[l], a[r]);
    struct r a2(a[l], p);
    return (a1 ^ a2) >= 0;
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(11);
    ll n, m, k;
    cin >> n >> m >> k;
    vector<r> a(n);
    for (auto &el: a) cin >> el;
    while (m--) {
        r p;
        cin >> p;
        k -= is_dot_in_polygon(p, a);
    }
    if (k <= 0) {
        cout << "YES";
    } else {
        cout << "NO";
    }
}
