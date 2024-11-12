/*
https://cses.fi/problemset/task/1736
*/

#include <bits/stdc++.h>

#define fr(i, a, b) for (int i = (a); i <= (b); ++i)
#define rf(i, a, b) for (int i = (a); i >= (b); --i)
#define fe(x, y)    for (auto &x : y)

#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define mt make_tuple

#define all(x) (x).begin(), (x).end()
#define sz(x)  (int) (x).size()
#define pw(x)  (1LL << (x))

using namespace std;

#ifdef LOCAL
mt19937_64 rng(228);
#else
mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());
#endif

#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
template <typename T>
using oset =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
#define fbo find_by_order
#define ook order_of_key

template <typename T> bool umn(T &a, T b) { return a > b ? a = b, 1 : 0; }
template <typename T> bool umx(T &a, T b) { return a < b ? a = b, 1 : 0; }

using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template <typename T> using ve = vector<T>;

ll rnd(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }

const int N = 2e5 + 5;

int n, q;
ll a[N];

struct SegmentTreeAP {
    int n;
    ll t[4 * N];
    // a + a * x + a * x^2 + ...
    ll lazyA[4 * N];
    ll lazyX[4 * N];

    ll sum(ll l, ll r) { return r * (r + 1) / 2 - l * (l - 1) / 2; }

    void init(int n) {
        this->n = n;
        build(1, 1, n);
    }

    void build(int v, int tl, int tr) {
        t[v] = 0;
        lazyA[v] = 0;
        lazyX[v] = 0;
        if (tl == tr) {
            // change if needed

            t[v] = a[tl];
            return;
        }
        int tm = (tl + tr) >> 1;
        build(v << 1, tl, tm);
        build(v << 1 | 1, tm + 1, tr);
        t[v] = t[v << 1] + t[v << 1 | 1];
    }

    void build() { build(1, 1, n); }

    void push(int v, int tl, int tr) {
        int tm = (tl + tr) >> 1;

        if (lazyA[v]) {
            ll newA = lazyA[v] + (tm - tl + 1) * lazyX[v];

            t[v << 1] += lazyA[v] * (tm - tl + 1);
            t[v << 1 | 1] += newA * (tr - tm);

            lazyA[v << 1] += lazyA[v];
            lazyA[v << 1 | 1] += newA;
            lazyA[v] = 0;
        }

        if (lazyX[v]) {
            t[v << 1] += sum(0, tm - tl) * lazyX[v];
            t[v << 1 | 1] += sum(0, tr - tm - 1) * lazyX[v];

            lazyX[v << 1] += lazyX[v];
            lazyX[v << 1 | 1] += lazyX[v];
            lazyX[v] = 0;
        }
    }

    ll get(int v, int tl, int tr, int l, int r) {
        if (l > r) {
            return 0;
        }
        if (tl == l && tr == r) {
            return t[v];
        }
        int tm = (tl + tr) >> 1;
        push(v, tl, tr);

        return get(v << 1, tl, tm, l, min(r, tm)) +
               get(v << 1 | 1, tm + 1, tr, max(tm + 1, l), r);
    }

    ll get(int l, int r) { return get(1, 1, n, l, r); }

    void upd(int v, int tl, int tr, int l, int r, ll start, ll step) {
        if (l > r) {
            return;
        }
        if (tl == l && tr == r) {
            lazyA[v] += start;
            t[v] += start * (r - l + 1);

            lazyX[v] += step;
            t[v] += sum(0, r - l) * step;

            return;
        }
        int tm = (tl + tr) >> 1;
        push(v, tl, tr);

        upd(v << 1, tl, tm, l, min(r, tm), start, step);
        upd(v << 1 | 1, tm + 1, tr, max(tm + 1, l), r,
            start + max(0, min(r, tm) - l + 1) * step, step);
        t[v] = t[v << 1] + t[v << 1 | 1];
    }

    void upd(int l, int r, ll start, ll step) {
        upd(1, 1, n, l, r, start, step);
    }
};

SegmentTreeAP T;

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif

    cin >> n >> q;
    fr(i, 1, n) { cin >> a[i]; }
    T.init(n);
    T.build();

    fr(i, 1, q) {
        int t, l, r;
        cin >> t >> l >> r;
        // t = rnd(1, 2);
        // l = rnd(1, n);
        // r = rnd(1, n);
        // if (l > r) {
        //     swap(l, r);
        // }

        // cout << t << " " << l << " " << r << "\n";

        if (t == 1) {
            T.upd(l, r, 1, 1);

            // fr (j, l, r) {
            //     a[j] += j - l + 1;
            // }

        } else {
            // ll sum = 0;
            // fr (j, l, r) sum += a[j];
            // assert(sum == T.get(l, r));

            cout << T.get(l, r) << "\n";
        }
    }

    return 0;
}