#include <cstdio>
#include <algorithm>
#include <vector>
#include <cmath>

typedef long long LL;
const int MAXN = 100003;

class SegmentTree {
public:
    static const int ROOT_INDEX;
    void build(int left, int right, int root);
    void update(int left, int right, int root, int update_left, int update_right);
    LL query(int left, int right, int root, int query_left, int query_right); 

private:
    LL _sum[MAXN<<2];  
    bool _all_one[MAXN<<2];
    void push_up(int root);
    void push_down(int root, int left, int right);
}tree;

const int SegmentTree::ROOT_INDEX = 1;

void SegmentTree::build(int left, int right, int root) {
    if (left == right) {
        scanf("%lld",&_sum[root]);
        _all_one[root] = (_sum[root] == 1);
        return;
    }
    int mid = (left + right) >> 1;
    build(left, mid, root<<1);
    build(mid+1, right, root<<1|1);
    push_up(root);
}

void SegmentTree::update(int left, int right, int root, int update_left, int update_right) {
    int mid = (left + right) >> 1;
    if (left == update_left && right == update_right) {
        if (_all_one[root]) { //都是1，不用更新
            return;
        }
        if (left == right) {
            _sum[root] = (LL)sqrt(1.0 * _sum[root]);
            _all_one[root] = (_sum[root] == 1);
            return;
        } 
        update(left, mid, root<<1, left, mid);
        update(mid+1, right, root<<1|1, mid+1, right);
        push_up(root);
        return; 
     } else {
        if (update_right <= mid) {
            update(left, mid, root<<1, update_left, update_right);
        } else if (update_left >= mid+1) {
            update(mid+1, right, root<<1|1, update_left, update_right);
        } else {
            update(left, mid, root<<1, update_left, mid);
            update(mid+1, right, root<<1|1, mid+1, update_right);
        }
        push_up(root);
     }

}

void SegmentTree::push_up(int root) { 
    _sum[root] = _sum[root<<1] + _sum[root<<1|1];
    _all_one[root] = _all_one[root<<1] && _all_one[root<<1|1];
}

LL SegmentTree::query(int left, int right,int root, int query_left, int query_right) {
    if (query_left > right || query_right < left) {
        return 0ll;
    } else if (query_left <= left && right <= query_right) {
        return _sum[root];
    } else {
        int mid = (left + right) >> 1;
        LL sum_lson = query(left, mid, root<<1, query_left, query_right);
        LL sum_rson = query(mid+1, right, root<<1|1, query_left, query_right);
        return sum_lson + sum_rson;
    }
}

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    
    int n, m, op, left, right, ca = 0;
    while (scanf("%d", &n) != EOF) {
        tree.build(1, n, 1);
        scanf("%d", &m);
        printf("Case #%d:\n",++ca);
        while (m--) {
            scanf("%d%d%d", &op, &left, &right);
            if (left > right) {
                std::swap(left, right);
            }
            if (op == 1) {
                printf("%lld\n",tree.query(1, n, 1, left, right));
            } else if (op == 0) {
                tree.update(1, n, 1, left, right);
            }       
        }
        printf("\n");
    }

    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}
