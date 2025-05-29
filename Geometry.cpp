#include <bits/stdc++.h>

#define fr(i, a, b) for (int i = (a); i <= (b); ++i)
#define rf(i, a, b) for (int i = (a); i >= (b); --i)
#define fe(x, y) for (auto& x : y)

#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define mt make_tuple

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define pw(x) (1LL << (x))

using namespace std;

#ifdef LOCAL
mt19937_64 rng(228);
#else
mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());
#endif

#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
template <typename T>
using oset = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
#define fbo find_by_order
#define ook order_of_key

template <typename T>
bool umn(T& a, T b) {
    return a > b ? a = b, 1 : 0;
}
template <typename T>
bool umx(T& a, T b) {
    return a < b ? a = b, 1 : 0;
}

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template <typename T>
using ve = vector<T>;

ll rnd(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
}

/*
https://github.com/kth-competitive-programming/kactl
*/

// Point.h
template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
    typedef Point P;
    T x, y;
    explicit Point(T x=0, T y=0) : x(x), y(y) {}
    bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
    bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
    P operator+(P p) const { return P(x+p.x, y+p.y); }
    P operator-(P p) const { return P(x-p.x, y-p.y); }
    P operator*(T d) const { return P(x*d, y*d); }
    P operator/(T d) const { return P(x/d, y/d); }
    T dot(P p) const { return x*p.x + y*p.y; }
    T cross(P p) const { return x*p.y - y*p.x; }
    T cross(P a, P b) const { return (a-*this).cross(b-*this); }
    T dist2() const { return x*x + y*y; }
    double dist() const { return sqrt((double)dist2()); }
    // angle to x-axis in interval [-pi, pi]
    double angle() const { return atan2(y, x); }
    P unit() const { return *this/dist(); } // makes dist()=1
    P perp() const { return P(-y, x); } // rotates +90 degrees
    P normal() const { return perp().unit(); }
    // returns point rotated 'a' radians ccw around the origin
    P rotate(double a) const {
        return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
    friend ostream& operator<<(ostream& os, P p) {
        return os << "(" << p.x << "," << p.y << ")"; }
};

// using P = Point<double>;

// lineDistance.h
// Returns the signed distance between point p and the line con-
// taining points a and b. Positive value on left side and negative
// on right as seen from a towards b. a==b gives nan. P is sup-
// posed to be Point<T> or Point3D<T> where T is e.g. double
// or long long. It uses products in intermediate steps so watch
// out for overflow if using int or long long. Using Point3D will
// always give a non-negative distance. For Point3D, call .dist
// on the result of the cross product.
template<class P>
double lineDist(const P& a, const P& b, const P& p) {
    return (double)(b-a).cross(p-a)/(b-a).dist();
}

// SegmentDistance.h
// returns the shortest distance between point p 
// and the line segment from point s to e.
template<typename P = Point<double>>
double segDist(P& s, P& e, P& p) {
    if (s==e) return (p-s).dist();
    auto d = (e-s).dist2(), t = min(d,max(.0,(p-s).dot(e-s)));
    return ((p-s)*d-(e-s)*t).dist()/d;
}

// onSegment.h
// Returns true iff p lies on the line segment from s to e. Use
// (segDist(s,e,p)<=epsilon) instead when using Point<double>.
template<class P> 
bool onSegment(P s, P e, P p) {
    return p.cross(s, e) == 0 && (s - p).dot(e - p) <= 0;
}

// SegmentIntersection.h
// If a unique intersection point between the line segments going
// from s1 to e1 and from s2 to e2 exists then it is returned.
// If no intersection point exists an empty vector is returned.
// If infinitely many exist a vector with 2 elements is returned,
// containing the endpoints of the common line segment. The
// wrong position will be returned if P is Point<ll> and the in-
// tersection point does not have integer coordinates. Products
// of three coordinates are used in intermediate steps so watch
// out for overflow if using int or long long.
template<class P> vector<P> segInter(P a, P b, P c, P d) {
    auto oa = c.cross(d, a), ob = c.cross(d, b),
         oc = a.cross(b, c), od = a.cross(b, d);
    // Checks if intersection is single non-endpoint point.
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0)
        return {(a * ob - b * oa) / (ob - oa)};
    set<P> s;
    if (onSegment(c, d, a)) s.insert(a);
    if (onSegment(c, d, b)) s.insert(b);
    if (onSegment(a, b, c)) s.insert(c);
    if (onSegment(a, b, d)) s.insert(d);
    return {all(s)};
}

