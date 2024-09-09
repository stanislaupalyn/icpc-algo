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

const static int mod = 1e9 + 7;

void addSelf(int& a, int b) {
    a += b;
    if(a >= mod) a -= mod;
}

int mult(int a, int b) {
    return 1LL * a * b % mod;
}

struct Matrix {
    const static int matrix_n = 2;
    const static int matrix_m = 2;

    int n, m;
    int a[matrix_n][matrix_m];

    int* operator[](int x) {
        return a[x];
    }

    Matrix(int n, int m) : n(n), m(m) {
        fr (i, 0, matrix_n - 1) {
            fr (j, 0, matrix_m - 1) {
                a[i][j] = 0;
            }
        }
    }
};

Matrix mult(Matrix a, Matrix b) {
    assert(a.m == b.n);

    // a.n x b.m
    Matrix c(a.n, b.m);

    fr (row, 0, c.n - 1) {
        fr (col, 0, b.m - 1) {
            fr (i, 0, a.m - 1) {
                addSelf(c[row][col], mult(a[row][i], b[i][col]));
            }
        }
    }
    return c;
}

Matrix bpow(Matrix a, ll b) {
    // cout << a.n << " " << a.m << "\n";
    assert(a.n == a.m);

    Matrix c(a.n, a.m);
    fr (i, 0, a.n - 1) c[i][i] = 1;

    while (b) {
        if (b & 1) {
            c = mult(c, a);
        }
        a = mult(a, a);
        b >>= 1;
    }
    return c;
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

    ll n;
    cin >> n;

    if (n == 0) {
        cout << "0\n";
        return 0;
    }

    Matrix T(2, 2);
    T[0][1] = 1;
    T[1][0] = 1;
    T[1][1] = 1;

    Matrix A(2, 1);
    A[0][0] = 1;
    A[1][0] = 1;

    if (n > 1) {
        T = bpow(T, n - 1);
        A = mult(T, A);
    }

    cout << A[0][0] << "\n";


    return 0;
}

/*
http://fusharblog.com/solving-linear-recurrence-for-programming-contest/
*/