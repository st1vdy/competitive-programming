# AtCoder Beginner Contest 371题解

## **E - I Hate Sigma Problems**

> 定义函数 $f(l,r)$ 表示 $A[l],A[l+1],\cdots,A[r]$ 中不同的元素个数，求
> $$
> \sum_{i=1}^N\sum_{j=i}^N f(i,j).
> $$

其实就是求数组 $A$ 中所有区间的元素种类数。可以通过枚举不同元素的贡献来解决，以样例 `5 4 2 2 3 2 4 4 1` 为例，可以枚举元素 $1,2,3,4,5$；以元素 $2$ 为例，此时我们只关注该元素，即数组 `x x 2 2 x 2 x x x`。

要做的就是求出元素2出现在哪些区间中，反向思考：求出元素2不出现哪些区间中本题就解决了。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n;
    cin >> n;
    map<int, vector<int>> mp;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        if (!mp.count(x)) {
            mp[x].push_back(0);
        }
        mp[x].push_back(i + 1);
    }
    
    ll res = 0;

    auto solve = [&](vector<int>& v) {
        ll ans = 1ll * n * (n + 1) / 2;
        v.push_back(n + 1);
        for (int i = 1; i < v.size(); i++) {
            ans -= 1ll * (v[i] - v[i - 1]) * (v[i] - v[i - 1] - 1) / 2;
        }
        return ans;
    };

    for (auto&& [k, v] : mp) {
        res += solve(v);
    }
    cout << res;
}
```



## **F - Takahashi in Narrow Road**

> $N$ 个人初始分布在一维数轴上，第 $i$ 个人位于位置 $X_i$，且 $0\le X_1\lt X_2\lt\cdots\lt X_N\le 10^8$。
>
> 进行以下操作 $Q$ 次：
>
> - 指定第 $T_i$ 个人前往坐标 $G_i$。但是有一个要求：任意两个人的位置不能重合。
>
> 求出所有人总共的移动距离。

我们可以将每一个人看作一个“块”，当某个人要前往一个坐标且需要推动别人前进时，我们可以认为这个人和被推动的人所在的块合并了。当一个“大块”内部的某个人被指定移动时，这个“大块”就可以被认为是分裂成了两个新的块，一个块在原地，另一个块则发生了移动并可能继续与其他块合并。

由于块的合并必定会减少块的数量，而块的分裂至多令块的总数加一，所以我们可以直接维护这个块的合并、分裂过程。需要的操作就是：

- 查询第k个人在当前的哪一个块中
- 块的合并/分裂

块的合并/分裂就是模拟。查询第k个人在当前的哪一个块中可以通过维护前缀和+二分的方法搞定，这是因为本题中每个人的相对位置是不会变的。具体实现上，我们只需要维护每个块覆盖的区间 $[l,r]$ 并将这个块挂在位于 $l$ 的这个人的排名所在的结点上。假设我们当前在维护的区间是 $[1,1],[12,13],[20,22],[30,30]$，那么这4个区间分别应该挂在结点 $1,2,4,7$ 上。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct seg {
    int l, r;
    bool operator < (const seg& rhs) {
        return l < rhs.l;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n, q;
    cin >> n;
    map<int, seg> mp;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        mp[i] = { x,x };
    }
    mp[(int)1e9] = { (int)2e9,(int)2e9 };
    mp[(int)-1e9] = { (int)-2e9,(int)-2e9 };

    ll res = 0;
    cin >> q;
    while (q--) {
        int t, g;
        cin >> t >> g;
        --t;
        auto&& it = mp.upper_bound(t);
        it--;
        auto pre_it = it;
        int pl = pre_it->second.l, pr = pre_it->second.r;
        int current_pos = it->second.l + (t - it->first);
        int pt = current_pos;
        int lo = pt, hi = pt;

        ll res_q = 0;
        if (current_pos > g) {  // go left
            // split node
            // [l, r] split into [l, current_pos], [current_pos + 1, r]
            res_q += 1ll * (current_pos - pl + 1) * (current_pos - g);
            lo = g - (current_pos - pl);
            if (pr > current_pos) {
                mp[t + 1] = { current_pos + 1, pr };
            }
            it--;
            mp.erase(pre_it);
            pre_it = it;
            pl = it->second.l, pr = it->second.r;
            int pl_target = g - t + it->first;  // taeget position that pl is going to
            
            while (pl > pl_target) {
                lo = pl_target;
                res_q += 1ll * (pr - pl + 1) * (pl - pl_target);
                it--;
                mp.erase(pre_it);
                pre_it = it;

                pl = it->second.l, pr = it->second.r;
                pl_target = g - t + it->first;
            }
            mp[t - g + lo] = { lo,g };
        } else if (current_pos < g) {  // go right
            // split node
            // [l, r] split into [l, current_pos - 1], [current_pos, r]
            res_q += 1ll * (pr - current_pos + 1) * (g - current_pos);
            hi = g + (pr - current_pos);
            it++;
            if (current_pos > pl) {
                mp[pre_it->first] = { pl,current_pos - 1 };
            } else {
                mp.erase(pre_it);
            }
            pre_it = it;

            pl = it->second.l, pr = it->second.r;
            int pr_target = g + it->first - t + pr - pl;  // target position that pr is going to
            while (pr < pr_target) {
                hi = pr_target;
                res_q += 1ll * (pr - pl + 1) * (pr_target - pr);
                it++;
                mp.erase(pre_it);
                pre_it = it;

                pl = it->second.l, pr = it->second.r;
                pr_target = g + it->first - t + pr - pl;
            }
            mp[t] = { g,hi };
        }
        res += res_q;
    }
    cout << res;
}
```