// lineIntersection.h
// If a unique intersection point of the lines going through s1,e1
// and s2,e2 exists {1, point}is returned. If no intersection point
// exists {0, (0,0)}is returned and if infinitely many exists {-1,
// (0,0)} is returned. The wrong position will be returned if P
// is Point<ll>and the intersection point does not have integer
// coordinates. Products of three coordinates are used in inter-
// mediate steps so watch out for overflow if using int or ll.
template<class P>
pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
    auto d = (e1 - s1).cross(e2 - s2);
    if (d == 0) // if parallel
        return {-(s1.cross(e1, s2) == 0), P(0, 0)};
    auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
    return {1, (s1 * p + e1 * q) / d};
}


// sideOf.h
// Returns where pis as seen from s towards e. 1/0/-1 left/on
// line/right. If the optional argument eps is given 0 is returned if p is within
// distance eps from the line. P is supposed to be Point<T> where T is e.g.
// double or long long. It uses products in intermediate steps so watch out for
// overflow if using int or long long.
template<class P>
int sideOf(P s, P e, P p) { return sgn(s.cross(e, p)); }

template<class P>
int sideOf(const P& s, const P& e, const P& p, double eps) {
    auto a = (e-s).cross(p-s);
    double l = (e-s).dist()*eps;
    return (a > l) - (a < -l);
}

// linearTransformation.h
// Apply the linear transformation (translation, rotation and scaling) 
// which takes line p0-p1 to line q0-q1 to point r.
template<typename P = Point<double>>
P linearTransformation(const P& p0, const P& p1,
        const P& q0, const P& q1, const P& r) {
    P dp = p1-p0, dq = q1-q0, num(dp.cross(dq), dp.dot(dq));
    return q0 + P((r-p0).cross(num), (r-p0).dot(num))/dp.dist2();
}

// Angle.h
// point and and a number of rotations around the origin
struct Angle {
    int x, y;
    int t;
    Angle(int x, int y, int t=0) : x(x), y(y), t(t) {}
    Angle operator-(Angle b) const { return {x-b.x, y-b.y, t}; }
    int half() const {
        assert(x || y);
        return y < 0 || (y == 0 && x < 0);
    }
    Angle t90() const { return {-y, x, t + (half() && x >= 0)}; }
    Angle t180() const { return {-x, -y, t + half()}; }
    Angle t360() const { return {x, y, t + 1}; }
};
bool operator<(Angle a, Angle b) {
    // add a.dist2() and b.dist2() to also compare distances
    return make_tuple(a.t, a.half(), a.y * (ll)b.x) <
           make_tuple(b.t, b.half(), a.x * (ll)b.y);
}

// Given two points, this calculates the smallest angle between
// them, i.e., the angle that covers the defined line segment.
pair<Angle, Angle> segmentAngles(Angle a, Angle b) {
    if (b < a) swap(a, b);
    return (b < a.t180() ?
            make_pair(a, b) : make_pair(b, a.t360()));
}
Angle operator+(Angle a, Angle b) { // point a + vector b
    Angle r(a.x + b.x, a.y + b.y, a.t);
    if (a.t180() < r) r.t--;
    return r.t180() < a ? r.t360() : r;
}
Angle angleDiff(Angle a, Angle b) { // angle b - angle a
    int tu = b.t - a.t; a.t = b.t;
    return {a.x*b.x + a.y*b.y, a.x*b.y - a.y*b.x, tu - (b < a)};
}

// CircleIntersection.h
// Computes the pair of points at which two circles intersect.
// Returns false in case of no intersection.
template<typename P = Point<double>>
bool circleInter(P a,P b,double r1,double r2,pair<P, P>* out) {
    if (a == b) { assert(r1 != r2); return false; }
    P vec = b - a;
    double d2 = vec.dist2(), sum = r1+r2, dif = r1-r2,
           p = (d2 + r1*r1 - r2*r2)/(d2*2), h2 = r1*r1 - p*p*d2;
    if (sum*sum < d2 || dif*dif > d2) return false;
    P mid = a + vec*p, per = vec.perp() * sqrt(fmax(0, h2) / d2);
    *out = {mid + per, mid - per};
    return true;
}

