// https://atcoder.jp/contests/dp/submissions/59070069
// �� [1, k] ����λ֮���� d �ı�������������
#include <bits/stdc++.h>
using namespace std;
constexpr int MOD = (int)1e9 + 7;
inline int add(int a, int b) { return a + b >= MOD ? a + b - MOD : a + b; }
const int N = 10010;
const int M = 105;
int digits, a[N], d;
int dp[N][M];  // dp[i][j]��ʾ ö�ٵ���iλ,֮ǰ��״̬Ϊj

// pos: ö�ٵ���iλ, last: ǰ��״̬, limit: ֮ǰ�Ƿ��Ǳ߽���λ, lz(leading zero): ֮ǰ�Ƿ���ǰ����
int dfs(int pos, int last, int limit, int lz) {
    if (pos == digits) {  // �ﵽ�����߽�
        return (!lz) && (last == 0);
    }
    if (!limit && !lz && dp[pos][last] != -1) {  // ֮ǰ��������״̬
        return dp[pos][last];
    }
    int res = 0;
    if (lz) {  // ֮ǰ����λ����ǰ����
        res = dfs(pos + 1, 0, 0, 1);
    }
    for (int i = 1 - (lz == 0); i <= (limit ? a[pos] : 9); i++) {  // �����������з�ǰ����ĺ��״̬
        int nxt_limit = (limit ? a[pos] == i : 0);  // ��һ��limit״̬
        res = add(res, dfs(pos + 1, (last + i) % d, nxt_limit, 0));
    }
    if (!limit && !lz) {  // ֻҪ���Ǳ߽�״̬�ͼ��仯
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