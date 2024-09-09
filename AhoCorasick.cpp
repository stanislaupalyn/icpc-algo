#include <bits/stdc++.h>

#pragma GCC optimize("O3")

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

const int N = 1e6 + 5;

int n;
string s;
string a[N];

int szTrie = 1;
int to[N][26];
int suf[N], ssuf[N];
int par[N];
// bool mark[N];
ve<int> in[N];
int ch[N];

void add(const string& s, int idx) {
    int v = 1;
    fr (i, 0, sz(s) - 1) {
        if (!to[v][s[i] - 'a']) {
            to[v][s[i] - 'a'] = ++szTrie;
            par[szTrie] = v;
            ch[szTrie] = s[i] - 'a';
        }
        v = to[v][s[i] - 'a'];
    }
    in[v].pb(idx);
    // mark[v] = 1;
}

ve<int> ans[N];

int main() {
#ifndef LOCAL
    freopen("inputik.txt", "r", stdin);
    freopen("outputik.txt", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
#else
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#endif

    cin >> s;
    cin >> n;

    fr (i, 1, n) {
        cin >> a[i];

        add(a[i], i);
    }

    {
        queue<int> q;
        q.push(1);

        while (sz(q)) {
            int v = q.front();
            q.pop();
            fr (i, 0, 25) {
                if (to[v][i]) {
                    q.push(to[v][i]);
                }
            }

            if (v != 1) {
                int c = suf[par[v]];
                while (c && !to[c][ch[v]]) {
                    c = suf[c];
                }
                suf[v] = c == 0 ? 1 : to[c][ch[v]];
                ssuf[v] = sz(in[suf[v]]) ? suf[v] : ssuf[suf[v]];
            }
        }
    }


    int v = 1;
    fr (i, 0, sz(s) - 1) {
        while (v && !to[v][s[i] - 'a']) {
            v = suf[v];
        }
        v = v == 0 ? 1 : to[v][s[i] - 'a'];

        int c = v;
        while (c) {
            fe (x, in[c]) {
                ans[x].pb(i - sz(a[x]) + 2);
            }
            c = ssuf[c];
        }
    }

    fr (i, 1, n) {
        cout << sz(ans[i]) << " ";
        fe (x, ans[i]) cout << x << " ";
        cout << "\n";
    }

    return 0;
}