// CircleTangents.h
/*
 *   Finds the external tangents of two circles, or internal if r2 is negated.
 *   Can return 0, 1, or 2 tangents -- 0 if one circle contains the other (or overlaps it, in the internal case, or if the circles are the same);
 *   1 if the circles are tangent to each other (in which case .first = .second and the tangent line is perpendicular to the line between the centers).
 *   .first and .second give the tangency points at circle 1 and 2 respectively.
 *   To find the tangents of a circle with a point set r2 to 0.
 */
template<class P>
vector<pair<P, P>> tangents(P c1, double r1, P c2, double r2) {
    P d = c2 - c1;
    double dr = r1 - r2, d2 = d.dist2(), h2 = d2 - dr * dr;
    if (d2 == 0 || h2 < 0)  return {};
    vector<pair<P, P>> out;
    for (double sign : {-1, 1}) {
        P v = (d * dr + d.perp() * sqrt(h2) * sign) / d2;
        out.push_back({c1 + v * r1, c2 + v * r2});
    }
    if (h2 == 0) out.pop_back();
    return out;
}

// CirclePolygonIntersection.h
// Returns the area of the intersection of a circle with a ccw polygon.
template <typename P>
double arg(P p, P q) {
    return atan2(p.cross(q), p.dot(q));
}
template<typename P = Point<double>>
double circlePoly(P c, double r, vector<P> ps) {
    auto tri = [&](P p, P q) {
        auto r2 = r * r / 2;
        P d = q - p;
        auto a = d.dot(p)/d.dist2(), b = (p.dist2()-r*r)/d.dist2();
        auto det = a * a - b;
        if (det <= 0) return arg(p, q) * r2;
        auto s = max(0., -a-sqrt(det)), t = min(1., -a+sqrt(det));
        if (t < 0 || 1 <= s) return arg(p, q) * r2;
        P u = p + d * s, v = q + d * (t-1);
        return arg(p,u) * r2 + u.cross(v)/2 + arg(v,q) * r2;
    };
    auto sum = 0.0;
    fr(i,0,sz(ps)-1)
        sum += tri(ps[i] - c, ps[(i + 1) % sz(ps)] - c);
    return sum;
}

// circumcircle.h
// The circumcirle of a triangle is the circle intersecting all
// three vertices. ccRadius returns the radius of the circle going
// through points A, B and C and ccCenter returns the center
// of the same circle.
template<typename P = Point<double>>
double ccRadius(const P& A, const P& B, const P& C) {
    return (B-A).dist()*(C-B).dist()*(A-C).dist()/
            abs((B-A).cross(C-A))/2;
}
template<typename P = Point<double>>
P ccCenter(const P& A, const P& B, const P& C) {
    P b = C-A, c = B-A;
    return A + (b*c.dist2()-c*b.dist2()).perp()/b.cross(c)/2;
}

// minimumEnclosingCircle.h
// Computes the minimum circle that encloses a set of points.
template<typename P = Point<double>>
pair<P, double> mec(vector<P> ps) {
    shuffle(all(ps), mt19937(time(0)));
    P o = ps[0];
    double r = 0, EPS = 1 + 1e-8;
    fr(i,0,sz(ps)-1) if ((o - ps[i]).dist() > r * EPS) {
        o = ps[i], r = 0;
        fr(j,0,i-1) if ((o - ps[j]).dist() > r * EPS) {
            o = (ps[i] + ps[j]) / 2;
            r = (o - ps[i]).dist();
            fr(k,0,j-1) if ((o - ps[k]).dist() > r * EPS) {
                o = ccCenter(ps[i], ps[j], ps[k]);
                r = (o - ps[i]).dist();
            }
        }
    }
    return {o, r};
}

// insidePolygon.h
// Returns true if p lies within the polygon. If strict is true, it
// returns false for points on the boundary. The algorithm uses products in
// intermediate steps so watch out for overflow.
template<class P>
bool inPolygon(vector<P> &p, P a, bool strict = true) {
    int cnt = 0, n = sz(p);
    fr(i,0,n-1) {
        P q = p[(i + 1) % n];
        if (onSegment(p[i], q, a)) return !strict;
        //or: if (segDist(p[i], q, a) <= eps) return !strict;
        cnt ^= ((a.y<p[i].y) - (a.y<q.y)) * a.cross(p[i], q) > 0;
    }
    return cnt;
}

