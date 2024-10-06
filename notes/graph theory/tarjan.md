# Tarjan

## DFS生成树

![dfs-tree](pics/dfs-tree.svg)

<center>DFS生成树的示例</center>

有向图的 DFS 生成树主要有 4 种边（不一定全部出现）：

1. 树边（tree edge）：示意图中以黑色边表示，每次搜索找到一个还没有访问过的结点的时候就形成了一条树边。
2. 反祖边（back edge）：示意图中以红色边表示（即 $7 \rightarrow 1$），也被叫做回边，即指向祖先结点的边。
3. 横叉边（cross edge）：示意图中以蓝色边表示（即 $9 \rightarrow 7$），它主要是在搜索的时候遇到了一个已经访问过的结点，但是这个结点 **并不是** 当前结点的祖先。
4. 前向边（forward edge）：示意图中以绿色边表示（即 $3 \rightarrow 6$），它是在搜索的时候遇到子树中的结点的时候形成的。

## Tarjan算法

### 强连通分量

#### 强连通分量与DFS生成树的关系

如果结点 $u$ 是某个强连通分量在搜索树中遇到的第一个结点，那么这个强连通分量的其余结点肯定是在搜索树中以 $u$ 为根的子树中。结点 $u$ 被称为这个强连通分量的根。

反证法：假设有个结点 $v$ 在该强连通分量中但是不在以 $u$ 为根的子树中，那么 $u$ 到 $v$ 的路径中肯定有一条离开子树的边。但是这样的边只可能是横叉边或者反祖边，然而这两条边都要求指向的结点已经被访问过了，这就和 $u$ 是第一个访问的结点矛盾了。得证。

#### 算法流程

对于一棵DFS生成树，图上的每个结点需要维护以下信息：

1. $dfn[u]$：深度优先搜索时结点 $u$ 被搜索的次序。

2. $low[u]$：从以结点 $u$ 为根的子树可以一步走到的“最早的结点”的dfn值。设以 $u$ 为根的子树为 $\textit{Subtree}_u$。$\textit{low}[u]$ 定义为以下结点的 $\textit{dfn}$ 的最小值：$\textit{Subtree}_u$ 中的结点；从 $\textit{Subtree}_u$ 通过一条不在搜索树上的边（反祖边、横叉边、前向边）能到达的结点。

   > 对于上方图片“一个DFS生成树的示例”，它的 $dfn,low$ 值如下：
   >
   > |      |  1   |  2   |  3   |  4   |  5   |  6   |  7   |  8   |  9   |
   > | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
   > | dfn  |  1   |  2   |  3   |  4   |  5   |  6   |  7   |  8   |  9   |
   > | low  |  1   |  1   |  3   |  4   |  5   |  6   |  1   |  7   |  7   |
   >
   > ![dfs-tree](pics/dfs-tree.svg)
   >
   > 这里有一个关键的性质：每个满足 $dfn[u]=low[u]$ 的结点 $u$ 一定是一个SCC的根节点。比如本例中就有这些SCC：
   >
   > 1. `1 2 7 8 9`
   > 2. `3`
   > 3. `4`
   > 4. `5`
   > 5. `6`

维护过程中，难点主要在于 $low$ 数组的维护，有以下几种情况（假设当前在结点 $u$，将要遍历的结点为 $v$）：

1. $v$ 未被访问过：此时 $<u,v>$ 是一条树边，在回溯的过程中，更新 `low[u] = min(low[u], low[v])`。

2. $v$ 被访问过：此时 $<u,v>$ 不是一条树边，然后我们可以分两种情况讨论：

   1. $<u,v>$ 是返祖边：在回溯的过程中，更新 `low[u] = min(low[u], dfn[v])`。
   2. $<u,v>$ 是横叉边或前向边：此时不需要更新。（因为横叉边说明 $u,v$ 所在的两棵子树不能互达，即 $u,v$ 不可能在一个SCC中）

   > 这里是最后Tarjan算法求SCC的一个问题：如何判断某一条边 $<u,v>$ 是返祖边？
   >
   > 我们可以在DFS的过程中维护一个当前已经遍历到的结点组成的栈