## **G - Lexicographically Smallest Permutation**

> 给定两个排列 $P$ 和 $A$。你可以进行以下操作任意次：
>
> - 对于所有 $i$，将 $A_i$ 替换为 $A_{P_i}$。
>
> 求字典序最小的 $A$。

首先很显然的是这个替换的操作必定是一堆置换环，然后目标是字典序最小，因此考虑贪心，$A[0]$ 这个位置必定要让元素 $A[0]$ 所在的置换环上最小的元素占据。假设 $A[0]$ 所在的环大小为 $M_0$，将最小元素换到 $A[0]$ 的操作次数为 $T_0$，则对于其他任意一个环而言，他们就只能执行操作 $T_0+nM_0$ 次了。

然后再考虑怎么让第二个环取到最小值，如果不考虑第一个环的限制，假设环的大小是 $M_1$，将最小元素换到环的最左侧需要 $T_1$ 次操作，那么我们就只需要检测同余方程组
$$
\begin{cases}
x\equiv T_0\pmod{M_0}\\
x\equiv T_1\pmod{M_1}\\
\end{cases}
$$
是否有解。这是一个经典模型，根据裴蜀定理，只需要检测 $T_0-T_1$ 是否整除 $\gcd(M_0,M_1)$ 即可。但是，由于方程组可能无解（即第二个环无法令最小元素换到最左），所以我们实际上需要枚举出所有可行的 $T_1$，并在所有可行的 $T_1$ 中找到字典序最小的那个。

可以用exgcd将同余方程 $T_0\pmod{M_0}\equiv T_1\pmod{M_1}$ 合并为一个新的同余方程 $x\equiv T^\prime\pmod{M^\prime}$，然后重复上述操作直到找到所有环的最小可能字典序。这个做法实际上就是[excrt](https://oi-wiki.org/math/number-theory/crt/#%E4%B8%A4%E4%B8%AA%E6%96%B9%E7%A8%8B)。

当然也可以简化这一流程，因为我们可以直接枚举所有可能的 $T_1$。仍然沿用上方的符号，设第二个环的大小为 $M_1$，那么多少个位置可能被枚举到呢？即方程组
$$
\begin{cases}
x\equiv T_0\pmod{M_0}\\
x\equiv y\pmod{M_1}\\
\end{cases}
$$
中 $y$ 的可能取值数量。这个值就是 $\frac{\text{lcm}(M_0,M_1)}{M_0} = \frac{M_1}{\gcd(M_0,M_1)}$，因此我们可以直接枚举 $T_0+nM_1 \pmod{M_2},n\in[0,\frac{M_1}{\gcd(M_0,M_1)})$，并找到最小的 $A[i]$。

需要注意的是这里将同余方程组合并为方程 $x\equiv T^\prime\pmod{M^\prime}$ 时的 $M^\prime$ 可能很大，因为 $M^\prime = \text{lcm}(M_0,M_1)$，因此直接做需要大数或者python。

```python
import sys
from math import gcd
readline = sys.stdin.readline


n = int(readline())
p = list(map(int, readline().split()))
a = list(map(int, readline().split()))
p = [i - 1 for i in p]
visited = [0 for _ in range(n)]
t0, m0 = 0, 0


for i in range(n):
    if visited[i]:
        continue

    cycle = [a[i]]
    pos = p[i]
    while cycle[0] != a[pos]:
        visited[pos] = 1
        cycle.append(a[pos])
        pos = p[pos]

    if m0 == 0:
        t0 = min((cycle[i], i) for i in range(len(cycle)))[1]
        m0 = len(cycle)
        pos = i
        for j in range(m0):
            a[pos] = cycle[(j + t0) % m0]
            pos = p[pos]
    else:
        m1 = len(cycle)
        step = m0
        cnt = m1 // gcd(m0, m1)
        m0 *= cnt
        min_pos = min((cycle[(t0 + step * i) % m1], i) for i in range(cnt))[1]
        t0 += min_pos * step
        t1 = t0
        pos = i
        for j in range(m1):
            a[pos] = cycle[(j + t1) % m1]
            pos = p[pos]


print(" ".join(map(str, a)))
```

