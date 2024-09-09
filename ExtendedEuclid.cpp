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
#define pw(x) (1LL << x)

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
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template <typename T>
using ve = vector<T>;

ll rnd(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
}

ll gcd(ll a, ll b, ll& x, ll& y) {
    x = 1, y = 0;
    ll x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1) {
        ll q = a1 / b1;
        tie(x, x1) = make_tuple(x1, x - q * x1);
        tie(y, y1) = make_tuple(y1, y - q * y1);
        tie(a1, b1) = make_tuple(b1, a1 - q * b1);
    }
    return a1;
}

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif

    ll a, b;
    gcd(18, 24, a, b);

    cout << a * 18 + b * 24 << "\n";

    return 0;
}