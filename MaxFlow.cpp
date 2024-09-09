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

struct Dinic {
    struct Edge {
        int a, b;
        ll cap, flow;
    };
        
    int n, S, T;
    ve<ve<int>> g;
    ve<Edge> e;
    ve<int> d, ptr;
    int lim;

    void init(int n) {
        this->n = n;
        g.resize(n);
        d.resize(n);
        ptr.resize(n);
    }

    void addEdge(int a, int b, int c) {
        g[a].pb(sz(e));
        e.pb({a, b, c, 0});
        g[b].pb(sz(e));
        e.pb({b, a, 0, 0});
    }

    bool bfs() {
        fill(all(d), -1);
        queue<int> q;
        d[S] = 0;
        q.push(S);

        while (sz(q)) {
            int v = q.front();
            q.pop();

            fe (id, g[v]) {
                if (d[e[id].b] == -1 && e[id].cap - e[id].flow >= lim) {
                    d[e[id].b] = d[v] + 1;
                    q.push(e[id].b);
                }
            }
        }

        return d[T] != -1;
    }

    ll sendFlow(int v, ll flow) {
        if (v == T) return flow;
        if (!flow) return 0;

        for (; ptr[v] < sz(g[v]); ptr[v]++) {
            int id = g[v][ptr[v]];

            if (d[e[id].b] != d[v] + 1) continue;

            ll pushed = sendFlow(e[id].b, min(flow, e[id].cap - e[id].flow));
            if (pushed) {
                e[id].flow += pushed;
                e[id ^ 1].flow -= pushed;
                return pushed;
            }
        }

        return 0;
    }

    ll calc(int s, int t) {
        S = s;
        T = t;
        ll ans = 0;

        for (lim = pw(29); lim >= 1; lim /= 2) {
            while (bfs()) {
                fill(all(ptr), 0);
                ll pushed;
                do {
                    pushed = sendFlow(S, 1e18);
                    ans += pushed;
                } while (pushed);
            }
        }
        return ans;
    }
};

int n, m;

Dinic D;

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

    cin >> n >> m;

    D.init(n + 1);

    fr (i, 1, m) {
        int a, b, c;
        cin >> a >> b >> c;
        D.addEdge(a, b, c);
    }

    cout << D.calc(1, n) << "\n";

    return 0;
}