// polygonArea.h
template<class T>
T polygonArea2(vector<Point<T>>& v) {
    T a = v.back().cross(v[0]);
    fr(i,0,sz(v)-2) a += v[i].cross(v[i+1]);
    return a;
}

// polygonCenter.h
// Returns the center of mass for a polygon.
template<typename P = Point<double>>
P polygonCenter(const vector<P>& v) {
    P res(0, 0); double A = 0;
    for (int i = 0, j = sz(v) - 1; i < sz(v); j = i++) {
        res = res + (v[i] + v[j]) * v[j].cross(v[i]);
        A += v[j].cross(v[i]);
    }
    return res / A / 3;
}

// polygonCut.h
// Returns a vector with the vertices of a polygon with every-
// thing to the left of the line going from s to e cut away.
template<typename P = Point<double>>
vector<P> polygonCut(const vector<P>& poly, P s, P e) {
    vector<P> res;
    fr(i,0,sz(poly)-1) {
        P cur = poly[i], prev = i ? poly[i-1] : poly.back();
        auto a = s.cross(e, cur), b = s.cross(e, prev);
        if ((a < 0) != (b < 0))
            res.push_back(cur + (prev - cur) * (a / (a - b)));
        if (a < 0)
            res.push_back(cur);
    }
    return res;
}


// convexHull.h
// Returns a vector of the points of the convex hull in counter-
// clockwise order. Points on the edge of the hull between two
// other points are not considered part of the hull.
template<typename P = Point<ll>>
vector<P> convexHull(vector<P> pts) {
    if (sz(pts) <= 1) return pts;
    sort(all(pts));
    vector<P> h(sz(pts)+1);
    int s = 0, t = 0;
    for (int it = 2; it--; s = --t, reverse(all(pts)))
        for (P p : pts) {
            while (t >= s + 2 && h[t-2].cross(h[t-1], p) <= 0) t--;
            h[t++] = p;
        }
    return {h.begin(), h.begin() + t - (t == 2 && h[0] == h[1])};
}

// hullDiameter.h
// Returns the two points with max distance on a convex hull
// (ccw, no duplicate/collinear points).
template<typename P = Point<ll>>
array<P, 2> hullDiameter(vector<P> S) {
    int n = sz(S), j = n < 2 ? 0 : 1;
    pair<ll, array<P, 2>> res({0, {S[0], S[0]}});
    fr(i,0,j-1)
        for (;; j = (j + 1) % n) {
            res = max(res, {(S[i] - S[j]).dist2(), {S[i], S[j]}});
            if ((S[(j + 1) % n] - S[j]).cross(S[i + 1] - S[i]) >= 0)
                break;
        }
    return res.second;
}

// pointInsideHull.h
// Determine whether a point t lies inside a convex hull (CCW
// order, with no collinear points). Returns true if point lies within the hull. If
// strict is true, points on the boundary aren’t included.
template <typename P = Point<ll>>
bool inHull(const vector<P>& l, P p, bool strict = true) {
    int a = 1, b = sz(l) - 1, r = !strict;
    if (sz(l) < 3) return r && onSegment(l[0], l.back(), p);
    if (sideOf(l[0], l[a], l[b]) > 0) swap(a, b);
    if (sideOf(l[0], l[a], p) >= r || sideOf(l[0], l[b], p)<= -r)
        return false;
    while (abs(a - b) > 1) {
        int c = (a + b) / 2;
        (sideOf(l[0], l[c], p) > 0 ? b : a) = c;
    }
    return sgn(l[a].cross(l[b], p)) < r;
}

// Line-convex polygon intersection. The polygon must be ccw and have no collinear points.
//   lineHull(line, poly) returns a pair describing the intersection of a line with the polygon:
//    
//   \item $(-1, -1)$ if no collision,
//   \item $(i, -1)$ if touching the corner $i$,
//   \item $(i, i)$ if along side $(i, i+1)$,
//   \item $(i, j)$ if crossing sides $(i, i+1)$ and $(j, j+1)$.
//    
//    In the last case, if a corner $i$ is crossed, this is treated as happening on side $(i, i+1)$.
//    The points are returned in the same order as the line hits the polygon.
//   \texttt{extrVertex} returns the point of a hull with the max projection onto a line.
#define cmp(i,j) sgn(dir.perp().cross(poly[(i)%n]-poly[(j)%n]))
#define extr(i) cmp(i + 1, i) >= 0 && cmp(i, i - 1 + n) < 0
template <class P> int extrVertex(vector<P>& poly, P dir) {
    int n = sz(poly), lo = 0, hi = n;
    if (extr(0)) return 0;
    while (lo + 1 < hi) {
        int m = (lo + hi) / 2;
        if (extr(m)) return m;
        int ls = cmp(lo + 1, lo), ms = cmp(m + 1, m);
        (ls < ms || (ls == ms && ls == cmp(lo, m)) ? hi : lo) = m;
    }
    return lo;
}

