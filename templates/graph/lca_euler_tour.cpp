// test link: https://judge.yosupo.jp/submission/236788
#ifdef _MSC_VER
int __builtin_clz(unsigned int x) {
    if (x == 0) {
        return 32;  // 0 的 clz 是未定义行为，通常返回 32，因为 32 位整数有 32 个前导零
    }
    unsigned long index;
    _BitScanReverse(&index, x); // index 是最高位 1 的位置
    return 31 - index;          // clz 计算的是前导零
}
#endif // _MSC_VER

struct lca {
private:
    vector<int> et, d;
    vector<pair<int, int>> range;
    vector<vector<int>> f_min, min_index;
public:
    lca(int rt, const vector<vector<int>>& g) {  // 传入根节点id和二维邻接表, O(N) euler tour预处理, O(NlogN)建ST表
        int n = g.size();
        et.reserve(n * 2), d.reserve(n * 2), range.resize(n);

        auto dfs = [&](auto&& self, int u, int from, int dep) -> void {
            range[u].first = range[u].second = d.size();
            et.push_back(u);
            d.push_back(dep);
            for (auto&& v : g[u]) {
                if (v == from) continue;
                self(self, v, u, dep + 1);
                range[u].second = d.size();
                et.push_back(u);
                d.push_back(dep);
            }
        };
        dfs(dfs, rt, -1, 0);

        int _log = 0, m = d.size();
        while ((1 << _log) < m) ++_log;
        f_min.resize(_log), min_index.resize(_log);
        f_min[0].resize(m), min_index[0].resize(m);
        for (int i = 0; i < m; i++) {
            f_min[0][i] = d[i], min_index[0][i] = i;
        }
        for (int i = 1; i < _log; i++) {
            int _sz = f_min[i - 1].size() - (1 << (i - 1));
            f_min[i].resize(_sz), min_index[i].resize(_sz);
            for (int j = 0; j < _sz; j++) {
                if (f_min[i - 1][j] < f_min[i - 1][j + (1 << (i - 1))]) {
                    f_min[i][j] = f_min[i - 1][j];
                    min_index[i][j] = min_index[i - 1][j];
                } else {
                    f_min[i][j] = f_min[i - 1][j + (1 << (i - 1))];
                    min_index[i][j] = min_index[i - 1][j + (1 << (i - 1))];
                }
            }
        }
    }
    int query(int u, int v) {  // O(1) 查询u和v的LCA
        int l = min(range[u].first, range[v].first), r = max(range[u].second, range[v].second);
        if (l == r) {
            return et[min_index[0][l]];
        }
        int s = 31 - __builtin_clz(r - l);
        if (f_min[s][l] < f_min[s][r - (1 << s) + 1]) {
            return et[min_index[s][l]];
        } else {
            return et[min_index[s][r - (1 << s) + 1]];
        }
    }
};