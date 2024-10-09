# 斜率优化

## 斜率优化前置

## 例题

### [CF1083E. The Fair Nut and Rectangles](https://codeforces.com/contest/1083/problem/E)

> 给定 $N$ 个两两之间互不包含的矩形，每个矩形的左下角为原点，右上角为 $(x_i,y_i)$，每个矩形有一个权重 $a_i$。选出一个矩形的集合使得该集合中矩形的面积并减去权重之和最大。

令 $f[i]$ 表示选第 $i$ 个矩形时的最大分数，则
$$
\begin{aligned}
f[i] &= \max_{j\lt i}(f[j]-a_i+y_i(x_i-x_j))\\
&= \max_{j\lt i}(f[j]-x_jy_i)-a_i+x_iy_i
\end{aligned}
$$
到这里就是一个显然的斜率优化形式了 $y=kx+m$ 的形式为 $k=-x_j,m=f[j]$。

### [APIO10A - Commando](https://www.spoj.com/problems/APIO10A/)

> 有 $n$ 个士兵，每个士兵的战斗力是 $x_i$，你作为指挥官可以将士兵们划分为一系列连续的区间，每个区间中士兵们的修正战斗力为 $a(\sum x_i)^2+b(\sum x_i)+c$，这里 $\sum x_i$ 表示区间中所有士兵战力之和，最大化修正战斗力之和。

令 $f[i]$ 表示恰好将士兵 $i$ 作为最后一人划分入一个区间时前 $i$ 人的最大修正战斗力之和，则
$$
f[i] = \max_{j\lt i}(f[j]+g(i+1,j))
$$
这里 $g(i+1,j)$ 表示 $a(\sum_{k=j+1}^i x_k)^2+b(\sum_{k=j+1}^i x_k)+c$，这里的连续区间之和我们用前缀和 $p_i=\sum_{j=1}^i p_j$ 表示，则
$$
f[i] = \max_{j\lt i}(f[j]+ap_j^2-bp_j-2ap_jp_i) + ap_i^2+bp_i+c
$$
到这里就是一个显然的斜率优化形式了 $y=kx+m$ 的形式为 $k=-2ap_j,m=f[j]+ap_j^2-bp_j$。

### [Covered Walkway](https://open.kattis.com/problems/coveredwalkway)

### [IOI '02 P4 - Batch Scheduling](https://dmoj.ca/problem/ioi02p4)

> 有 $N$ 件工作，你要将所有工作分成数个批次，每个批次内的工作都是一段连续区间，你要从左往右依次处理每个批次，每件工作有两个属性 $o_i,f_i$，处理每个批次之前你需要先花费 $s$ 的时间调整机器。设当前批次的区间为 $[i,j]$，则该批次的处理代价为 `(之前已经花费的总时长 + s + o[i] + o[i + 1] + ... + o[j]) x (f[i] + f[i + 1] + ... + f[j])`。求处理完所有批次的最小代价。

容易想到一个简单的转移是 $f_i$ 表示处理前 $i$ 件工作的最小代价，则
$$
f[i] = \min_{j\lt i} (f[j] + (s+t[j]+po[i]-po[i-1])\times(pf[i]-pf[j-1]))
$$
这里 $po$ 表示 $o$ 的前缀和数组，$pf$ 表示 $f$ 的前缀和数组，$t[j]$ 表示做完前 $j$ 件工作花费的时间。但是直接这么做有问题，因为我们无法确认 $f[j]$ 最优时一定能推出 $f[i]$，这是因为 $t[j]$ 这个量的存在，我们实际上还需要枚举之前的批次数量。但是这么做的复杂度就不对了。

正解是逆推，设 $f_i$ 表示处理 $[i,n]$ 区间内所有工作的最小代价，然后就有下列递推关系
$$
f_i = \min_{k\gt i} \bigg(f_k + (s + \sum_{j=i}^{k-1}o_j)\times \sum_{j=i}^{n}f_j \bigg)
$$
这里 $f_{n+1}=0$，而 $f_1$ 就是本题答案。这个转移成立的关键就是注意到每一件工作对于代价的贡献实际上就是 $f_i\times time_i$，这里的 $time_i$ 是第 $i$ 个批次的工作全部完成时的时间花费，而这其实是一个可分离的量。

然后就可以转化为斜率优化形式了。复杂度 $O(N)$。

### [ABC289G - Shopping in AtCoder store](https://atcoder.jp/contests/abc289/tasks/abc289_g)

> 有 $N$ 个顾客，每个人有一个意愿值 $B_i$；$M$ 件商品，每件商品的价值为 $C_i$。对于商品 $i$，假设该商品定价为 $P_i$，则所有满足 $B_j+C_i\ge P_i$ 的顾客 $j$ 都会购买一件商品 $i$，对于所有商品求它能创造的最大利益（购买人数乘定价）。

假设这里只考虑商品 $i$，那么我们只需要先对 $B$ 从大到小排序，再枚举商品 $i$ 的出售数量，则我们的所求答案就是
$$
\max_{j=1}^{N}(j\times(B_j+C_i))
$$
不难发现如果我们将 $C_i$ 看作自变量，那么这就是一堆一次函数（$k=j,m=jB_j$）求最值的问题，用斜率优化即可处理。
