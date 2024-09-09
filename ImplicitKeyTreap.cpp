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

mt19937_64 rng(228);

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

const int N = 2e5 + 5;

int sz[N];
ll prior[N];
int L[N], R[N], P[N];
bool flip[N];
bool cycle[N];

void upd(int v) {
    sz[v] = sz[L[v]] + sz[R[v]] + 1;
    P[L[v]] = v;
    P[R[v]] = v;
}

void push(int v) {
    if (flip[v]) {
        swap(L[v], R[v]);
        flip[L[v]] ^= 1;
        flip[R[v]] ^= 1;
        flip[v] = 0;
    }
}

int merge(int a, int b) {
    if (!a) return b;
    if (!b) return a;

    push(a);
    push(b);

    if (prior[a] > prior[b]) {
        R[a] = merge(R[a], b);
        upd(a);
        return a;
    } else {
        L[b] = merge(a, L[b]);
        upd(b);
        return b;
    }
}

pii split(int v, int k) {
    if (!v) return {0, 0};
    push(v);

    if (sz[L[v]] + 1 <= k) {
        P[R[v]] = 0;
        pii p = split(R[v], k - sz[L[v]] - 1);
        R[v] = p.fi;
        upd(v);

        assert(P[v] == 0);
        P[p.se] = 0;
        return {v, p.se};
    } else {
        P[L[v]] = 0;
        pii p = split(L[v], k);
        L[v] = p.se;
        upd(v);

        assert(P[v] == 0);
        P[p.fi] = 0;
        return {p.fi, v};
    }
}

int getLeft(int v) {
    int was = v;
    ve<int> st;
    while (1) {
        st.pb(v);
        if (!P[v]) break;
        v = P[v];
    }
    reverse(all(st));
    fe (x, st) {
        push(x);
    }

    v = was;
    int ans = 0;
    int last = -1;
    while (1) {
        if (last == -1) {
            ans += sz[L[v]];
        } else {
            if (last == R[v]) {
                ans += 1 + sz[L[v]];
            }
        }

        last = v;
        if (!P[v]) break;
        v = P[v];
    }
    return ans;
}

int getRoot(int v) {
    while (1) {
        if (!P[v]) break;
        v = P[v];
    }
    return v;
}


int n, m, q;

void addEdge(int a, int b) {
    if (getRoot(a) == getRoot(b)) {
        cycle[getRoot(a)] = 1;
    } else {
        int v1 = getRoot(a);
        int v2 = getRoot(b);

        if (getLeft(a) != sz[v1] - 1) {
            flip[v1] ^= 1;
            assert(getLeft(a) == sz[v1] - 1);
        }

        if (getLeft(b) != 0) {
            flip[v2] ^= 1;
            assert(getLeft(b) == 0);
        }

        merge(v1, v2);
    }
}

void delEdge(int a, int b) {
    assert(getRoot(a) == getRoot(b));

    int v = getRoot(a);
    if (cycle[v]) {
        int p1 = getLeft(a);
        int p2 = getLeft(b);

        if (p1 > p2) {
            swap(p1, p2);
        }

        cycle[v] = 0;

        if (p1 == 0 && p2 == sz[v] - 1) {
            return;
        }
        assert(p1 + 1 == p2);

        auto p = split(v, p1 + 1);
        assert(sz[p.fi] == p1 + 1);
        merge(p.se, p.fi);

    } else {
        int p1 = getLeft(a);
        int p2 = getLeft(b);

        if (p1 > p2) {
            swap(p1, p2);
        }

        assert(p1 + 1 == p2);
        split(v, p1 + 1);
    }
}

int getDist(int a, int b) {
    if (getRoot(a) != getRoot(b)) {
        return -1;
    } else {
        int v = getRoot(a);
        int d1 = getLeft(a);
        int d2 = getLeft(b);

        if (cycle[v]) {
            return min(sz[v] - abs(d1 - d2), abs(d1 - d2)) - 1;
        } else {
            return abs(d1 - d2) - 1;
        }
    }
    assert(0);
}

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

    cin >> n >> m >> q;

    fr (i, 1, n) {
        prior[i] = rng();
        sz[i] = 1;
    }

    fr (i, 1, m) {
        int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }


    fr (i, 1, q) {
        char c;
        cin >> c;

        if (c == '+') {
            int a, b;
            cin >> a >> b;
            addEdge(a, b);
        }

        if (c == '-') {
            int a, b;
            cin >> a >> b;
            delEdge(a, b);
        }

        if (c == '?') {
            int a, b;
            cin >> a >> b;
            if (a == b) {
                cout << "0\n";
            } else {
                cout << getDist(a, b) << "\n";
            }
        }
    }

    return 0;
}