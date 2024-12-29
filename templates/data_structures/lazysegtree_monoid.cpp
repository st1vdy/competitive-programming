// https://atcoder.jp/contests/practice2/submissions/61229095
#include <bits/stdc++.h>
using namespace std;
const int md = 998244353;
int add(int a, int b) { return a + b > md ? a + b - md : a + b; }
int mul(int a, int b) { return 1LL * a * b % md; }

template<class S, auto op, auto e, class F, auto mapping, auto composition, auto id>
class segTree {
public:
    segTree() : segTree(0) {}
    explicit segTree(int n) : segTree(std::vector<S>(n, e())) {}
    explicit segTree(const vector<S>& v) : _n(int(v.size())) {
        data.resize(4 * _n, e());
        lazy.resize(4 * _n, id());

        auto build = [&](auto&& self, int rt, int l, int r) -> void {
            if (l == r) {
                data[rt] = v[l - 1];
                return;
            }
            int mid = (l + r) / 2;
            self(self, rt * 2, l, mid);
            self(self, rt * 2 + 1, mid + 1, r);
            push_up(rt);
        };
        build(build, 1, 1, _n);
    }
    void push_up(int rt) {
        data[rt] = op(data[2 * rt], data[2 * rt + 1]);
    }
    void apply(int rt, F f) {
        data[rt] = mapping(f, data[rt]);
        lazy[rt] = composition(f, lazy[rt]);
    }
    void push_down(int rt) {
        if (lazy[rt] == id()) {
            return;
        }
        apply(2 * rt, lazy[rt]);
        apply(2 * rt + 1, lazy[rt]);
        lazy[rt] = id();
    }
    void range_modify(int rt, int l, int r, int ql, int qr, F f) {
        if (r < ql or l > qr) {
            return;
        }
        if (ql <= l and r <= qr) {
            apply(rt, f);
            return;
        }
        push_down(rt);
        int mid = (l + r) / 2;
        range_modify(rt * 2, l, mid, ql, qr, f);
        range_modify(rt * 2 + 1, mid + 1, r, ql, qr, f);
        push_up(rt);
    }
    S range_query(int rt, int l, int r, int ql, int qr) {
        if (r < ql or l > qr) {
            return e();
        }
        if (ql <= l and r <= qr) {
            return data[rt];
        }
        push_down(rt);
        int mid = (l + r) / 2;
        auto&& res = op(range_query(rt * 2, l, mid, ql, qr), range_query(rt * 2 + 1, mid + 1, r, ql, qr));
        push_up(rt);
        return res;
    }
private:
    int _n;
    vector<S> data;
    vector<F> lazy;
};

struct S {
    int sum, size;
};
S op(S l, S r) {
    return { add(l.sum, r.sum), l.size + r.size };
}
S e() {
    return { 0,1 };
}

struct F {
    int c, d;  // f(x) = cx + d.
    bool operator == (const F& k) const { return c == k.c and d == k.d; }
};
S mapping(F f, S x) {
    return { add(mul(f.c, x.sum), mul(f.d, x.size)),x.size };
}
F composition(F f, F g) {  // f(g(x))
    return { mul(f.c,g.c),add(mul(f.c, g.d), f.d) };
}
F id() {
    return { 1,0 };
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<S> a(n);
    for (auto&& i : a) {
        cin >> i.sum;
        i.size = 1;
    }
    segTree<S, op, e, F, mapping, composition, id> st(a);
    while (q--) {
        int type, l, r, c, d;
        cin >> type >> l >> r;
        l++;
        if (type == 0) {
            cin >> c >> d;
            st.range_modify(1, 1, n, l, r, { c,d });
        } else {
            auto&& x = st.range_query(1, 1, n, l, r);
            cout << x.sum << "\n";
        }
    }
    return 0;
}