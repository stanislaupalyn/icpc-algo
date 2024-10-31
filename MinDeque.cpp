/*
https://codeforces.com/group/DLeDfdBUa8/contest/555223/problem/E

Implementation from https://codeforces.com/blog/entry/122003
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

const int N = 2e4 + 5;

int n;
pii a[N];
ll dp[N], ndp[N];

struct MinStack {
    stack<pair<ll, ll>> st;
    ll getMin() {
        if (empty()) {
            return 2e18;
        }
        return st.top().second;
    }
    bool empty() { return st.empty(); }
    int size() { return st.size(); }
    void push(ll x) {
        ll mn = x;
        if (!empty()) {
            mn = min(mn, getMin());
        }
        st.push({x, mn});
    }
    void pop() { st.pop(); }
    ll top() { return st.top().first; }
    void swap(MinStack &x) { st.swap(x.st); }
    void clear() {
        while (!empty()) {
            pop();
        }
    }
};

struct MinDeque {
    MinStack l, r, t;
    void rebalance() {
        bool f = false;
        if (r.empty()) {
            f = true;
            l.swap(r);
        }
        int sz = r.size() / 2;
        while (sz--) {
            t.push(r.top());
            r.pop();
        }
        while (!r.empty()) {
            l.push(r.top());
            r.pop();
        }
        while (!t.empty()) {
            r.push(t.top());
            t.pop();
        }
        if (f) {
            l.swap(r);
        }
    }
    ll getMin() {
        if (l.empty()) {
            return r.getMin();
        }
        if (r.empty()) {
            return l.getMin();
        }
        return min(l.getMin(), r.getMin());
    }
    bool empty() { return l.empty() && r.empty(); }
    int size() { return l.size() + r.size(); }
    void pushFront(ll x) { l.push(x); }
    void pushBack(ll x) { r.push(x); }
    void popFront() {
        if (l.empty()) {
            rebalance();
        }
        l.pop();
    }
    void popBack() {
        if (r.empty()) {
            rebalance();
        }
        r.pop();
    }
    ll front() {
        if (l.empty()) {
            rebalance();
        }
        return l.top();
    }
    ll back() {
        if (r.empty()) {
            rebalance();
        }
        return r.top();
    }
    void swap(MinDeque &x) {
        l.swap(x.l);
        r.swap(x.r);
    }
    void clear() {
        l.clear();
        r.clear();
    }
};

MinDeque Q[2];

ll brute() {
    fr(i, 0, int(1e4)) {
        dp[i] = 1e18;
        ndp[i] = 1e18;
    }

    dp[0] = 0;

    fr(i, 1, n) {
        fr(was, 0, 10000) {
            if (dp[was] == ll(1e18))
                continue;

            fr(take, 0, min(was, a[i].fi)) {
                if (was + a[i].fi - 2 * take <= 10000) {
                    umn(ndp[was + a[i].fi - 2 * take],
                        dp[was] + 1LL * take * a[i].se);
                }
            }
        }

        rf(now, 9999, 0) { umn(ndp[now], ndp[now + 1] + a[i].se); }

        fr(was, 0, 10000) {
            dp[was] = ndp[was];
            ndp[was] = 1e18;
        }
    }
    return dp[0];
}

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif

    cin >> n;
    fr(i, 1, n) {
        cin >> a[i].fi >> a[i].se;
        // a[i].fi = rnd(1, int(1e4));
        // a[i].se = rnd(1, int(10000000));

        // cout << a[i].fi << " " << a[i].se << "\n";
    }

    sort(a + 1, a + 1 + n, [](pii a, pii b) { return a.se < b.se; });

    fr(i, 0, int(2e4)) {
        dp[i] = 1e18;
        ndp[i] = 1e18;
    }

    dp[0] = 0;

    fr(i, 1, n) {
        rf(was, 20000, a[i].fi) { dp[was] = dp[was - a[i].fi]; }

        rf(was, a[i].fi - 1, 0) { dp[was] = 1e18; }

        fr(was, 0, 20000) { dp[was] += 1LL * (was / 2) * a[i].se; }

        int L = 0;
        int R = -1;
        Q[0].clear();
        Q[1].clear();

        fr(now, 0, 10000) {
            int l = now + max(0, a[i].fi - now) * 2;
            int r = min(20000, now + a[i].fi * 2);

            while (R < r) {
                R++;
                Q[R % 2].pushBack(dp[R]);
            }
            while (L > l) {
                L--;
                Q[L % 2].pushFront(dp[L]);
            }
            while (L < l && L <= R) {
                Q[L % 2].popFront();
                L++;
            }

            if (l <= r) {
                assert(L == l && R == r);
            }

            ll mn = 2e18;
            mn = Q[now % 2].getMin();

            umn(ndp[now], mn - 1LL * (now / 2) * a[i].se);
        }

        rf(now, 9999, 0) { umn(ndp[now], ndp[now + 1] + a[i].se); }

        fr(now, 0, 10000) {
            dp[now] = ndp[now];
            ndp[now] = 1e18;
        }
    }

    cout << dp[0] << "\n";

    return 0;
}