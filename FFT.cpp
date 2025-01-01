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

using base = complex<ld>;

/*
https://judge.yosupo.jp/submission/259427

NTT variant: https://judge.yosupo.jp/submission/259429
*/

template <int LOG> struct FFT {
    ve<int> rev[LOG + 1];
    ve<base> G[LOG + 1];
    FFT() {
        int N = pw(LOG);

        // change to root = bpow(3, (MOD - 1) / N) in NTT case
        base root(cosl(2 * M_PI / N), sinl(2 * M_PI / N));
        base start = root;

        rf(lvl, LOG, 0) {
            int tot = pw(lvl);
            G[lvl].resize(tot);

            base cur = 1;
            fr(i, 0, tot - 1) {
                G[lvl][i] = cur;
                cur *= start;
            }

            start = start * start;
        }

        fr(lvl, 0, LOG) {
            int tot = pw(lvl);
            rev[lvl].resize(tot);
            fr(i, 1, tot - 1) {
                rev[lvl][i] = ((i & 1) << (lvl - 1)) | (rev[lvl][i >> 1] >> 1);
            }
        }
    }
    void fft(ve<base> &a, int sz, bool invert) {
        int n = pw(sz);
        fr(i, 0, n - 1) {
            if (i < rev[sz][i]) {
                swap(a[i], a[rev[sz][i]]);
            }
        }

        base u, v;
        fr(lvl, 1, sz) {
            for (int i = 0; i < n; i += pw(lvl)) {
                fr(j, i, i + pw(lvl - 1) - 1) {
                    u = a[j];
                    v = a[j + pw(lvl - 1)] * G[lvl][j - i];
                    a[j] = u + v;
                    a[j + pw(lvl - 1)] = u - v;
                }
            }
        }

        if (invert) {
            reverse(a.begin() + 1, a.end());
            fr(i, 0, n - 1) { a[i] /= n; }
        }
    }

    ve<int> multiply(const ve<int> &a, const ve<int> &b) {
        ve<base> fa(a.begin(), a.end());
        ve<base> fb(b.begin(), b.end());
        int n = (int) a.size();
        int m = (int) b.size();

        int sz = 0;
        while (pw(sz) < n + m - 1) {
            sz++;
        }
        fa.resize(pw(sz));
        fb.resize(pw(sz));

        fft(fa, sz, false);
        fft(fb, sz, false);
        int SZ = pw(sz);
        fr(i, 0, SZ - 1) { fa[i] = fa[i] * fb[i]; }

        fft(fa, sz, true);
        ve<int> res(SZ);
        for (int i = 0; i < SZ; i++) {
            res[i] = roundl(fa[i].real());
        }
        return res;
    }
};

FFT<22> F;

void solve() {
    string a, b;
    cin >> a >> b;

    // bool first = 1;
    // while (sz(a) < int(2e6)) {
    //     a.pb('0' + rnd(first ? 1 : 0, 9));
    //     b.pb('0' + rnd(first ? 1 : 0, 9));
    //     first = 0;
    // }
    // cout << a << "\n" << b << "\n";

    int sgn = 1;
    if (a[0] == '-') {
        sgn *= -1;
        a.erase(a.begin());
    }
    if (b[0] == '-') {
        sgn *= -1;
        b.erase(b.begin());
    }

    ve<int> A, B;
    rf(i, sz(a) - 1, 0) { A.pb(a[i] - '0'); }
    rf(i, sz(b) - 1, 0) { B.pb(b[i] - '0'); }

    auto res = F.multiply(A, B);

    fr(i, 0, sz(res) - 2) {
        res[i + 1] += res[i] / 10;
        res[i] %= 10;
    }
    while (sz(res) > 1 && res.back() == 0) {
        res.pop_back();
    }

    if (sgn == -1 && !(sz(res) == 1 && res[0] == 0)) {
        cout << "-";
    }
    rf(i, sz(res) - 1, 0) { cout << res[i]; }
    cout << "\n";
}

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif

    int test;
    cin >> test;

    while (test--) {
        solve();
    }

    return 0;
}