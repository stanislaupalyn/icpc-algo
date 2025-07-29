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

mt19937_64 rng(232);

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

struct MCF {
    static const int N = 205;

    struct Edge {
        int from, to;
        int flow, cap;
        int cost;
    };

    int s, t;
    ve<int> g[N];
    ve<Edge> e;
    ve<int> oldCost;

    void addEdge(int from, int to, int cap, int cost) {
        g[from].pb(sz(e));
        e.pb({from, to, 0, cap, cost});
        oldCost.pb(cost);
        g[to].pb(sz(e));
        e.pb({to, from, 0, 0, -cost});
        oldCost.pb(-cost);
    }

    ll phi[N];
    ll dist[N];
    bool inQueue[N];
    int p[N];

    pll maxFlow(int s, int t) {
        fr (i, 0, sz(e) - 1) {
            e[i].flow = 0;
            oldCost[i] = e[i].cost;
        }

        this->s = s;
        this->t = t;

        ll pushed = 0;
        // SPFA
        {
            queue<int> q;
            fill(inQueue, inQueue + N, 0);
            fill(dist, dist + N, 1e18);
            dist[s] = 0;
            inQueue[s] = 1;
            q.push(s);

            while (sz(q)) {
                int v = q.front();
                q.pop();
                inQueue[v] = 0;

                fe (id, g[v]) {
                    int to = e[id].to;
                    if (e[id].flow == e[id].cap) continue;

                    if (dist[to] > dist[v] + e[id].cost) {
                        dist[to] = dist[v] + e[id].cost;
                        p[to] = id;

                        if (!inQueue[to]) {
                            inQueue[to] = 1;
                            q.push(to);
                        }
                    }
                }
            }

            if (dist[t] == 1e18) {
                return {0, 0};
            }

            int mn = 2e9;
            int v = t;
            while (1) {
                assert(e[p[v]].to == v);
                umn(mn, e[p[v]].cap - e[p[v]].flow);
                v = e[p[v]].from;
                if (v == s) {
                    break;
                }
            }

            assert(mn > 0);
            pushed += mn;

            v = t;
            while (1) {
                e[p[v]].flow += mn;
                e[p[v] ^ 1].flow -= mn;

                v = e[p[v]].from;
                if (v == s) {
                    break;
                }
            }

            fr (i, 0, N - 1) {
                phi[i] = dist[i];
            }
            fe (cur, e) {
                cur.cost += phi[cur.from] - phi[cur.to];
            }
        }
    
        while (1) {
            set<pll> q;
            fill(dist, dist + N, 1e18);
            q.insert({0, s});
            dist[s] = 0;
            while (sz(q)) {
                int v = q.begin()->se;
                q.erase(q.begin());

                fe (id, g[v]) {
                    int to = e[id].to;
                    if (e[id].flow == e[id].cap) continue;

                    if (dist[to] > dist[v] + e[id].cost) {
                        q.erase({dist[to], to});
                        dist[to] = dist[v] + e[id].cost;
                        p[to] = id;
                        q.insert({dist[to], to});
                    }
                }
            }
        
            if (dist[t] == 1e18) {
                break;
            }

            int mn = 2e9;
            int v = t;
            while (1) {
                assert(e[p[v]].to == v);
                umn(mn, e[p[v]].cap - e[p[v]].flow);
                v = e[p[v]].from;
                if (v == s) {
                    break;
                }
            }
            assert(mn > 0);
            pushed += mn;

            v = t;
            while (1) {
                e[p[v]].flow += mn;
                e[p[v] ^ 1].flow -= mn;

                v = e[p[v]].from;
                if (v == s) {
                    break;
                }
            }

            fr (i, 0, N - 1) {
                phi[i] = dist[i];
            }

            fe (cur, e) {
                cur.cost += phi[cur.from] - phi[cur.to];
            }
        }

        ll val = 0;
        for (int i = 0; i < sz(e); i += 2) {
            val += 1LL * e[i].flow * oldCost[i];
        }

        return {pushed, val};
    }
};

int getId(char c) {
    if (islower(c)) {
        return c - 'a' + 1;
    } else {
        return 26 + c - 'A' + 1;
    }
}

char getChar(int idx) {
    if (idx <= 26) {
        return 'a' + idx - 1;
    }
    return 'A' + idx - 27;
}

int n, k;
string s, t;
int cnt[53][53], cnt1[53];

MCF T;

int rnd(int l, int r) {
    return rng() % (r - l + 1) + l;
}

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#else
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif

    cin >> n >> k;
    cin >> s >> t;

    // k = 52;
    // fr (i, 1, n) {
    //     s.pb(getChar(rnd(1, k)));
    //     t.pb(getChar(rnd(1, k)));
    // }

    fe (cur, s) {
        assert(getId(cur) <= k);
    }
    fe (cur, t) {
        assert(getId(cur) <= k);
    }

    // cout << s << "\n" << t << "\n";

    fr (i, 0, n - 1) {
        cnt1[getId(s[i])]++;
        cnt[getId(s[i])][getId(t[i])]++;
    }

    fr (i, 1, k) {
        T.addEdge(0, i, 1, 0);
        T.addEdge(52 + i, 105, 1, 0);
    }

    fr (from, 1, k) {
        fr (to, 1, k) {
            T.addEdge(from, 52 + to, 1, cnt1[from] - cnt[from][to]);
        }
    }

    pll got = T.maxFlow(0, 105);

    cout << n - got.se << "\n";

    ve<char> who(k + 1);
    fr (i, 1, k) {
        bool found = 0;
        fe (id, T.g[i]) {
            if (T.e[id].flow > 0) {
                found = 1;
                who[i] = getChar(T.e[id].to - 52);
                assert(T.e[id].to - 52 <= k);

                cout << who[i];

                break;
            }
        }
        assert(found);
    }

    int res = 0;
    fr (i, 0, n - 1) {
        if (who[getId(s[i])] == t[i]) {
            res++;
        }
    }
    assert(res == n - got.se);

    return 0;
}