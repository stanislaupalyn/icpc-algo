/*
Offline incrementing SCC

https://codeforces.com/contest/1989/problem/F
*/

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

const int N = 1e6 + 5;

int nVer;
int row[N], col[N];
int n, m, q;

array<int, 3> a[N];

ve<int> uX, uY;

ve<array<int, 3>> edges;

ll brute(int t) {
    ve<ve<int>> g, gr;
    gr.resize(nVer + 1);
    g.resize(nVer + 1);
    fe (cur, edges) {
        if (cur[0] > t) continue;

        g[cur[1]].pb(cur[2]);
        gr[cur[2]].pb(cur[1]);
    }
    ve<int> order;
    ve<bool> vis(nVer + 1);
    function<void(int)> tsort = [&](int v) {
        vis[v] = 1;
        fe (to, g[v]) {
            if (!vis[to]) {
                tsort(to);
            }
        }
        order.pb(v);
    };


    fr (i, 1, nVer) {
        if (!vis[i]) {
            tsort(i);
        }
    }
    reverse(all(order));
    fill(all(vis), 0);

    ve<int> st;

    function<void(int)> dfs = [&](int v) {
        vis[v] = 1;
        st.pb(v);
        fe (to, gr[v]) {
            if (!vis[to]) {
                dfs(to);
            }
        }
    };

    ll ans = 0;
    fe (i, order) {
        if (!vis[i]) {
            st.clear();
            dfs(i);

            if (sz(st) > 1) {
                ans += 1LL * sz(st) * sz(st);
            }
        }
    }
    return ans;
}

ve<int> g[N];
ve<int> gr[N];
bool vis[N];
ve<int> order;

void tsort(int v) {
    vis[v] = 1;
    fe (to, g[v]) {
        if (!vis[to]) {
            tsort(to);
        }
    }
    order.pb(v);
}

ve<int> st;

void dfs(int v) {
    vis[v] = 1;
    st.pb(v);
    fe (to, gr[v]) {
        if (!vis[to]) {
            dfs(to);
        }
    }
}

int who[N];
int sz[N];
ll ans;

void upd(int v, int x) {
    if (sz[v] > 1) {
        ans -= 1LL * sz[v] * sz[v];
    }
    sz[v] += x;
    if (sz[v] > 1) {
        ans += 1LL * sz[v] * sz[v];
    }
}

ll res[N];

void go(int tl, int tr, ve<array<int, 3>> edges) {
    int tm = (tl + tr) >> 1;
    
    fe (cur, edges) {
        if (cur[0] > tm) continue;
        g[cur[1]].clear();
        g[cur[2]].clear();
        gr[cur[1]].clear();
        gr[cur[2]].clear();

        vis[cur[1]] = 0;
        vis[cur[2]] = 0;
    }

    fe (cur, edges) {
        if (cur[0] > tm) continue;

        g[cur[1]].pb(cur[2]);
        gr[cur[2]].pb(cur[1]);
    }

    order.clear();
    fe (cur, edges) {
        if (cur[0] > tm) continue;

        if (!vis[cur[1]]) {
            tsort(cur[1]);
        }
        if (!vis[cur[2]]) {
            tsort(cur[2]);
        }
    }
    reverse(all(order));

    fe (cur, edges) {
        if (cur[0] > tm) continue;
        vis[cur[1]] = 0;
        vis[cur[2]] = 0;
    }

    ve<array<int, 4>> changes;

    fe (i, order) {
        if (!vis[i]) {
            st.clear();
            dfs(i);

            fe (x, st) {
                assert(x == who[x]);

                if (who[x] != st[0]) {
                    int old = sz[x];
                    upd(st[0], sz[x]);
                    upd(who[x], -sz[x]);
                    changes.pb({x, who[x], st[0], old});
                    who[x] = st[0];
                }
            }
        }
    }
    
    res[tm] = ans;
    ve<array<int, 3>> edgesL;
    ve<array<int, 3>> edgesR;

    fe (cur, edges) {
        if (who[cur[1]] == who[cur[2]]) {
            edgesL.pb(cur);
        } else {
            edgesR.pb({cur[0], who[cur[1]], who[cur[2]]});
        }
    }

    rf (i, sz(changes) - 1, 0) {
        auto& cur = changes[i];

        who[cur[0]] = cur[1];
        assert(cur[1] != cur[2]);

        upd(cur[2], -cur[3]);
        upd(cur[1], cur[3]);
    }

    if (tl == tr) {

    } else {
        if (tl <= tm - 1) {
            go(tl, tm - 1, edgesL);
        }

        if (tm + 1 <= tr) {
            fe (cur, changes) {
                assert(cur[1] != cur[2]);
                upd(cur[2], cur[3]);
                upd(cur[1], -cur[3]);
                
                who[cur[0]] = cur[2];
            }

            go(tm + 1, tr, edgesR);

            rf (i, sz(changes) - 1, 0) {
                auto& cur = changes[i];
                
                who[cur[0]] = cur[1];
                assert(cur[1] != cur[2]);
                upd(cur[1], cur[3]);
                upd(cur[2], -cur[3]);
            }
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

    cin >> n >> m >> q;

    fr (i, 1, q) {
        int x, y;
        char ch;
        cin >> x >> y >> ch;
        int c = ch;

        a[i][0] = x;
        a[i][1] = y;
        a[i][2] = c;

        uX.pb(x);
        uY.pb(y);
    }

    sort(all(uX));
    uX.erase(unique(all(uX)), uX.end());
    sort(all(uY));
    uY.erase(unique(all(uY)), uY.end());

    fe (cur, uX) {
        row[cur] = ++nVer;
    }
    fe (cur, uY) {
        col[cur] = ++nVer;
    }

    assert(nVer < N);

    fr (i, 1, q) {
        int x = a[i][0];
        int y = a[i][1];
        int c = a[i][2];

        if (c == 'B') {
            edges.pb({i, row[x], col[y]});
        } else {
            edges.pb({i, col[y], row[x]});
        }
    }

    sort(all(edges));

    fr (i, 1, nVer) {
        who[i] = i;
        sz[i] = 1;
    }

    go(1, q, edges);

    fr (i, 1, q) {
        cout << res[i] << "\n";
    }

    return 0;
}