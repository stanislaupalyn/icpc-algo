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

struct Dinic {
    static const int N = 1005;

    struct Edge {
        int from, to;
        ll flow, cap;
    };
    
    int s, t;
    ve<Edge> e;
    ve<int> g[N];
    int d[N];
    int ptr[N];

    void addEdge(int a, int b, ll c) {
        g[a].pb(sz(e));
        e.pb({a, b, 0, c});
        g[b].pb(sz(e));
        e.pb({b, a, 0, 0});
    }

    bool bfs() {
        queue<int> q;
        fill(d, d + N, -1);
        d[s] = 0;
        q.push(s);

        while (sz(q)) {
            int v = q.front();
            q.pop();

            fe (id, g[v]) {
                int to = e[id].to;

                if (e[id].flow < e[id].cap && d[to] == -1) {
                    d[to] = d[v] + 1;
                    q.push(to);
                }
            }
        }
        return d[t] != -1;
    }

    ll sendFlow(int v, ll flow) {
        if (v == t || !flow) {
            return flow;
        }

        for (; ptr[v] < sz(g[v]); ptr[v]++) {
            int id = g[v][ptr[v]];

            if (d[e[id].to] != d[v] + 1) {
                continue;
            }

            ll pushed = sendFlow(e[id].to, min(flow, e[id].cap - e[id].flow));
            if (pushed) {
                e[id].flow += pushed;
                e[id ^ 1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }

    ll maxFlow(int s, int t) {
        this->s = s;
        this->t = t;
        fe (cur, e) {
            cur.flow = 0;
        }

        ll flow = 0;
        while (bfs()) {
            fill(ptr, ptr + N, 0);

            ll pushed = sendFlow(s, 1e18);
            while (pushed) {
                flow += pushed;
                pushed = sendFlow(s, 1e18);
            }
        }
        return flow;
    }
};

int n, m;
Dinic D;

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif

    cin >> n >> m;
    fr (i, 1, m) {
        int a, b, c;
        cin >> a >> b >> c;

        D.addEdge(a, b, c);
        D.addEdge(b, a, c);
    }

    cout << D.maxFlow(1, n) << "\n";

    for (int i = 0; i < sz(D.e); i += 4) {
        cout << D.e[i].flow - D.e[i + 2].flow << "\n";
    }

    return 0;
}