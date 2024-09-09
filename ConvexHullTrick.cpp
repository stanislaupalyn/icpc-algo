#include <bits/stdc++.h>

#define fr(i, a, b) for (int i = (a); i <= (b); i++)
#define rf(i, a, b) for (int i = (a); i >= (b); i--)
#define fe(x, y) for (auto& x : y)

#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define mt make_tuple

#define pw(x) (1LL << (x))
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()

using namespace std;

mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());

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
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template <typename T>
using ve = vector<T>;

int n;
ve<pii> a, b;

int sign(int x) {
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

struct ConvexHullTrick {
    struct Line {
        ll k, b;
        ll operator()(ll x) {
            return k * x + b;
        }
    };

    ve<ll> dots;
    ve<Line> hull;

    ll intersect(Line a, Line b) {
        ll x = (b.b - a.b) / (a.k - b.k);
        if (abs(b.b - a.b) % abs(a.k - b.k)) {
            x++;
        }
        return x;
    }

    void add(Line cur) {
        if (sz(hull)) {
            assert(cur.k < hull.back().k);
        }

        while (sz(hull) > 1 && dots.back() >= intersect(cur, hull[sz(hull) - 2])) {
            hull.pop_back();
            dots.pop_back();
        }
        hull.pb(cur);
        if (sz(hull) > 1) {
            dots.pb(intersect(hull[sz(hull) - 1], hull[sz(hull) - 2]));
        }
    }

    ll get(ll x) {
        if (!sz(hull)) {
            return 2e18;
        }
        int p = upper_bound(all(dots), x) - dots.begin() - 1;
        return hull[p + 1](x);
    }
} T;

int rnd(int l, int r) {
    return l + rng() % (r - l + 1);
}

ll brute() {
    ve<ll> dp(sz(b), ll(1e18));

    fr (i, 0, sz(b) - 1) {
        umn(dp[i], 1LL * b[i].se * b[0].fi);

        fr (j, 0, i - 1) {
            umn(dp[i], dp[j] + 1LL * b[i].se * b[j + 1].fi);
        }
    }
    return dp[sz(b) - 1];
}

int main() {
#ifndef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
#else
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#endif

    cin >> n;
    a.resize(n);
    fr (i, 0, n - 1) {
        cin >> a[i].fi >> a[i].se;
    }
    sort(all(a));
    reverse(all(a));

    int mxY = -1e9;
    fr (i, 0, n - 1) {
        if (a[i].se <= mxY) {
            continue;
        }
        mxY = a[i].se;
        b.pb(a[i]);
    }

    T.add(ConvexHullTrick::Line{b[0].fi, 0});

    fr (i, 0, sz(b) - 1) {
        ll go = T.get(b[i].se);
        if (i + 1 < sz(b)) {
            T.add({b[i + 1].fi, go});
        }

        if (i == sz(b) - 1) {
            cout << go << "\n";
            return 0;
        }
    }

    return 0;
}

/*
    dp[i] -> dp[j] (dp[i] + a[i].fi * a[j].se) 
    a[i].fi -- k
    decreasing
*/