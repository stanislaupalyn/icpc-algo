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

/*
https://asfjwd.github.io/2020-04-24-floor-sum-ap/

https://atcoder.jp/contests/practice2/tasks/practice2_c
// sum of floor((ax+b)/c) for x=0..n 

1) a >= c or b >= c
transition to (a mod c, b mod c)
just rewrite ax+b into (a//c + a%c) * x + b//c + b%c


2) a < c and b < c
Goal: swap a and c and apply 1)
Rewrite such way, that we count how each point contributes to the answer
*/

ll floorSum(ll a, ll b, ll c, ll n) {
    if (a == 0) {
        return (b / c) * (n + 1);
    }

    if (a >= c || b >= c) {
        return n * (n + 1) / 2 * (a / c) + (n + 1) * (b / c) + floorSum(a % c, b % c, c, n);
    }

    ll m = (a * n + b) / c;

    return m * n - floorSum(c, c - b - 1, a, m - 1);
}

void solve() {
    ll a, b, c, n;
    cin >> n >> c >> a >> b;
    n--;

    cout << floorSum(a, b, c, n) << "\n";
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