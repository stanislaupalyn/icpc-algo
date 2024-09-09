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

const int N = 1e6 + 5;

int n;
string s;
int cl[N];
int cnt[N];
int ncl[N];
pii ord[N], tmp[N];
int lcp[N];
int pos[N];

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
    cin >> n;
    cin >> s;

    s += '$';

    ve<pii> order;
    {
        fr (i, 0, sz(s) - 1) {
            order.pb({s[i], i});
        }
        sort(all(order));
        int now = 0;
        fr (i, 0, sz(s) - 1) {
            if (i > 0 && order[i].fi != order[i - 1].fi) {
                now++;
            }
            cl[order[i].se] = now;
        }
    }

    for (int p = 1; pw(p - 1) < sz(s); p++) {

        if (p > 1) {
            fr (i, 0, sz(s) - 1) {
                tmp[i] = {(ord[i].fi - pw(p - 1) + sz(s)) % sz(s), ord[i].fi};
            }
        } else {
            fr (i, 0, sz(s) - 1) {
                tmp[i] = {(order[i].se - pw(p - 1) + sz(s)) % sz(s), order[i].se};
            }
        }

        fill(cnt, cnt + sz(s), 0);
        fr (i, 0, sz(s) - 1) {
            cnt[cl[tmp[i].fi] + 1]++;
        }
        fr (i, 1, sz(s) - 1) {
            cnt[i] += cnt[i - 1];
        }

        fr (i, 0, sz(s) - 1) {
            ord[cnt[cl[tmp[i].fi]]++] = tmp[i];
        }

        int now = 0;
        fr (i, 0, sz(s) - 1) {
            if (i > 0 && (cl[ord[i].fi] != cl[ord[i - 1].fi] || cl[ord[i].se] != cl[ord[i - 1].se])) {
                now++;
            }
            ncl[ord[i].fi] = now;
        }
        fr (i, 0, sz(s) - 1) {
            cl[i] = ncl[i];
        }
    }

    fr (i, 0, sz(s) - 1) {
        pos[ord[i].fi] = i;
    }

    {
        int len = 0;
        fr (i, 0, sz(s) - 1) {
            if (!pos[i]) {
                len = 0;
                continue;
            }
            int j = ord[pos[i] - 1].fi;
            while (i + len < sz(s) && j + len < sz(s) && s[i + len] == s[j + len]) {
                len++;
            }
            lcp[pos[i]] = len;

            len = max(0, len - 1);
        }
    }

    fr (i, 0, sz(s) - 1) {
        cout << s.substr(ord[i].fi) << "\n";
    }

    return 0;
}
