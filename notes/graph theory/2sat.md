# 2-SAT

## 2-SAT建模

有 $n$ 个布尔变量 $x_0,x_1,\ldots, x_{n-1}$ 满足以下形式的逻辑表达式：$(x_i \cup x_j)\cap\cdots (x_u\cup  x_{v})\cap \cdots = \text{true}$ 。要求你判断这些逻辑表达式是否能够同时成立，并给出一组构造方案的问题我们称为2-SAT问题（[2-satisfiability](https://en.wikipedia.org/wiki/2-satisfiability)）。

这么说可能有一点抽象，我们不妨用一个2-SAT模板题入门：[**H - Two SAT**](https://atcoder.jp/contests/practice2/tasks/practice2_h)。

> 有 $N$ 面旗帜，每一面旗帜 $i$ 都可以放在一维数轴上的点 $X_i$ 或者点 $Y_i$ 。任意两面旗帜之间的距离不能小于 $D$ ，询问是否存在一种合法的放置方案。

旗帜 $i$ 可以放在点 $X_i$ 或 $Y_i$ 这个条件显然可以看作一个布尔变量 $a_i$ ，其中 $a_i=\text{true}$ 表示放在 $X_i$ ，否则表示放在 $Y_i$ 。

然后，我们遍历任意两个布尔变量 $a_i,a_j(i<j)$ ，并且分四类讨论：

1. 如果 $a_i=\text{true} \cup a_j=\text{true}$ 并且 $|X_i-X_j| < D$（旗帜 $i$ 放在 $X_i$，旗帜 $j$ 放在 $X_j$，但是 $X_i$ 和 $X_j$ 之间的距离小于 $D$），那么显然有 $(\neg a_i \cup \neg a_j)=\text{true}$ ，这是因为 $a_i=\text{true} 和 a_j=\text{true}$ 不能同时成立等价于 $a_i = \text{false}$ 和 $a_j = \text{false}$ 至少有一个成立。
2. 如果 $a_i=\text{true} \cup a_j=\text{false}$ 并且 $|X_i-X_j| < D$ ，那么显然有 $(\neg a_i \cup a_j)$ 。
3. 如果 $a_i=\text{false} \cup a_j=\text{true}$ 并且 $|X_i-X_j| < D$ ，那么显然有 $(a_i \cup \neg a_j)$ 。
4. 如果 $a_i=\text{false} \cup a_j=\text{false}$ 并且 $|X_i-X_j| < D$ ，那么显然有 $(a_i \cup a_j)$ 。

这样我们就得到了 $O(N^2)$ 个逻辑表达式，并且建立了2-SAT的模型，接下来我们将讨论如何解决2-SAT问题。



## 解法

我们可以建一个 $2n$ 个点的有向图，对于每个布尔变量 $a_i$ 我们都拆成两个点 $a_i$ 和 $a^\prime_i$ 分别表示 $a_i$ 为真和假（下文中将沿用这个定义）。

对于每组逻辑表达式 ($a_i\cup a_j$) 我们建这样两条边：$a^\prime_i \rightarrow a_j$ 和 $a^\prime_j\rightarrow a_i$ 。这个建边表示如果选了 $a^\prime_i$ 就必选 $a_j$ 或者如果选了 $a^\prime_j$ 就必选 $a_i$ ，也就是逻辑表达式 ($a_i\cup a_j$) 的含义。

那么现在，每一个逻辑变量 $a_i$ 都有以下四种情况：

1. $a_i$ 和 $a^\prime_i$ 无关：无事发生。
2. $a_i\rightarrow a^\prime_i$ ：那么 $a_i$ 取假即可避免矛盾。
3. $a^\prime_i\rightarrow a_i$ ：那么 $a_i^\prime$ 取假即可避免矛盾。
4. $a_i\rightarrow a^\prime_i \cap a^\prime_i\rightarrow a_i$ ：此时无论如何都不能避免矛盾，因此无解。

注意到我们可以通过Tarjan缩点的方法，将所有的强连通分量找到（只有SCC中才有可能出现情况4，即 $a_i$ 和 $a^\prime_i$ 在同一个SCC中），因此我们可以通过SCC对2-SAT问题进行快速的求解。

2-SAT的构造可以利用DAG的性质：我们缩点之后一定会得到一个DAG，因此我们可以根据 $a_i$ 和 $a^\prime_i$ 间的拓扑序确定某个变量为真或假。



## 例题

### [Gym104059G Guessing Game](https://codeforces.com/gym/104059/problem/G)

> 在一个星期中，每天有 $d_i(i=1,2,\cdots,7)$ 场比赛，每一场比赛只有两种结果：来自Greenville的村民胜利，或者来自Tomatown的村民胜利。Alan和他的 $N$ 个朋友在每一天会选择当天发生的某一场比赛（每个人都可能选不同的一场比赛），并对比赛结果做出预测。但是Alan只能对最后两天的比赛进行预测（他的朋友每人会预测7场）。
>
> 询问：每一场比赛的结果由你安排，Alan的预测正确数能否严格大于他的朋友。
>
> $N\le 5\times 10^4,\sum d_i\le 10^5$。

每一场比赛的预测结果只有两种，这就容易想到这就是一个2-SAT模型。

显然Alan的最优情况只能是两次预测都正确，而他的朋友应该尽量都预测错，更确切地说：他的朋友最多预测正确一次。我们假设Alan的两次预测是两个布尔变量：$a1,a2$，朋友 $i$ 的七次预测是七个布尔变量 $p_{i1},p_{i2},\cdots,p_{i7}$，那就有：

1. Alan的预测全对：$a1=1\cap a2=1$
2. 朋友 $i$ 最多对1个预测：$\underset{1\le j\lt k\le 7}{\cap} (\neg p_{ij}\cup \neg p_{ik})=1$。

只需要检测上方的约束条件能否全部满足即可。