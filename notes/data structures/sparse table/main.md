# Sparse Table

Sparse Table最常见的应用是解决RMQ问题（Range Minimum Query）。下面以求区间最大值为例解释Sparse Table。

Sparse Table的核心思路是倍增，设 $f(i,j)$ 表示区间 $[i,i+2^j-1]$ 中的最大值，那么显然有以下的dp转移：
$$
f(i,j) = \max(f(i,j-1), f(i+2^{j-1},j-1))
$$
![img](main.assets/st-preprocess-lift.svg)

显然这个预处理是 $O(N\log N)$ 的。

如果我们直接从Sparse Table中利用倍增来求任意区间最值，那么每一次计算是 $O(\log N)$ 的，但是这里我们可以利用最值查询可重叠的性质直接做到 $O(1)$。

对于区间 $[l,r]$ 的最值，我们可以暴力地直接求两个可重叠区间 $[l,l+2^s-1],[r-2^s+1,r]$ 的最值，其中 $s=\lfloor \log_2(r-l+1) \rfloor$。如下图所示：

![ST 表的查询过程](main.assets/st-query.svg)