 

int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(11);
    ll n;
    cin >> n;
    vector<r> a(n);
    for (auto &el: a) cin >> el;
    a = graham_convex_hull(a);
    cout << a.size() << '\n';
    for (auto el: a) cout << el << '\n';
    cout << polygon_area(a);

}
