# 线性递推第N项

## 问题引入

> 已知线性递推式 $A_N=c_1A_{N-1}+c_2A_{N-2}+\cdots+c_KA_{N-K}=\sum_{i=1}^K c_iA_{N-i}$，和该递推关系的初值，求 $A_N$。

比如“斐波那契数列 $F_{i}=F_{i-1}+F_{i-2},F_0=F_1=1$，求 $F_N$”就是一个经典例子。

显然，暴力递推的复杂度是 $O(NK)$，一旦 $N$ 是一个很大的值（比如 $N=10^{18}$）就不行了。本文内容就是该问题的解决方案。

## 矩阵快速幂

对于该问题，矩阵快速幂是一个显然的解法，时间复杂度 $O(K^3\log N)$，比如求斐波那契数列第 $10^{18}$ 项就可以用该方法解决。

做法是将递推关系表示为矩阵乘法，这里以斐波那契数列为例
$$
\pmatrix{F_{i+2}\\F_{i+1}} = \pmatrix{1&1\\1&0}\pmatrix{F_{i+1}\\F_{i}} = \pmatrix{1&1\\1&0}^2\pmatrix{F_{i}\\F_{i-1}} = \cdots = \pmatrix{1&1\\1&0}^{i+1}\pmatrix{F_1\\F_0}
$$
于是我们只需要求出转移矩阵 $\pmatrix{1&1\\1&0}$ 的 $O(N)$ 次幂即可求出斐波那契数列的第 $N$ 项，用快速幂即可。

## Kitamasa方法

### $O(K^2\log N)$

显然矩阵快速幂的一个瓶颈就是矩阵乘法的 $O(K^3)$。对于斐波那契数列这个复杂度很小，但是如果 $K=1000$，那么矩阵快速幂方法就不行了。

Kitamasa方法就是一种在 $O(K^2\log N)$ 的复杂度下求解线性递推第N项的方法。让我们转变递推式为
$$
A_N = \sum_{i=1}^K c_iA_{N-i} = \sum_{j=0}^{K-1}d_j A_{j}
$$
也就是将 $A_N$ 用递推关系的初值表示出来。

举一个例子：“已知递推关系 $A_N=2A_{N-1}+A_{N-2}$（也就是 $c_1=2,c_2=1$），将 $A_5$ 用初值 $A_0,A_1$ 表示”
$$
\begin{aligned}
A_5 &= 2A_4 + A_3\\
A_5 &= 2(2A_3 + A_2) + A_3 = 5A_3 + 2A_2\\
A_5 &= 5(2A_2 + A_1) + 2A_2 = 12A_2 + 5A_1\\
A_5 &= 12(2A_1 + A_0) + 5A_1 = 29A_1 + 12A_0\\
\end{aligned}
$$
于是 $d_0=29,d_2=12$。

我们从另一个角度来考虑这个流程，对于式子 $A_5 = 2A_4 + A_3$，我们考虑先消掉除了 $A_5$ 的最高次项（即次高次项），也就是先消掉 $A_4$：

- 两侧同减 $2(A_4-2A_3-A_2)=0$，得到 $A_5 = 5A_3 + 2A_2$。
- 两侧同减 $5(A_3-2A_2-A_1)=0$，得到 $A_5 = 12A_2 + 5A_1$。
- 两侧同减 $12(A_2-2A_1-A_0)=0$，得到 $A_5=29A_1 + 12A_0$。

也就是说，对于等式 $A_N = \sum_{i=1}^K c_iA_{N-i}$，每次两侧同减 $c(A_x-\sum_{i=1}^K c_i A_{N-i})=0$（$c$ 是当前次高次项的系数），消掉当前的次高次项，直到只剩初值。

不难发现，这一流程实际上等价于求解 $x^n \pmod{f(x)}$，$f(x)=x^K-c_1x^{K-1}-c_2x^{K-2}-\cdots-c_Kx^0$（$f(x)$ 实际上就是递推关系的**特征多项式**）。对于上方样例就是
$$
x^5 = (x^2-2x-1)(x^3+2x^2+5x+12) + (29x + 12)
$$
也就是 $x^5 \equiv (29x+12)\pmod{(x^2-2x-1)}$