#### 模板题

[Strongly Connected Components](https://judge.yosupo.jp/problem/scc)



### 割点

判断一个点 $u$ 是否是割点有两种情况：

1. $u$ 是DFS树的根结点：此时只需要检查以 $u$ 为根的DFS树是否有2个儿子。
2. $u$ 不是根结点：此时只需要判断以 $u$ 为根的DFS子树的所有儿子 $v$ 中是否存在 $low[v]\ge dfn[u]$（这表示了儿子结点 $v$ 是否能够不通过父结点 $u$ 回到祖先）。

#### 模板题

[P3388 【模板】割点（割顶）](https://www.luogu.com.cn/problem/P3388)



### 桥（割边）

判断割边类似于割点，但是只有一种情况：

1. 在以 $u$ 为根的DFS子树的所有儿子 $v$ 中，若 $low[v]\gt dfn[u]$ 则 $<u,v>$ 是一条割边（这表明 $v$ 是否能够不通过父结点 $u$ 回到祖先）。

> 割点和割边的区别就在于是否可以取到 $low[v]= dfn[u]$。

#### 模板题

[#10102. 「一本通 3.6 练习 3」旅游航道](https://loj.ac/p/10102)

[1192. 查找集群内的关键连接](https://leetcode.cn/problems/critical-connections-in-a-network/)

### 模板

```cpp
class SCC {
private:
    int cnt = 0;
    int n, m = 0; // 点数 边数
    vector<vector<int>> graph, scc;
    vector<vector<int>> edge_id;
public:
    SCC(int n_) : n(n_), graph(n_), scc(n_), edge_id(n_) {}

    // 建单向边
    void add_edge(int u, int v) {
        graph[u].push_back(v);
        m++;
    }

    // 建双向边
    void add_edge2(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
        edge_id[u].push_back(cnt);
        edge_id[v].push_back(cnt++);
        m += 2;
    }

    // 找强连通分量
    vector<vector<int>> find_sccs() {
        vector<vector<int>> res;
        vector<int> dfn(n), low(n), in_stack(n), stk;

        function<void(int)> tarjan = [&](int u) {
            static int time = 0;
            dfn[u] = low[u] = ++time;
            stk.push_back(u);
            in_stack[u] = 1;

            for (int v : graph[u]) {
                if (!dfn[v]) {
                    tarjan(v);
                    low[u] = min(low[u], low[v]);
                } else if (in_stack[v]) {
                    low[u] = min(low[u], dfn[v]);
                }
            }
            if (dfn[u] == low[u]) {
                vector<int> tmp;
                while (stk.back() != u) {
                    tmp.push_back(stk.back());
                    in_stack[stk.back()] = 0;
                    stk.pop_back();
                }
                tmp.push_back(u);
                in_stack[u] = 0;
                stk.pop_back();
                res.push_back(tmp);
            }
        };

        for (int i = 0; i < n; i++) {
            if (!low[i]) {
                tarjan(i);
            }
        }
        reverse(res.begin(), res.end());
        return res;
    }

    // 求dag
    vector<vector<int>> build_dag() {
        auto scc = find_sccs();
        vector<int> color(n);
        int cnt = scc.size();
        vector<vector<int>> res(cnt);

        for (int i = 0; i < cnt; i++) {
            for (int j : scc[i]) {
                color[j] = i;
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j : graph[i]) {
                if (color[i] != color[j]) {
                    res[color[i]].push_back(color[j]);
                }
            }
        }

        return res;
    }

    // 找割点
    // 返回一个大小为N的0/1数组 0表示不是割点 1表示是割点
    // 建图时务必使用add_edge2()
    vector<int> find_articulation_points() {
        vector<int> dfn(n), low(n), res(n);

        function<void(int, int)> tarjan = [&](int u, int pa) {
            static int time = 0;
            dfn[u] = low[u] = ++time;
            int children = 0;

            for (auto v : graph[u]) {
                if (!dfn[v]) {
                    children++;
                    tarjan(v, u);
                    if (low[v] >= dfn[u] && pa != -1)
                        res[u] = 1;
                    low[u] = min(low[u], low[v]);
                } else if (pa != v) {
                    low[u] = min(low[u], dfn[v]);
                }
            }
            if (pa == -1 && children > 1) {
                res[u] = 1;
            }
        };

        for (int i = 0; i < n; i++) {
            if (!dfn[i]) {
                tarjan(i, -1);
            }
        }
        return res;
    }

    // 找桥/割边
    // 返回一个大小为M的0/1数组 res[i]=0表示第i条边不是桥 1表示是桥
    vector<int> find_bridges() {
        vector<int> dfn(n), low(n), res(m);

        function<void(int, int)> tarjan = [&](int u, int pa) {
            static int time = 0;
            dfn[u] = low[u] = ++time;
            bool flag = false;

            for (int i = 0; i < graph[u].size(); i++) {
                int v = graph[u][i];
                if (!dfn[v]) {
                    tarjan(v, u);
                    if (low[v] > dfn[u])
                        res[edge_id[u][i]] = 1;
                    low[u] = min(low[u], low[v]);
                } else if (flag || pa != v) {
                    low[u] = min(low[u], dfn[v]);
                }
                if (pa == v) {
                    flag = true;
                }
            }
        };

        for (int i = 0; i < n; i++) {
            if (!low[i]) {
                tarjan(i, -1);
            }
        }
        return res;
    }
};
```



## 例题

### [CF427C Checkposts](https://codeforces.com/contest/427/problem/C)

找SCC的简单题。



### [NWERC2008 Proving Equivalences](https://www.luogu.com.cn/problem/UVA12167)

> 题意：给定一个有向图，询问最少加入几条边才能将该图变成一个强连通图。

先缩点建立DAG，然后答案就是 `max(入度为0的点数, 出度为0的点数)`。注意一个corner case：DAG只有一个点（原图就是一个SCC）。

> 答案是 `max(入度为0的点数, 出度为0的点数)` 的思考：将出度为0的点视作源点，入度为0的点视作汇点，然后分情况讨论：
>
> - 1个源点，n个汇点：此时所有的汇点向源点连边即可。
> - n个源点，1个汇点：同理。
> - n个源点，m个汇点（$n,m\ge2$）：此时至少存在两个源点 $s1,s2$，两个汇点 $t1,t2$，并且满足 $s1$ 可以走到 $t1$，$s2$ 可以走到 $t2$，我们连接 $<s1,t2>,<s2,t1>$ 就可以使这四个点互相可达。这种构造能够一直推广。



### [CF118E Bertown roads](https://codeforces.com/contest/118/problem/E)

> 题意：给定一个联通的无向图，将每条边都改为有向边，使得整个图是一个SCC。询问是否可行，如果可行则构造一种方案。

容易发现如果无向图中存在桥就不可行，可行解直接按照DFS树对每条边的遍历顺序构造即可。



### [CF732F Tourist Reform](https://codeforces.com/contest/732/problem/F)

> 题意：给定一个联通的无向图，将每条边都改为有向边。对于有向图上的任意点 $i$，$r_i$ 表示从点 $i$ 出发能够到达的不同点数，构造一种方案使得 $\min(r_i),i=1,2,\cdots,n$ 取最大值。求出这个最大值，并且给出一个构造。

这题跟上一题很像，核心点仍然是桥边。

- 如果没有桥边，那么无向图一定可以利用DFS树构造为一个SCC。
- 如果有桥边，那么整张图实际上就会变成数个边双连通分量，每个边双连通分量内部都可以构造为SCC。然后我们就只需要考虑桥边的方向，显然所有的边双连通分量我们可以视作一个点，然后建一个新图，这个图的形态是一棵树，有向树上必然至少有一点的出度为0（也就是该点无法到达其他任何一点），因此我们只需要找到最大的边双连通分量，然后以该点为汇点，树上其余所有点流向该点即可。
