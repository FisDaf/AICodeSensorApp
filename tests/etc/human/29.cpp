 

vector<r> jarvis_convex_hull(vector<r> &points) {
    r start = *min_element(points.begin(), points.end());
    vector<r> hull = {start};
    while (true) {
        r next = hull.back();
        for (auto pt: points) {
            ll cross_product = (next - hull.back()) ^ (pt - hull.back());
            ll manh_d1 = manhattan_dist(hull.back(), pt);
            ll manh_d2 = manhattan_dist(hull.back(), next);
            if (next == hull.back() || cross_product < 0) {
                next = pt;
            } else if (cross_product == 0 && manh_d1 > manh_d2) {
                next = pt;
            }
        }
        if (next == start) break;
        hull.push_back(next);
    }
    return hull;
}

ld polygon_area(vector<r> &points) {
    ll s = 0;
    for (ll i = 0; i < points.size() - 1; i++) {
        s += points[i] ^ points[i + 1];
    }
    s += points.back() ^ points[0];
    s = std::abs(s);
    return (ld) (s >> 1) + (s & 1 ? .5 : 0);
}

ld polygon_perimeter(vector<r> &points) {
    ld s = 0;
    for (ll i = 0; i < points.size() - 1; i++) {
        s += r(points[i], points[i + 1]).length();
    }
    s += r(points.back(), points[0]).length();
    return s;
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


int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);
    r a, b, c, d;
    cin >> a >> b >> c >> d;
    cout << r(a, c).length() << '\n'; // 1
    cout << from_dot_to_segment(a, c, d) << '\n'; // 2
    cout << from_dot_to_ray(a, c, d) << '\n'; // 3
    cout << from_dot_to_line(a, c, d) << '\n'; // 4
    cout << from_dot_to_segment(c, a, b) << '\n'; // 5
    cout << from_segment_to_segment(a, b, c, d) << '\n'; // 6
    cout << from_segment_to_ray(a, b, c, d) << '\n'; // 7
    cout << from_segment_to_line(a, b, c, d) << '\n'; // 8
    cout << from_dot_to_ray(c, a, b) << '\n'; // 9
    cout << from_segment_to_ray(c, d, a, b) << '\n'; // 10
    cout << from_ray_to_ray(a, b, c, d) << '\n'; // 11
    cout << from_line_to_ray(c, d, a, b) << '\n'; // 12
    cout << from_dot_to_line(c, a, b) << '\n'; // 13
    cout << from_segment_to_line(c, d, a, b) << '\n'; // 14
    cout << from_line_to_ray(a, b, c, d) << '\n'; // 15
    cout << from_line_to_line(a, b, c, d) << '\n'; // 16
}
