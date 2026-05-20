 

ld triangle_area(r &a, r &b, r &c) {
    ll s = std::abs(r(a, b) ^ r(a, c));
    return (ld) (s >> 1) + (s & 1 ? .5 : 0);
}

ld from_dot_to_line(r p, r a, r b) {
    return std::abs(r(p, a) ^ r(p, b)) / r(a, b).length();
}

ld from_dot_to_ray(r p, r a, r b) {
    if (sign(r(a, b) * r(a, p)) >= 0) return from_dot_to_line(p, a, b);
    return r(p, a).length();
}

ld from_dot_to_segment(r p, r a, r b) {
    if (is_dot_on_segment(p, a, b)) return 0;
    if (sign(r(a, b) * r(a, p)) >= 0 && sign(r(b, a) * r(b, p)) >= 0) {
        return from_dot_to_line(p, a, b);
    }
    return min(r(p, a).length(), r(p, b).length());
}

ld from_segment_to_segment(r a, r b, r c, r d) {
    if (is_segments_intersect(a, b, c, d)) return 0;
    return min(
            min(from_dot_to_segment(a, c, d),
                from_dot_to_segment(b, c, d)),
            min(from_dot_to_segment(c, a, b),
                from_dot_to_segment(d, a, b)));
}

ld from_segment_to_ray(r a, r b, r c, r d) {
    if (is_segment_intersect_ray(a, b, c, d)) return 0;
    return min(from_dot_to_segment(c, a, b),
               min(from_dot_to_ray(a, c, d),
                   from_dot_to_ray(b, c, d)));
}

bool is_dot_in_angle(r p, r a, r o, r b) {
    if (sign(r(o, a) ^ r(o, b)) < 0) {
        swap(a, b);
    }
    ll s1 = sign(r(o, a) ^ r(o, p));
    ll s2 = sign(r(o, p) ^ r(o, b));
    return s1 >= 0 && s2 >= 0;
}

ld from_segment_to_line(r a, r b, r c, r d) {
    if (is_segment_intersect_line(a, b, c, d)) return 0;
    return min(from_dot_to_line(a, c, d),
               from_dot_to_line(b, c, d));
}

ld from_ray_to_ray(r a, r b, r c, r d) {
    if (is_rays_intersect(a, b, c, d)) return 0;
    return min(from_dot_to_ray(a, c, d),
               from_dot_to_ray(c, a, b));
}

ld from_line_to_ray(r a, r b, r c, r d) {
    if (is_ray_intersect_line(c, d, a, b)) return 0;
    return from_dot_to_line(c, a, b);
}


ld from_line_to_line(r a, r b, r c, r d) {
    if (is_lines_intersect(a, b, c, d)) return 0;
    return from_dot_to_line(c, a, b);
}

bool is_dot_in_convex_polygon_n(r p, vector<r> &a) {
    set<ll> was;
    for (ll i = 0; i < (ll) a.size(); i++) {
        if (is_dot_on_segment(p, a[i], a[(i + 1) % a.size()])) { return true; }
        ll s = sign(r(p, a[i]) ^ r(p, a[(i + 1) % a.size()]));
        was.insert(s);
    }
    return was.size() == 1;
}


bool is_dot_in_convex_polygon_logn(r p, vector<r> &a) {
    if (!is_dot_in_angle(p, a[1], a[0], a.back())) {
        return false;
    }
    ll n = (ll) a.size();
    ll left = 0;
    ll right = n - 1;
    while (right - left > 1) {
        ll mx = (right + left) >> 1;
        if (is_dot_in_angle(p, a[left], a[0], a[mx])) {
            right = mx;
        } else {
            left = mx;
        }
    }
    r a1(a[right], a[left]);
    r a2(a[right], p);
    return (a1 ^ a2) < 0;
}

bool is_dot_in_polygon_n(r p, vector<r> &a) {
    ld ans = 0;
    ll n = a.size();
    for (ll i = 0; i < n; i++) {
        if (is_dot_on_segment(p, a[i], a[(i + 1) % n])) {
            return true;
        }
        ans += angle(a[i] - p, a[(i + 1) % n] - p);
    }
    return abs(ans) > PI;
}

ll manhattan_dist(r r1, r r2) {
    return abs(r1.x - r2.x) + abs(r1.y - r2.y);
}

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
