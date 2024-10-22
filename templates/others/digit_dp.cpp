// https://atcoder.jp/contests/dp/submissions/59070069
// 求 [1, k] 中数位之和是 d 的倍数的数字总数
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