 

ld polygon_area(vector<r> &points) {
    ll s = 0;
    for (ll i = 0; i < (ll) points.size() - 1; i++) {
        s += points[i] ^ points[i + 1];
    }
    s += points.back() ^ points[0];
    s = std::abs(s);
    return (ld) (s >> 1) + (s & 1 ? .5 : 0);
}

ld polygon_perimeter(vector<r> &points) {
    ld s = 0;
    for (ll i = 0; i < (ll) points.size() - 1; i++) {
        s += r(points[i], points[i + 1]).length();
    }
    s += r(points.back(), points[0]).length();
    return s;
}

bool polar_angle_cmp(r a, r b) {
    if (a.hp_pos() != b.hp_pos())
        return a.hp_pos() > b.hp_pos();
    ll s = sign(a ^ b);
    if (s != 0) return s > 0;
    return a.manhattan_length() < b.manhattan_length();
}

vector<r> graham_convex_hull(vector<r> points) {
    r start = *min_element(points.begin(), points.end());
    for (auto &el: points) el = el - start;
    sort(points.begin(), points.end(), [&](r a, r b) {
        return (a ^ b) > 0 || ((a ^ b) == 0 &&
                               manhattan_dist(r(0, 0), a) < manhattan_dist(r(0, 0), b));
    });
    for (auto &el: points) el = el + start;
    vector<r> hull;
    for (r pt: points) {
        while (hull.size() >= 2) {
            r new_vector = pt - hull.back();
            r last_vector = hull.back() - hull[hull.size() - 2];
            if ((new_vector ^ last_vector) < 0) break;
            hull.pop_back();
        }
        hull.push_back(pt);
    }
    return hull;
}

ld diameter_of_convex_polygon(vector<r> a) {
    ll id = 0;
    ll n = a.size();
    for (ll i = 1; i < n; i++) {
        if (r(a[0], a[id]).square_length() < r(a[0], a[i]).square_length())
            id = i;
    }
    ld ans = r(a[0], a[id]).length();
    for (ll i = 0; i < n; i++) {
        r left = r(a[i], a[(i + 1) % n]) * -1;
        r right = r(a[id % n], a[(id + 1) % n]);
        while (sign(right ^ left) > 0) {
            ans = max(ans, r(a[i], a[(id++) % n]).length());
            right = r(a[id % n], a[(id + 1) % n]);
        }
        ans = max(ans, r(a[i], a[id % n]).length());
    }
    return ans;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);
    ll n;
    cin >> n;
    vector<r> a(n);
    for (auto &el: a) cin >> el;
    cout << diameter_of_convex_polygon(graham_convex_hull(a));
}
