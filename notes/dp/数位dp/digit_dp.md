# 数位DP

## 模板

> **例题：[EDPC S - Digit Sum](https://atcoder.jp/contests/dp/tasks/dp_s)**
>
> 求出 $1\sim K$ 中数位之和是 $D$ 的倍数的数字总数。
>
> $K\le 10^{10000},D\le 100$。

```cpp
#include <bits/stdc++.h>
using namespace std;
constexpr int MOD = (int)1e9 + 7;
inline int add(int a, int b) { return a + b >= MOD ? a + b - MOD : a + b; }
const int N = 10010;
const int M = 105;
int digits, a[N], d;
int dp[N][M];  // dp[i][j]表示 枚举到第i位,之前的状态为j

// pos: 枚举到第i位, last: 前驱状态, limit: 之前是否是边界数位, lz(leading zero): 之前是否是前导零
int dfs(int pos, int last, int limit, int lz) {
    if (pos == digits) {  // 达到搜索边界
        return (!lz) && (last == 0);
    }
    if (!limit && !lz && dp[pos][last] != -1) {  // 之前搜索过的状态
        return dp[pos][last];
    }
    int res = 0;
    if (lz) {  // 之前的数位都是前导零
        res = dfs(pos + 1, 0, 0, 1);
    }
    for (int i = 1 - (lz == 0); i <= (limit ? a[pos] : 9); i++) {  // 这里搜索所有非前导零的后继状态
        int nxt_limit = (limit ? a[pos] == i : 0);  // 下一个limit状态
        res = add(res, dfs(pos + 1, (last + i) % d, nxt_limit, 0));
    }
    if (!limit && !lz) {  // 只要不是边界状态就记忆化
        dp[pos][last] = res;
    }
    return res;
}

int solve(const string& s) {
    memset(dp, -1, sizeof dp);
    digits = s.length();
    for (int i = 0; i < s.length(); i++) {
        a[i] = s[i] - '0';
    }
    return dfs(0, 0, 1, 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string k;
    cin >> k >> d;
    cout << solve(k);
    return 0; 
}
```



## 练习题

### [P2657 [SCOI2009] windy 数](https://www.luogu.com.cn/problem/P2657)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 11;
int digits, a[N], dp[N][N];

// pos: 枚举到第i位, last: 前驱状态, limit: 之前是否是边界数位, lz(leading zero): 之前是否是前导零
int dfs(int pos, int last, int limit, int lz) {
    if (pos == digits) {  // 达到搜索边界
        return (!lz);
    }
    if (!limit && !lz && dp[pos][last] != -1) {  // 之前搜索过的状态
        return dp[pos][last];
    }
    int res = 0;
    if (lz) {  // 之前的数位都是前导零
        res = dfs(pos + 1, 0, 0, 1);
    }
    for (int i = 1 - (lz == 0); i <= (limit ? a[pos] : 9); i++) {  // 这里搜索所有非前导零的后继状态
        if (!lz && abs(i - last) < 2) continue;  // 相邻两个数字之差至少为 2
        int nxt_limit = (limit ? a[pos] == i : 0);  // 下一个limit状态
        res += dfs(pos + 1, i, nxt_limit, 0);
    }
    if (!limit && !lz) {  // 只要不是边界状态就记忆化
        dp[pos][last] = res;
    }
    return res;
}

int solve(int x) {
    string s = to_string(x);
    digits = 0;
    memset(dp, -1, sizeof dp);
    for (int i = 0; i < s.length(); i++) {
        a[i] = s[i] - '0';
        digits++;
    }
    return dfs(0, 0, 1, 1);
}

int main() {
    int a, b;
    cin >> a >> b;
    cout << solve(b) - solve(a - 1);
    return 0;
}
```

### [P2602 [ZJOI2010] 数字计数](https://www.luogu.com.cn/problem/P2602)

对每个数位分别讨论（代码中的target），然后记录状态时需要多设置一维用于记录target的出现次数。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 15;
int target;
ll digits, a[N], dp[N][N][N];

// pos: 枚举到第i位, last: 前驱状态, limit: 之前是否是边界数位, lz(leading zero): 之前是否是前导零, cnt: 记录target在当前的出现总次数
ll dfs(int pos, int last, int limit, int lz, ll cnt) {
    if (pos == digits) {  // 达到搜索边界
        return lz ? 0LL : cnt;
    }
    if (!limit && !lz && dp[pos][last][cnt] != -1) {  // 之前搜索过的状态
        return dp[pos][last][cnt];
    }
    ll res = 0;
    if (lz) {  // 之前的数位都是前导零
        res = dfs(pos + 1, 0, 0, 1, 0);
    }
    for (int i = 1 - (lz == 0); i <= (limit ? a[pos] : 9); i++) {  // 这里搜索所有非前导零的后继状态
        int nxt_limit = (limit ? a[pos] == i : 0);  // 下一个limit状态
        res += dfs(pos + 1, i, nxt_limit, 0, cnt + (i == target));
    }
    if (!limit && !lz) {  // 只要不是边界状态就记忆化
        dp[pos][last][cnt] = res;
    }
    return res;
}

ll solve(ll x) {
    string s = to_string(x);
    digits = 0;
    memset(dp, -1, sizeof dp);
    for (int i = 0; i < s.length(); i++) {
        a[i] = s[i] - '0';
        digits++;
    }
    return dfs(0, 0, 1, 1, 0);
}

int main() {
    ll a, b;
    cin >> a >> b;
    a--;
    vector<ll> res(10);
    for (int i = 0; i < 10; i++) {
        target = i;
        res[i] = solve(b) - solve(a);
        cout << res[i] << " \n"[i == 9];
    }
    return 0;
}
```

### [P4127 [AHOI2009] 同类分布](https://www.luogu.com.cn/problem/P4127)

注意到本题中数位长度 $\le 18$，所以数位之和不超过 $9\times 18=162$。因此可以枚举数位之和 $m$，统计 $m$ 的倍数的数量。复杂度上限是 $18\times 162^3$。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 20;
const int M = 9 * 18;
int md;
ll digits, a[N], dp[N][M][M];

// pos: 枚举到第i位, last_*: 前驱状态, limit: 之前是否是边界数位, lz(leading zero): 之前是否是前导零
ll dfs(int pos, int last_sum_total, int last_sum_digits,int limit, int lz) {
    if (pos == digits) {  // 达到搜索边界
        return (!lz) && (last_sum_digits == md) && (last_sum_total == 0);
    }
    if (!limit && !lz && dp[pos][last_sum_total][last_sum_digits] != -1) {  // 之前搜索过的状态
        return dp[pos][last_sum_total][last_sum_digits];
    }
    ll res = 0;
    if (lz) {  // 之前的数位都是前导零
        res = dfs(pos + 1, 0, 0, 0, 1);
    }
    for (int i = 1 - (lz == 0); i <= (limit ? a[pos] : 9); i++) {  // 这里搜索所有非前导零的后继状态
        int nxt_limit = (limit ? a[pos] == i : 0);  // 下一个limit状态
        if (last_sum_digits + i > md) continue;  // 当前枚举的模数(数位之和)为md
        res += dfs(pos + 1, (last_sum_total * 10 + i) % md, last_sum_digits + i, nxt_limit, 0);
    }
    if (!limit && !lz) {  // 只要不是边界状态就记忆化
        dp[pos][last_sum_total][last_sum_digits] = res;
    }
    return res;
}

ll solve(ll x) {
    string s = to_string(x);
    digits = 0;
    memset(dp, -1, sizeof dp);
    for (int i = 0; i < s.length(); i++) {
        a[i] = s[i] - '0';
        digits++;
    }
    return dfs(0, 0, 0, 1, 1);
}

int main() {
    ll a, b, res = 0;
    cin >> a >> b;
    a--;
    for (int i = 1; i <= M; i++) {
        md = i;
        res += solve(b) - solve(a);
    }
    cout << res;
    return 0;
}
```

### [MYQ10 - Mirror Number](https://www.spoj.com/problems/MYQ10/)

> $T$ 组询问，每次询问 $[a,b]$ 中只含有 $0,1,8$，并且是回文数的数字数量。
>
> $T\le 1e5;0\le a\le b\le 1e44$。

不考虑多组，只考虑单组询问怎么做：首先，本题询问回文数的数量，因此我们可以只枚举一半的数位，当我们枚举到中心点时，后一半数位就已经确定了，只需要检查这个数字是否合法（$\le limit$）即可。

更具体地，假设我们要求 $f(x)$，$f(x)$ 表示 $\le x$ 的mirror number数量，实际上我们只需要对长度等于 $x$ 的位数的mirror number进行dp，因为只有这些数字存在边界问题。而长度 $\lt x$ 的位数时，我们枚举mirror number的长度 $l$，mirror number的数量一定是 $2\times 3^{\lfloor (l-1)/2\rfloor}$。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 50;
const int M = 9 * 18;
int md;
int p[] = { 0,1,-1,-1,-1,-1,-1,-1,2 };
ll digits, a[N], dp[N / 2][3], dp2[N / 2];
string border;

// pos: 枚举到第i位, last: 前驱状态, limit: 之前是否是边界数位, lz(leading zero): 之前是否是前导零
ll dfs(int pos, int last, int limit, int lz) {
    if (pos == (digits + 1) / 2) {  // 达到搜索边界
        if (!lz) {
            if (limit) {
                string tmp = border;
                for (int i = 0; i < (int)tmp.length() / 2; i++) {
                    tmp[(int)tmp.length() - i - 1] = tmp[i];
                }
                return tmp <= border;
            } else {
                return 1;
            }
        }
        return 0;
    }
    if (!limit && !lz && dp[pos][p[last]] != -1) {  // 之前搜索过的状态
        return dp[pos][p[last]];
    }
    ll res = 0;
    // 不能有前导零
    for (int i : {0, 1, 8}) {  // 这里搜索所有非前导零的后继状态
        if (lz && !i) continue;
        if (limit && i > border[pos] - '0') continue;
        int nxt_limit = (limit ? a[pos] == i : 0);  // 下一个limit状态
        res += dfs(pos + 1, i, nxt_limit, 0);
    }
    if (!limit && !lz) {  // 只要不是边界状态就记忆化
        dp[pos][p[last]] = res;
    }
    return res;
}

ll solve(string& s) {
    digits = 0;
    memset(dp, -1, sizeof dp);
    border = s;
    for (int i = 0; i < s.length(); i++) {
        a[i] = s[i] - '0';
        digits++;
    }
    ll res = 0;
    dp2[1] = 2;
    for (int i = 1; i <= digits - 1; i++) {
        int len = (i + 1) / 2;
        if (!dp2[len]) dp2[len] = dp2[len - 1] * 3LL;
        res += dp2[len];
    }
    return dfs(0, 0, 1, 1) + res;
}

int check(string& a) {  // check a 是否是mirror number
    auto b = a;
    reverse(b.begin(), b.end());
    if (a == b) {
        for (auto c : a) {
            if (!(c == '0' or c == '1' or c == '8')) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        string a, b;
        cin >> a >> b;
        cout << solve(b) - solve(a) + check(a) << "\n";
    }
    return 0;
}
```

