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

using base = complex<double>;

const double PI = acos(-1);

/*
https://judge.yosupo.jp/submission/289671

NTT variant: https://judge.yosupo.jp/submission/259429
*/

template<int LOG = 22>
struct FFT {
    ve<int> rev[LOG + 1];
    ve<base> g[LOG + 1];

    FFT() {
        fr (lvl, 0, LOG) {
            // change to root = bpow(3, (MOD - 1) / N) in NTT case
            // as well as all operations by modulo
            base w{cos(2 * PI / pw(lvl)), sin(2 * PI / pw(lvl))};
            g[lvl].resize(pw(lvl));

            g[lvl][0] = 1;
            fr (i, 1, pw(lvl) - 1) {
                g[lvl][i] = g[lvl][i - 1] * w;
            }

            rev[lvl].resize(pw(lvl));
            if (lvl > 0) {
                fr (i, 0, pw(lvl) - 1) {
                    rev[lvl][i] = rev[lvl - 1][i >> 1] + (i & 1) * pw(lvl - 1);
                }
            }
        }
    }

    ve<base> fft(ve<base> a, bool inv = 0) {
        int p = __lg(sz(a));
        fr (i, 0, sz(a) - 1) {
            if (i < rev[p][i]) {
                swap(a[i], a[rev[p][i]]);
            }
        }

        fr (lvl, 1, p) {
            for (int i = 0; i < sz(a); i += pw(lvl)) {
                fr (j, 0, pw(lvl - 1) - 1) {
                    auto u = a[i + j];
                    auto v = a[i + j + pw(lvl - 1)];
                    a[i + j] = u + g[lvl][j] * v;
                    a[i + j + pw(lvl - 1)] = u - g[lvl][j] * v;
                }
            }
        }

        if (inv) {
            reverse(a.begin() + 1, a.end());
            fe (x, a) x /= sz(a);
        }

        return a;
    }

    ve<ll> multiply(ve<ll> a, ve<ll> b) {
        int p = 1;
        while (p < sz(a) + sz(b) - 1) p <<= 1;

        ve<base> ca(p), cb(p);
        copy(all(a), ca.begin());
        copy(all(b), cb.begin());

        auto va = fft(ca);
        auto vb = fft(cb);
        fr (i, 0, p - 1) {
            va[i] *= vb[i];
        }

        va = fft(va, 1);

        ve<ll> ans(p);
        fr (i, 0, p - 1) {
            ans[i] = round(va[i].real());
        }
        while (sz(ans) > 1 && ans.back() == 0) ans.pop_back();
        return ans;
    }
};

FFT T;

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

    ve<ll> A, B;
    rf(i, sz(a) - 1, 0) { A.pb(a[i] - '0'); }
    rf(i, sz(b) - 1, 0) { B.pb(b[i] - '0'); }

    auto res = T.multiply(A, B);

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