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

### [HDU 2089 不要 62](https://acm.hdu.edu.cn/showproblem.php?pid=2089)

> 统计 $[n,m]$ 中不包含62和4的数字个数。举个例子，61152不包含62，62315包含62。
>
> $1e9\gt n,m\gt 0$。尽管本题的数据并没有这么大，但是实际上 $n,m$ 可以是很大的数字。

首先可以用经典的计数技巧将该问题转换为：设 $f(x)$ 表示 $\le x$ 的数字中有几个数字中不包含62，那么本题的答案就是 $f(m)-f(n-1)$。

然后直接看代码：

```cpp
int a[20], digits;
int dp[10][11][2];

int dfs(int pos, int last, int limit) {
    int res = 0;
    if (pos == digits) return 1;
    if (dp[pos][last][limit] != -1) return dp[pos][last][limit];
    for (int i = 0; i <= (limit ? a[pos] : 9); i++) {
        if ((last == 6 && i == 2) || (i == 4)) continue;
        res += dfs(pos + 1, i, (limit ? (i == a[pos]) : 0));
    }
    return dp[pos][last][limit] = res;
}

int solve(ll x) {  // 计算f(x)
    digits = 0;
    memset(dp, -1, sizeof dp);
    while (x) {  // 将大数x的每一位存入数组a
        a[digits++] = x % 10;
        x /= 10;
    }
    reverse(a, a + digits);
    return dfs(0, 10, 1);
}
```

这里 $dp[pos][last][limit]$ 表示当前枚举到第 $pos$ 位，上一位的值是 $last$，上一位是否达到上限（边界，即 $limit$ 标志位）；该状态**所有后继状态**的合法数字个数。此外，注意我们是从高位往低位枚举。

在数位dp中，一个比较特殊的点是我们需要记录一个边界的标志位，这是因为如果上一位达到了边界，那么当前这一位就只能枚举到当前位的边界（假设 $x=64$，上一位是 $6$，到达了边界；那么当前就只能枚举到 $6$）；如果上一位没有达到边界，那么当前这一位就可以任意枚举（假设 $x=64$，上一位是 $5$，没有到达边界；那么当前这一位就可以从 $0$ 枚举到 $9$）。此外，在本题中合法的数字不能包含62，也就是 $last=6$ 时，当前这一位不能枚举 $2$。



### [P2602 [ZJOI2010] 数字计数](https://www.luogu.com.cn/problem/P2602)

> 给定两个正整数 $a$ 和 $b$，求在 $[a,b]$ 中的所有整数中，每个数码(digit)各出现了多少次。
>
> $a\le b\le1e12$。

同理，设 $f_k(x)$ 表示 $\le x$ 的数字中数码 $k$ 出现了多少次。