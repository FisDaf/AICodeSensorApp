#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

struct Point {
    double x, y;
};

double distance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
    Point p1, p2, p3;
    cin >> p1.x >> p1.y;
    cin >> p2.x >> p2.y;
    cin >> p3.x >> p3.y;

    double d1 = distance(p1, p2);
    double d2 = distance(p2, p3);
    double d3 = distance(p3, p1);

    double a = d1;
    double b = d2;
    double c = d3;

    double s = (a + b + c) / 2;
    double area_triangle = sqrt(s * (s - a) * (s - b) * (s - c));

    double angle_A = acos((b * b + c * c - a * a) / (2 * b * c));
    double angle_B = acos((a * a + c * c - b * b) / (2 * a * c));
    double angle_C = acos((a * a + b * b - c * c) / (2 * a * b));

    double radius = (a * b * c) / (2 * area_triangle);

    double n = 2 * M_PI / angle_C;
    double area = (n * radius * radius) / 2;

    cout << fixed << setprecision(6) << area << endl;

    return 0;
}
