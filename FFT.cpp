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

using base = int;

const double PI = acos(-1);

constexpr int MOD[] = {998244353, 167772161, 377487361};

int bpow(int a, int b, int mod) {
    int res = 1;
    while (b) {
        if (b & 1) {
            res = 1LL * res * a % mod;
        }
        a = 1LL * a * a % mod;
        b >>= 1;
    }
    return res;
}

// https://codeforces.com/group/1rv4rhCsHp/contest/613054/problem/C
template <int LOG, int MOD>
struct FFT {
    ve<int> rev[LOG + 1];
    ve<int> G[LOG + 1];
    FFT() {
        int N = pw(LOG);
        assert((MOD - 1) % N == 0);

        ve<int> d;

        for (int i = 2; i * i <= MOD - 1; i++) {
            if ((MOD - 1) % i == 0) {
                d.pb(i);
                if ((MOD - 1) / i != i) {
                    d.pb((MOD - 1) / i);
                }
            }
        }

        auto check = [&](int x) {
            fe (y, d) {
                if (bpow(x, y, MOD) == 1) {
                    return 0;
                }
            }
            return 1;
        };

        int g = -1;
        fr (i, 2, MOD - 1) {
            if (check(i)) {
                g = i;
                break;
            }
        }

        int root = bpow(g, (MOD - 1) / N, MOD);
        int start = root;

        rf (lvl, LOG, 0) {
            int tot = pw(lvl);
            G[lvl].resize(tot);

            int cur = 1;
            fr (i, 0, tot - 1) {
                G[lvl][i] = cur;
                cur = 1LL * cur * start % MOD;
            }

            start = 1LL * start * start % MOD;
        }

        fr (lvl, 0, LOG) {
            int tot = pw(lvl);
            rev[lvl].resize(tot);
            fr (i, 1, tot - 1) {
                rev[lvl][i] = ((i & 1) << (lvl - 1)) | (rev[lvl][i >> 1] >> 1);
            }
        }
    }
    void fft(ve<int>& a, int sz, bool invert) {
        int n = pw(sz);
        // assert((MOD - 1) % n == 0);

        fr (i, 0, n - 1) {
            if (i < rev[sz][i]) {
                swap(a[i], a[rev[sz][i]]);
            }
        }

        int u, v;
        fr (lvl, 1, sz) {
            for (int i = 0; i < n; i += pw(lvl)) {
                fr (j, i, i + pw(lvl - 1) - 1) {
                    u = a[j];
                    v = 1LL * a[j + pw(lvl - 1)] * G[lvl][j - i] % MOD;
                    a[j] = u + v;
                    if (a[j] >= MOD) a[j] -= MOD;
                    a[j + pw(lvl - 1)] = u - v;
                    if (a[j + pw(lvl - 1)] < 0) a[j + pw(lvl - 1)] += MOD;
                }
            }
        }

        if (invert) {
            reverse(a.begin() + 1, a.end());
            int inv = bpow(n, MOD - 2, MOD);
            fr (i, 0, n - 1) {
                a[i] = 1LL * a[i] * inv % MOD;
            }
        }
    }

    ve<int> multiply(const ve<int>& a, const ve<int>& b) {
        ve<int> fa(a.begin(), a.end());
        ve<int> fb(b.begin(), b.end());

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
        fr (i, 0, SZ - 1) {
            fa[i] = 1LL * fa[i] * fb[i] % MOD;
        }

        fft(fa, sz, true); 
        ve<int> res(SZ);
        for (int i = 0; i < SZ; i++) {
            res[i] = fa[i];
        }
        return res;
    }
};


__int128 gcdex(__int128 a, __int128 b, __int128& x, __int128& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    __int128 x1, y1;
    __int128 g = gcdex(b, a % b, x1, y1);

    x = y1;
    y = x1 - a / b * y1;
    assert(a * x + b * y == g);

    return g;
}

__int128 crt(__int128 a1, __int128 m1, __int128 a2, __int128 m2) {
    assert(0 <= a1 && a1 < m1);
    assert(0 <= a2 && a2 < m2);

    __int128 t1, t2;
    gcdex(m1, m2, t1, t2);
    // x * m1 + y * m2 == 1

    assert(__int128(t1) * m1 + __int128(t2) * m2 == 1);

    __int128 x = t1;
    __int128 y = t2;
    y = -y;
    x *= a2 - a1;
    y *= a2 - a1;

    y %= m1 * m2;
    __int128 val = a2 + y * m2;

    if (val < 0) {
        val += (-val + m1 * m2 - 1) / (m1 * m2) * (m1 * m2);
    }
    assert(val >= 0);

    assert(val % m1 == a1);
    assert(val % m2 == a2);

    return val % (m1 * m2);
}

FFT<22, MOD[0]> F1;
FFT<22, MOD[1]> F2;

const int N = 2e6 + 5;

int n, m;
int a[N];
int b[N];
int ans[2][N];

void dc(int l, int r) {
    if (l == r) {
        return;
    }

    int mid = l + ((r - l) >> 1);

    ve<int> u;
    ve<int> v;
    fr (i, l, mid) u.pb(a[i]);
    fr (i, mid + 1, r) u.pb(0);

    fr (i, l, mid) v.pb(0);
    fr (i, mid + 1, r) v.pb(b[i]);

    ve<int> res[2];
    res[0] = F1.multiply(u, v);
    res[1] = F2.multiply(u, v);

    fr (who, 0, 1) {
        fr (i, 0, sz(res[who]) - 1) {
            ans[who][i + 2 * l] += res[who][i];
            if (ans[who][i + 2 * l] >= MOD[who]) {
                ans[who][i + 2 * l] -= MOD[who];
            }
        }
    }

    dc(l, mid);
    dc(mid + 1, r);
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
    fr (i, 0, n - 1) {
        cin >> a[i];
    }

    cin >> m;
    fr (i, 0, m - 1) {
        cin >> b[i];
    }

    int tot = n + m - 1;

    while (n < m) {
        a[n] = 0;
        n++;
    }
    while (m < n) {
        b[m] = 0;
        m++;
    }

    dc(0, n - 1);

    fr (i, 0, tot - 1) {
        if (i > 0) {
            cout << ll(crt(ans[0][i], MOD[0], ans[1][i], MOD[1])) << " ";
        } else {
            cout << "0 ";
        }
    }
    cout << "\n";

    return 0;
}