#define cmpL(i) sgn(a.cross(poly[i], b))
template <class P>
array<int, 2> lineHull(P a, P b, vector<P>& poly) {
    int endA = extrVertex(poly, (a - b).perp());
    int endB = extrVertex(poly, (b - a).perp());
    if (cmpL(endA) < 0 || cmpL(endB) > 0)
        return {-1, -1};
    array<int, 2> res;
    fr(i,0,1) {
        int lo = endB, hi = endA, n = sz(poly);
        while ((lo + 1) % n != hi) {
            int m = ((lo + hi + (lo < hi ? 0 : n)) / 2) % n;
            (cmpL(m) == cmpL(endB) ? lo : hi) = m;
        }
        res[i] = (lo + !cmpL(hi)) % n;
        swap(endA, endB);
    }
    if (res[0] == res[1]) return {res[0], -1};
    if (!cmpL(res[0]) && !cmpL(res[1]))
        switch ((res[0] - res[1] + sz(poly) + 1) % sz(poly)) {
            case 0: return {res[0], res[0]};
            case 2: return {res[1], res[1]};
        }
    return res;
}

// closestPair.h
template<typename P = Point<ll>>
pair<P, P> closest(vector<P> v) {
    assert(sz(v) > 1);
    set<P> S;
    sort(all(v), [](P a, P b) { return a.y < b.y; });
    pair<ll, pair<P, P>> ret{LLONG_MAX, {P(), P()}};
    int j = 0;
    for (P p : v) {
        P d{1 + (ll)sqrt(ret.first), 0};
        while (v[j].y <= p.y - d.x) S.erase(v[j++]);
        auto lo = S.lower_bound(p - d), hi = S.upper_bound(p + d);
        for (; lo != hi; ++lo)
            ret = min(ret, {(*lo - p).dist2(), {*lo, p}});
        S.insert(p);
    }
    return ret.second;
}

// sphericalDistance.h
// Returns the shortest distance on the sphere with radius radius between the points
// with azimuthal angles (longitude) f1 ($\phi_1$) and f2 ($\phi_2$) from x axis and zenith angles
// (latitude) t1 ($\theta_1$) and t2 ($\theta_2$) from z axis (0 = north pole). All angles measured
// in radians. The algorithm starts by converting the spherical coordinates to cartesian coordinates
// so if that is what you have you can use only the two last rows. dx*radius is then the difference
// between the two points in the x direction and d*radius is the total distance between the points.
double sphericalDistance(double f1, double t1,
        double f2, double t2, double radius) {
    double dx = sin(t2)*cos(f2) - sin(t1)*cos(f1);
    double dy = sin(t2)*sin(f2) - sin(t1)*sin(f1);
    double dz = cos(t2) - cos(t1);
    double d = sqrt(dx*dx + dy*dy + dz*dz);
    return radius*2*asin(d/2);
}

ll countIntegerPoints(Point<ll> A, Point<ll> B) {
    auto C = B - A;
    if (C == Point<ll>(0, 0)) {
        return 0;
    }
    ll g = __gcd(C.x, C.y);
    C.x /= g;
    C.y /= g;
    if (C.x != 0) {
        return abs((B - A).x / C.x);
    }
    return abs((B - A).y / C.y);
}

// https://cses.fi/problemset/task/2193

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif
    int n;
    cin >> n;
    ve<Point<ll>> a(n);
    fr (i, 0, n - 1) {
        cin >> a[i].x >> a[i].y;
    }

    ll b = 0;
    fr (i, 0, n - 1) {
        b += countIntegerPoints(a[i], a[(i + 1) % n]);
    }

    ll A = abs(polygonArea2(a));

    assert((A - b) % 2 == 0);
    ll i = (A - b) / 2 + 1;

    cout << i << " " << b << "\n";

    return 0;
}
