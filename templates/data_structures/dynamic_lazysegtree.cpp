template<class S, auto op, auto e, class F, auto mapping, auto composition, auto id>
    class dynamicSegTree {
    public:
        dynamicSegTree() : dynamicSegTree(0) {}
        // 注意这里尽量多开空间 不知道会申请多少结点
        explicit dynamicSegTree(int n, int m = (int)2e6 * 4 + 10) : _n(n) {
            data.reserve(m);
            lazy.reserve(m);
            t.reserve(m);
            t.push_back({ -1,-1,-1,-1 });
            lazy.push_back(id());
            data.push_back(e());
            nodes = 0;
            make_node(1, n);  // root
        }
        int make_node(int l, int r) {
            t.push_back({ l,r,-1,-1 });
            lazy.push_back(id());
            data.push_back(e());
            return ++nodes;
        }
        void push_up(int rt) {
            auto&& [l, r, lc, rc] = t[rt];
            data[rt] = op(data[lc], data[rc]);
        }
        void apply(int rt, F f) {
            data[rt] = mapping(f, data[rt]);
            lazy[rt] = composition(f, lazy[rt]);
        }
        void push_down(int rt) {
            auto&& [l, r, lc, rc] = t[rt];
            int mid = (l + r) / 2;
            if (lc == -1) {
                lc = make_node(l, mid);
            }
            if (rc == -1) {
                rc = make_node(mid + 1, r);
            }
            if (not (lazy[rt] == id())) {
                apply(lc, lazy[rt]);
                apply(rc, lazy[rt]);
            }
            lazy[rt] = id();
        }
        void range_modify(int rt, int ql, int qr, F f) {
            auto&& [l, r, lc, rc] = t[rt];
            if (r < ql or l > qr) {
                return;
            }
            if (ql <= l and r <= qr) {
                apply(rt, f);
                return;
            }
            push_down(rt);
            int mid = (l + r) / 2;
            range_modify(lc, ql, qr, f);
            range_modify(rc, ql, qr, f);
            push_up(rt);
        }
        S range_query(int rt, int ql, int qr) {
            auto&& [l, r, lc, rc] = t[rt];
            if (r < ql or l > qr) {
                return e();
            }
            if (ql <= l and r <= qr) {
                return data[rt];
            }
            push_down(rt);
            int mid = (l + r) / 2;
            auto&& res = op(range_query(lc, ql, qr), range_query(rc, ql, qr));
            push_up(rt);
            return res;
        }
    private:
        int _n, nodes;
        vector<tuple<int, int, int, int>> t;  // (l, r, lc, rc)
        vector<S> data;
        vector<F> lazy;
    };