然后考虑如何求出 $x^N \pmod{f(x)}$，这里我们可以考虑类似于快速幂的做法，通过求出 $x^1 \pmod{f(x)},x^2 \pmod{f(x)},x^4 \pmod{f(x)},x^8 \pmod{f(x)},\ldots$ 然后通过 $N$ 的二进制分解，乘出 $x^N \pmod{f(x)}$。

写成c++代码就是

```cpp
// A[n] = c1*A[n-1] + c2*A[n-2] + ... + ck*A[n-k], given { A[0], ..., A[k-1] }, compute A[n].
int kitasama(vector<int>& c, vector<int>& a, long long n) {
    int k = c.size();
    vector<int> d = { 1 };     // result
    vector<int> xn = { 0,1 };  // xn = x^1(initial state), x^2, x^4, ...
    vector<int> f(k + 1);      // f(x) = x^k - \sum(c[i] * x^i)
    f.back() = 1;
    for (int i = 0; i < k; i++) {
        f[i] = -c[i];
    }
    while (n) {
        if (n & 1) {
            d = poly_mod(poly_mul(d, xn), f);
        }
        n >>= 1;
        xn = poly_mod(poly_mul(xn, xn), f);
    }
    int res = 0;
    for (int i = 0; i < a.size(); i++) {
        res += a[i] * d[i];
    }
    return res;
}
```

上方代码中 `poly_mod(x, y)` 表示多项式 $x$ 对多项式 $y$ 取模；`poly_mul(x, y)` 表示多项式 $x$ 乘多项式 $y$。

由于特征多项式的最高幂次为 $k$，所以这里的多项式乘法和多项式取模的复杂度都是 $O(k^2)$，完整代码如下

```cpp
constexpr int MOD = (int)1000000007;
inline int normal(long long a) { return (a % MOD + MOD) % MOD; }
inline int add(int a, int b) { return a + b >= MOD ? a + b - MOD : a + b; }
inline int sub(int a, int b) { return a - b < 0 ? a - b + MOD : a - b; }
inline int mul(int a, int b) { return (int)((long long)a * b % MOD); }

vector<int> poly_mul(const vector<int>& a, const vector<int>& b) {
    vector<int> res(a.size() + b.size() - 1);
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            res[i + j] = add(res[i + j], mul(a[i], b[j]));
        }
    }
    return res;
}

vector<int> poly_mod(const vector<int>& a, const vector<int>& b) {
    vector<int> res = a;
    int n = a.size(), m = b.size();
    for (int i = n - 1; i >= m - 1; i--) {
        for (int j = 0; j < m; j++) {
            res[i + j - m + 1] = sub(res[i + j - m + 1], mul(res[i], b[j]));
        }
    }
    res.resize(m - 1);
    return res;
}

// A[n] = c1*A[n-1] + c2*A[n-2] + ... + ck*A[n-k], given { A[0], ..., A[k-1] }, compute A[n].
int kitasama(vector<int>& c, vector<int>& a, long long n) {
    if (n < a.size()) {  // special case
        return a[n];
    }
    int k = c.size();
    vector<int> d = { 1 };     // result
    vector<int> xn = { 0,1 };  // xn = x^1, x^2, x^4, ...
    vector<int> f(k + 1);      // f(x) = x^k - \sum(c[i] * x^i)
    f.back() = 1;
    for (int i = 0; i < k; i++) {
        f[i] = normal(-c[i]);
    }
    while (n) {
        if (n & 1) {
            d = poly_mod(poly_mul(d, xn), f);
        }
        n >>= 1;
        xn = poly_mod(poly_mul(xn, xn), f);
    }
    int res = 0;
    for (int i = 0; i < a.size(); i++) {
        res = add(res, mul(a[i], d[i]));
    }
    return res;
}
```



### $O(K\log K\log N)$

注意到该方法的瓶颈就在于多项式乘法和多项式取模的 $O(K^2)$，其中多项式乘法显然可以利用FFT优化到 $O(K\log K)$，多项式取模也可以优化到 $O(K\log K)$。

