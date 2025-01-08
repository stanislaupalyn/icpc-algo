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

const int N = 3e5 + 5;
const int A = 26;

#define link abc
#define next def

int sz;
int link[N];
int next[N][A];
int len[N];
int last;

int occ[N];

/*
TASK: https://cses.fi/problemset/task/2109

Debug on abcbc
Should be:
1 2 a
1 6 b
1 8 c
2 3 b
3 4 c
4 5 b
5 7 c
6 8 c
8 5 b
link: 0 1 6 8 6 1 8 1
*/

void init() {
    link[1] = 0;
    fr(i, 0, A - 1) { next[1][i] = 0; }
    last = 1;
    len[1] = 0;
    sz = 1;
}

void extend(int c) {
    int cur = ++sz;

    len[cur] = len[last] + 1;
    fr(i, 0, A - 1) { next[cur][i] = 0; }
    int p = last;
    last = cur;

    while (p && !next[p][c]) {
        next[p][c] = cur;
        p = link[p];
    }

    if (!p) {
        link[cur] = 1;
        return;
    }

    int q = next[p][c];
    if (len[q] == len[p] + 1) {
        link[cur] = q;
        return;
    }

    int clone = ++sz;
    len[clone] = len[p] + 1;
    link[clone] = link[q];
    fr(i, 0, A - 1) { next[clone][i] = next[q][i]; }

    link[cur] = clone;
    link[q] = clone;

    while (p && next[p][c] == q) {
        next[p][c] = clone;
        p = link[p];
    }
}

string s;
ll k;

ll dp[N];
bool vis[N];

void dfs1(int v = 1) {
    if (vis[v]) {
        return;
    }
    vis[v] = 1;

    dp[v] = occ[v];
    fr(i, 0, A - 1) {
        if (next[v][i]) {
            dfs1(next[v][i]);
            dp[v] += dp[next[v][i]];
        }
    }
}

string ans;

void dfs2(int v, ll k) {
    if (k - occ[v] < 0) {
        cout << ans << "\n";
        exit(0);
    }

    fr(i, 0, A - 1) {
        if (!next[v][i]) {
            continue;
        }

        if (k - occ[v] - dp[next[v][i]] >= 0) {
            k -= dp[next[v][i]];
        } else {
            ans.pb(i + 'a');
            dfs2(next[v][i], k - occ[v]);
        }
    }
}

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif

    cin >> s;
    cin >> k;

    init();
    fr(i, 0, sz(s) - 1) { extend(s[i] - 'a'); }

    int v = 1;
    fr(i, 0, sz(s) - 1) {
        v = next[v][s[i] - 'a'];
        occ[v]++;
    }

    {
        fill(vis, vis + 1 + sz, 0);
        queue<int> q;
        q.push(1);
        ve<int> ord;
        while (sz(q)) {
            int v = q.front();
            q.pop();
            ord.pb(v);

            fr(i, 0, 25) {
                if (next[v][i] && !vis[next[v][i]]) {
                    vis[next[v][i]] = 1;
                    q.push(next[v][i]);
                }
            }
        }
        reverse(all(ord));

        fe(v, ord) { occ[link[v]] += occ[v]; }
    }

    fill(vis, vis + 1 + sz, 0);
    dfs1();

    ans.clear();

    k += sz(s) - 1;   // empty substrings
    dfs2(1, k);

    return 0;
}