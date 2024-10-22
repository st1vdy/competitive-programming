# [Educational DP Contest](https://atcoder.jp/contests/dp)

## **A - Frog 1**

> 有 $N$ 个石子，第 $i$ 个石子的高度为 $h_i$。有一只青蛙初始在石子 $1$ 上，它将重复以下行动直到抵达石子 $N$：
>
> - 如果青蛙当前在石子 $i$，下一步它可以跳跃至石子 $i+1$ 或 $i+2$，并花费 $|h_i-h_j|$，这里 $j$ 表示青蛙跳到的石子的下标。
>
> 求出青蛙的最小花费。

令 $dp[i]$ 表示跳到石子 $i$ 的最小花费，则
$$
\begin{cases}
dp[i+1] = \min(dp[i+1],dp[i] + abs(h[i+1]-h[i]))\\
dp[i+2] = \min(dp[i+2],dp[i] + abs(h[i+2]-h[i]))\\
\end{cases}
$$



## **B - Frog 2**

> 有 $N$ 个石子，第 $i$ 个石子的高度为 $h_i$。有一只青蛙初始在石子 $1$ 上，它将重复以下行动直到抵达石子 $N$：
>
> - 如果青蛙当前在石子 $i$，下一步它可以跳跃至石子 $i+1,i+2,\cdots,i+K$，并花费 $|h_i-h_j|$，这里 $j$ 表示青蛙跳到的石子的下标。
>
> 求出青蛙的最小花费。

令 $dp[i]$ 表示跳到石子 $i$ 的最小花费，则
$$
\begin{cases}
dp[i+j] = \min(dp[i+j],dp[i] + abs(h[i+j]-h[i]))\\
\end{cases}
$$



## **C - Vacation**

> 一共 $N$ 天，每天可以从三种事件中选择一个，每种分别会获得 $h_{i,0},h_{i,1},h_{i,2}$ 的幸福值。连续两天不能选择同一种事件，求最大幸福值。

$dp[i][3]$ 表示第 $i$ 天选择3种事件中某一种时的最大幸福值，则
$$
dp[i][j] = \max_{k\neq j}(dp[i][j], dp[i-1][k]+h[i][k])
$$



## **D - Knapsack 1**(TODO)

## **E - Knapsack 2**(TODO)

## **F - LCS**(TODO:O(NM/W)做法)

经典问题LCS。

## **G - Longest Path**

> 给定一个 $N$ 个点 $M$ 条边的有向无环图 $G$，求图中的最长路。

拓扑排序+dp。

设 $dp[u]$ 表示以结点 $u$ 为终点的最长路长度，则
$$
dp[v] = \max_{<u,v>\in G}(dp[v], dp[u] + 1)
$$
拓扑排序+dp即可。



## **H - Grid 1**

经典问题paths on the grid。



## **I - Coins**

> 有 $N$ 个硬币，抛出第 $i$ 个硬币后，该硬币正面朝上的概率为 $p_i$，求正面朝上的硬币总数多于反面朝上的硬币总数的概率。

概率dp。

$dp[i][j]$ 表示前 $i$ 个硬币中有 $j$ 个硬币正面朝上的概率，则
$$
dp[i][j] = dp[i-1][j-1]\times p_{i} + dp[i-1][j]\times (1-p_{i})
$$

> 或者用多项式的思路解决：每一次抛硬币显然可以表示为 $(1-p_i)+p_i x$，然后用FFT算出 $[(1-p_i)+p_i x]^N$ 中每一项的系数即可，$O(N\log^2(N))$。



## **J - Sushi**

> 有 $N$ 种菜，第 $i$ 种菜由 $a_i$ 个寿司构成，重复以下操作，直到所有寿司被吃完：
>
> - 等概率地从 $1,2,\cdots,N$ 中选一个数，如果第 $i$ 种菜还有寿司，则吃掉一个寿司；否则什么也不做。
>
> 求吃完所有寿司的期望操作次数。

期望dp。

设 $dp[i][j][k]$ 分别表示当前剩余3个寿司、2个寿司、1个寿司的菜还有 $i,j,k$ 种时的期望操作次数，则答案就是 $dp[i][j][k]$。然后就有
$$
dp[i][j][k] = \frac{i}{N}\times dp[i-1][j][k] + \frac{j}{N}\times dp[i+1][j-1][k-1] + \frac{k}{N}\times dp[i][j+1][k-1] + \frac{N-i-j-k}{N}\times dp[i][j][k] + 1
$$

移项可得
$$
\frac{i+j+k}{N}dp[i][j][k] = \frac{i}{N}\times dp[i-1][j][k] + \frac{j}{N}\times dp[i+1][j-1][k-1] + \frac{k}{N}\times dp[i][j+1][k-1] + 1
$$
然后记忆化搜索即可。



## **K - Stones**

> 给定 $N$ 个正整数 $A_i$，和 $K$ 个石子，Alice和Bob两个人玩游戏，每个人可以轮流从 $A$ 中选一个数字 $A_j$，然后从石子中取走 $A_j$ 个石子，谁不能取就输了，询问谁会赢？
>
> $N\le 100,K\le 10^5$

经典的[公平组合游戏博弈状态](https://oi-wiki.org/math/game-theory/impartial-game/#%E5%8D%9A%E5%BC%88%E5%9B%BE%E5%92%8C%E7%8A%B6%E6%80%81)。



## **L - Deque**

minmax博弈。这类博弈都可以用以下搜索解决：

```cpp
int dfs(State state) {
    if (end_state) return 0;
    int res = -INF;
    for (nxt_state : state) {
        res = max(res, score - dfs(nxt_state));  // 这里的score是从state跳转到nxt_state过程中获得的分数
    }
    return res;
}
```

来到本题，设 $dp[l][r]$ 表示当前剩下的数字区间为 $[l,r]$，则该博弈的状态数是 $O(N^2)$ 的，套用上方模板加上记忆化即可解决本题。



## **M - Candies(TODO)**

## **N - Slimes**

> 石子合并

经典区间dp。

## **O - Matching**

状压dp。

## **P - Independent Set**

树形dp。

## **Q - Flowers**

LIS变形。

## **R - Walk**

矩阵快速幂。

## **S - Digit Sum**

数位dp。

## **T - Permutation**

前缀和优化dp。

## **U - Grouping**

$O(3^N)$ 枚举子集dp。

## **V - Subtree**

> 对一棵 $N$ 个点的树黑白染色，对于每一个点询问：
>
> - 如果第 $i$ 个被染色为黑，所有黑色点只构成一个连通块的方案数。
>
> $N\le 2e5$。

换根dp。

## **W - Intervals**

线段树优化dp。

## **X - Tower**

贪心+01背包dp。

## **Y - Grid 2**

计数dp，容斥原理。

## **Z - Frog 3**

斜率优化。
