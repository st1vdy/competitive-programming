# 背包

## 0-1背包

> [[USACO07DEC] Charm Bracelet S](https://www.luogu.com.cn/problem/P2871)
>
> 有 $n$ 个物品和一个容量为 $W$ 的背包，每个物品有重量 $w_{i}$ 和价值 $v_{i}$ 两种属性，要求选若干物品放入背包使背包中物品的总价值最大且背包中物品的总重量不超过背包的容量。

设 DP 状态 $f_{i,j}$ 为在只能放前 $i$ 个物品的情况下，容量为 $j$ 的背包所能达到的最大总价值。

考虑转移。假设当前已经处理好了前 $i-1$ 个物品的所有状态，那么对于第 $i$ 个物品，当其不放入背包时，背包的剩余容量不变，背包中物品的总价值也不变，故这种情况的最大价值为 $f_{i-1,j}$；当其放入背包时，背包的剩余容量会减小 $w_{i}$，背包中物品的总价值会增大 $v_{i}$，故这种情况的最大价值为 $f_{i-1,j-w_{i}}+v_{i}$。

由此可以得出状态转移方程：

$$
f[i][j]=\max(f[i-1][j],f[i-1][j-w_{i}]+v_{i})
$$
注意到 $f_i$ 只和 $f_{i-1}$ 有关，因此可以用滚动数组优化掉一个维度
$$
f[j]=\max \left(f[j],f[j-w_i]+v_i\right)
$$
注意到 $f[j]$ 只能由 $f[j]$ 或者 $f[j-w_i]$ 推得，而 $j,j-w_i\le j$，也就是说如果我们从大到小遍历背包的值域，那么 $f_j$ 就不会受到前一轮dp的影响

```cpp
for (int i = 1; i <= n; i++) {
    for (int j = W; j >= w[i]; j--) {
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
}
```

## 完全背包

完全背包模型与 0-1 背包类似，与 0-1 背包的区别仅在于一个物品可以选取**无限次**，而非仅能选取一次。

[P1616 疯狂的采药](https://www.luogu.com.cn/problem/P1616)

设 DP 状态 $f_{i,j}$ 为在只能放前 $i$ 个物品的情况下，容量为 $j$ 的背包所能达到的最大总价值。则转移方程为
$$
f[i][j] = \max( f[i-1][j], f[i-1][j-w_i]+v_i,f[i-1][j-2w_i]+2v_i,\cdots ) = \max_{k=0}^{+\infty}(f[i-1][j-kw_i]+kv_i)
$$
如果用朴素的dp去求解，那么这个递推是 $O(n^3)$ 的。但是注意到 $f[i][j-kw_i] = \max(f[i][j-kw_i], f[i][j-kw_i-w_i]+v_i)$ 是一个连贯的递推关系，所以这个dp式子等价于
$$
f[i][j] = \max(f[i-1][j],f[i-1][j-w_i]+v_i)
$$
但是注意，在完全背包中，我们要让后继结点被前驱结点影响，所以值域的枚举是正向的。

也就是说只需要调转0-1背包的枚举顺序，让后继结点被前驱结点影响即可

```cpp
for (int i = 1; i <= n; i++) {
    for (int j = w[i]; j <= W; j--) {
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
}
```

## 多重背包

多重背包也是 0-1 背包的一个变式。与 0-1 背包的区别在于每种物品有 $k_i$ 个，而非一个。

[P1776 宝物筛选](https://www.luogu.com.cn/problem/P1776)

一个很朴素的想法就是：把「每种物品选 $k_i$ 次」等价转换为「有 $k_i$ 个相同的物品，每个物品选一次」。这样就转换成了一个 0-1 背包模型，套用上文所述的方法就可已解决。状态转移方程如下：
$$
f[i][j]=\max_{k=0}^{k_i}(f[i-1][j-k\times w_i]+k\times v_i)
$$
代码上表现为，在0-1背包的基础上再加一维用于枚举 $k$

```cpp
for (int i = 1; i <= n; i++) {
    for (int weight = W; weight >= w[i]; weight--) {
        // 多遍历一层物品数量
        for (int k = 1; k * w[i] <= weight && k <= cnt[i]; k++) {
            dp[weight] = max(dp[weight], dp[weight - k * w[i]] + k * v[i]);
        }
    }
}
```

时间复杂度为 $O(W\sum_{i=1}^n k_i)$。

### 二进制分组优化

在朴素的多重背包解决方法中，我们将「至多选 $k_i$ 次物品 $i$ 」等价为了「有 $k_i$ 个物品 $i$，每个物品至多选一次」，而这个条件可以用二进制分组优化。举个例子，假设 $k_i=13$，那么我们不需要将条件转化为「有 $13$ 个物品 $i$，每个物品至多选一次」，而是转化为有以下四种物品：

1. 物品1：$weight=1w_i,value=1v_i$
2. 物品2：$weight=2w_i,value=2v_i$
3. 物品3：$weight=4w_i,value=4v_i$
4. 物品3：$weight=7w_i,value=7v_i$

通过这4种物品的线性组合，我们就能表示出 $[0,13]$ 内的所有数字了。时间复杂度 $O(W\sum_{i=1}^n\log k_i)$。

```cpp
vector<int> dp(W + 1), w, v;
for (int i = 1; i <= n; i++) {
    int value, weight, m;
    cin >> value >> weight >> m;
    for (int j = 1;; j *= 2) {
        if (m <= j) {
            w.push_back(m * weight);
            v.push_back(m * value);
            break;
        }
        w.push_back(j * weight);
        v.push_back(j * value);
        m -= j;
        if (m <= j) {
            w.push_back(m * weight);
            v.push_back(m * value);
            break;
        }
    }
}
for (int i = 0; i < w.size(); i++) {
    for (int j = W; j >= w[i]; j--) {
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
}
```

### 单调队列优化

观察dp方程
$$
f[i][j]=\max_{k=0}^{k_i}(f[i-1][j-k\times w_i]+k\times v_i)
$$
不难发现 $f[i][j]$ 实际上实在求一段“跳跃的连续区间” `j, j - wi, j - 2wi, ..., j - kiwi` 上的最大值，只要我们能把它转换为连续区间上的最值问题，那么就可以用单调队列求解了。

设 $g[i][x][y]=f[i][x\times w_i+y]$，则转移方程可以重写为
$$
\begin{aligned}
g[i][x][y] &= \max_{k=0}^{k_i} (g[i-1][x-k][y] + k\times v_i)\\
&= \max_{k=0}^{k_i} (g[i-1][x-k][y] - (x-k)\times v_i) + x\times v_i\\
\end{aligned}
$$
这就变成了一个常见的单调队列优化dp形式，只需要单调队列维护 $\max_{k=0}^{k_i} (g[i-1][x-k][y] - (x-k)\times v_i)$ 即可。

```cpp
// https://www.luogu.com.cn/record/180486169
int n, W;
cin >> n >> W;
vector<int> v(n), w(n), m(n);
vector<ll> dp1(W + 1), dp2(W + 1), mx(W + 1);
for (int i = 0; i < n; i++) {
    cin >> v[i] >> w[i] >> m[i];
}
auto monotone_max = [&](int y, int mi, ll vi, int wi) {
    deque<pair<ll, int>> dq;
    int j = 0;
    for (int x = 0; x <= mi; x++, j++) {
        if (x * wi + y > W) break;
        while (!dq.empty() && dp1[x * wi + y] > dq.back().first + vi * (x - dq.back().second)) dq.pop_back();
        dq.push_back({ dp1[x * wi + y],x });
        mx[x] = dq.front().first - vi * dq.front().second;
    }
    for (int x = mi + 1; x * wi + y <= W; x++) {
        while (!dq.empty() && x - dq.front().second > mi) dq.pop_front();
        while (!dq.empty() && dp1[x * wi + y] > dq.back().first + vi * (x - dq.back().second)) dq.pop_back();
        dq.push_back({ dp1[x * wi + y],x });
        mx[x] = dq.front().first - vi * dq.front().second;
    }
};
for (int i = 0; i < n; i++) {
    for (int y = 0; y < w[i]; y++) {
        monotone_max(y, m[i], v[i], w[i]);
        for (int x = 0; x * w[i] + y <= W; x++) {
            dp2[x * w[i] + y] = mx[x] + 1ll * v[i] * x;
        }
    }
    dp1 = dp2;
}
cout << *max_element(dp1.begin(), dp1.end());
```

时间复杂度 $O(nW)$。
