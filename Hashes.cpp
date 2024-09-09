#include <bits/stdc++.h>

using namespace std;

int bpow(int a, int b, int mod) {
    int res = 1;
    while(b) {
        if(b & 1) {
            res = 1LL * res * a % mod;
        }
        a = 1LL * a * a % mod;
        b >>= 1;
    }
    return res;
}

const int HASH_LEN = 3e5 + 5;
const int MOD[2] = {int(1e9) + 7, int(1e9) + 9};
const int BASE[2] = {227, 229};

int pwr[HASH_LEN][2], ipwr[HASH_LEN][2];

void prepareHashes() {
    for (int i = 0; i < 2; i++) {
        pwr[0][i] = 1;
        for (int j = 1; j < HASH_LEN; j++) {
            pwr[j][i] = 1LL * pwr[j - 1][i] * BASE[i] % MOD[i];
        }
        ipwr[0][i] = 1;
        int inv = bpow(BASE[i], MOD[i] - 2, MOD[i]);
        for (int j = 1; j < HASH_LEN; j++) {
            ipwr[j][i] = 1LL * ipwr[j - 1][i] * inv % MOD[i];
        }
    }
}

struct Hash {
    int a[2] = {0, 0};

    Hash(char c) {
        a[0] = c - 'a' + 1;
        a[1] = c - 'a' + 1;
    }
    Hash() {
        
    }

    Hash operator+(const Hash &rhs) const {
        Hash result;
        for (int i = 0; i < 2; i++) {
            result.a[i] = a[i] + rhs.a[i];
            if (result.a[i] >= MOD[i]) {
                result.a[i] -= MOD[i];
            }
        }
        return result;
    }
    Hash operator-(const Hash &rhs) const {
        Hash result;
        for (int i = 0; i < 2; i++) {
            result.a[i] = a[i] - rhs.a[i];
            if (result.a[i] < 0) {
                result.a[i] += MOD[i];
            }
        }
        return result;
    }
    Hash operator<<(int x) const {
        Hash result;
        for (int i = 0; i < 2; i++) {
            result.a[i] = 1LL * a[i] * pwr[x][i] % MOD[i];
        }
        return result;
    }
    Hash operator>>(int x) const {
        Hash result;
        for (int i = 0; i < 2; i++) {
            result.a[i] = 1LL * a[i] * ipwr[x][i] % MOD[i];
        }
        return result;
    }
    bool operator==(const Hash &rhs) const {
        return a[0] == rhs.a[0] && a[1] == rhs.a[1];
    }
    long long toInt64() {
        return 1LL * a[0] * 2e9 + a[1];
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    prepareHashes();

    string s;
    cin >> s;

    vector<Hash> pref;
    for (int i = 0; i < s.size(); i++) {
        pref.push_back(Hash(s[i]) << i);
        if (i > 0) {
            pref[i] = pref[i - 1] + pref[i];
        }
    }

    auto getHash = [&](int l, int r) {
        return (pref[r] - (l > 0 ? pref[l - 1] : Hash())) >> l;
    };

    vector<int> p;
    int x = s.size();
    for (int j = 2; j * j <= x; j++) {
        if (x % j == 0) {
            p.push_back(j);
            while (x % j == 0) {
                x /= j;
            }
        }
    }
    if (x > 1) {
        p.push_back(x);
    }

    int len = s.size();

    for (auto cur : p) {
        while (len % cur == 0) {
            if (getHash(0, (int)s.size() - 1 - len / cur) == getHash(len / cur, (int)s.size() - 1)) {
                len /= cur;
            } else {
                break;
            }
        }
    }

    cout << "Minimal period of the string is " << len << endl;

    return 0;
}