## 例题

### [**EDPC T - フィボナッチ**](https://atcoder.jp/contests/tdpc/tasks/tdpc_fibonacci)

> 给定
>
> - $a_1=a_2=\cdots= a_K=1$
> - $a_i=a_{i-1}+\cdots+a_{i-K}(i\gt K)$
>
> 求 $a_N \pmod{1e9+7}$。$2\le k\le 1000,1\le N\le 10^9$。

直接套模板即可。注意本题要输出换行符。

### [**Restricted Digits**](https://atcoder.jp/contests/typical90/tasks/typical90_e)

> 给定个位数字 $c_1,\ldots,c_K$，询问：由这些数字构成且恰好是 $B$ 的倍数的 $N$ 位数字有多少？
>
> $1\le c_1\lt\cdots\lt c_K\le 9$；
>
> - 子问题1：$B\le 30,N\le 10000$
> - 子问题2：$B\le 30,N\le 10^8$
> - 子问题3：$B\le 10^3$；$N\le 10^{18}$。

先考虑最朴素的dp：设 $dp[i][j]$ 表示枚举到第 $i$ 位，先前数字模 $B$ 余 $j$ 的方案数，则
$$
dp[i][(j+c)\% B] \text{+=} dp[i-1][j]
$$
这里 $c$ 是枚举数字 $c_1,\ldots,c_K$。显然，这个dp的复杂度是 $O(NBK)$，足以通过子问题1。

不难发现对于每一个 $dp[i][j]$ 的转移，实际上都是由 $dp[i-1][*]$ 线性递推而来，形式化地，
$$
dp[i][j] = \sum_{i=1}^K dp[i-1][(j - c_i)\% B]
$$
也就是说要求出 $dp[N][0]$，我们实际上需要计算 $B$ 个线性递推 。注意到这里的 $B$ 个线性递推可以用矩阵乘法来表示。举个例子，$c_1=1,c_2=2; B=4$，初值为 $dp[0][*] = \pmatrix{1&0&0&0}^\text{T}$
$$
\pmatrix{dp[N][0]\\dp[N][1]\\dp[N][2]\\dp[N][3]} = \pmatrix{0&0&1&1\\ 1&0&0&1\\ 1&1&0&0\\ 0&1&1&0}^1\pmatrix{dp[N-1][0]\\dp[N-1][1]\\dp[N-1][2]\\dp[N-1][3]} = \pmatrix{0&0&1&1\\ 1&0&0&1\\ 1&1&0&0\\ 0&1&1&0}^N dp[0][*]=\pmatrix{0&0&1&1\\ 1&0&0&1\\ 1&1&0&0\\ 0&1&1&0}^N\pmatrix{1\\0\\0\\0}
$$
因此可以用矩阵快速幂优化这个dp到 $O(B^3\log N)$，足以通过子问题2。

再考虑怎么继续优化，对于 $dp[i+j][*]$，能否由 $dp[i][*]$ 和 $dp[j][*]$ 求出？答案是可以的，我们可以这么考虑：$dp[i+j][*]$ 表示所有枚举到 $i+j$ 位的状态，而一个 $i+j$ 位的数字可以拆分为高 $i$ 位和低 $j$ 位，所以就有以下转移：
$$
dp[i+j][(p\times 10^{j} + q)\% B] \text{+=} dp[i][p] \times dp[j][q]
$$
由于我们要求的是 $dp[N][0]$，所以我们可以倍增求出 $dp[1][*],dp[2][*],dp[4][*],dp[8][*],\cdots$，然后合并出 $dp[N][*]$。让我们分析该算法的复杂度：

- 已知 $dp[i][*],dp[j][*]$，求 $dp[i+j][*]$：根据转移方程 $dp[i+j][(p\times 10^{j} + q)\% B] \text{+=} dp[i][p] \times dp[j][q]$ 可知，这个子问题的复杂度是 $O(B^2)$。
- 再考虑倍增的复杂度，该算法的时间复杂度为 $O(B^2\log N)$。足以通过子问题3。         
