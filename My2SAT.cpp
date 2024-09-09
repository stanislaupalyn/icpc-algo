#include <bits/stdc++.h>
 
#define fr(i, a, b) for(int i = (a); i <= (b); i++)
#define rf(i, a, b) for(int i = (a); i >= (b); i--)
#define fe(x, y) for(auto& x : y)
 
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define mt make_tuple
 
#define pw(x) (1LL << (x))
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()
 
using namespace std;
 
using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template<typename T>
using ve = vector<T>;
 
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
template<typename T>
using oset = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
#define fbo find_by_order
#define ook order_of_key
 
mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());
 
template<typename T>
bool umn(T& a, T b) { return a > b ? a = b, 1 : 0; }
template<typename T>
bool umx(T& a, T b) { return a < b ? a = b, 1 : 0; }
 
struct twoSAT {
    int n;
    ve<tuple<bool, int, bool, int>> a;
    ve<ve<ve<int>>> g;
    twoSAT(int n) {
        this->n = n;
        g.resize(2, ve<ve<int>>(n));
    }
    void either(bool c1, int x1, bool c2, int x2) {
        g[c1][x1].pb(sz(a));
        g[c2][x2].pb(sz(a));
        a.pb(mt(c1, x1, c2, x2));
    }
    ve<bool> solve() {
        ve<int> col(n, -1);
        ve<bool> vis(sz(a));
        ve<int> st1, st2;
        bool bad;
 
        function<void(int)> dfs = [&](int v) {
            vis[v] = 1;
            st1.pb(v);
            bool c1, c2;
            int x1, x2;
            tie(c1, x1, c2, x2) = a[v];
            if(col[x1] == c1 || col[x2] == c2) {
                return;
            }
            if(col[x1] != -1 && col[x2] != -1) {
                bad = 1;
                return;
            }
 
            if(col[x1] == -1) {
                col[x1] = c1;
                st2.pb(x1);
                fe(to, g[!c1][x1]) {
                    if(!vis[to]) {
                        dfs(to);
                    }
                }
            } else {
                col[x2] = c2;
                st2.pb(x2);
                fe(to, g[!c2][x2]) {
                    if(!vis[to]) {
                        dfs(to);
                    }
                }
            }
        };
 
        fr(i, 0, sz(a) - 1) {
            if(!vis[i]) {
                bad = 0;
                st1.clear();
                st2.clear();
                dfs(i);
                if(!bad) continue;
                bad = 0;
                fe(x, st1) vis[x] = 0;
                fe(x, st2) col[x] = -1;
                swap(get<0>(a[i]), get<2>(a[i]));
                swap(get<1>(a[i]), get<3>(a[i]));
                dfs(i);
                if(bad) {
                    return ve<bool>(0);
                }
            }
        }
        ve<bool> ans(n);
        fr(i, 0, n - 1) {
            ans[i] = col[i];
        }
        fr(i, 0, sz(a) - 1) {
            assert(col[get<1>(a[i])] == get<0>(a[i]) || col[get<3>(a[i])] == get<2>(a[i]));
        }
        return ans;
    }
};
 
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
 
    int n, m;
    cin >> n >> m;
 
    twoSAT T(m);
 
    fr(i, 0, n - 1) {
        char c1;
        int x1;
        char c2;
        int x2;
        cin >> c1 >> x1 >> c2 >> x2;
        x1--;
        x2--;
        T.either(c1 == '+', x1, c2 == '+', x2);
    }
 
    ve<bool> ans = T.solve();
    if(!sz(ans)) {
        cout << "IMPOSSIBLE\n";
    } else {
        fr(i, 0, sz(ans) - 1) {
            if(ans[i]) cout << "+";
            else cout << "-";
            cout << " ";
        }
        cout << "\n";
    }
 
    return 0;
}