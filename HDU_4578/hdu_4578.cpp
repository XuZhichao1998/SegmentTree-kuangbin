#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm> //std::max std::min

const int MAXN = 1E5+3;
const int MOD = 10007;

class SegmentTree {
public:
    static const int ROOT_INDEX;
    void init(int max_length);
    void build(int left, int right, int root);
    void update(int left, int right, int root, int update_left, int update_right, int update_type, int op_value);
    int query(int left, int right, int root, int query_left, int query_right, int query_type);
private:
    std::vector<int> _sum; // 区间和
    std::vector<int> _sum2; // 区间平方和
    std::vector<int> _sum3; // 区间立方和
    std::vector<int> _lazy_add; //加的懒标记
    std::vector<int> _lazy_multiply; // 乘的懒标记
    std::vector<int> _lazy_change; // 赋值的懒标记
    void change_node(int root, int left, int right, int value);
    void add_node(int root, int left, int right, int value);
    void multiply_node(int root, int left, int right, int value);
    void push_up(int root);
    void push_down(int root, int left, int right);

};
const int SegmentTree::ROOT_INDEX = 1;

void SegmentTree::init(int max_length) {
    max_length = std::max(max_length, 0);
    max_length = max_length << 2 | 1;
    _sum.resize(max_length);
    _sum2.resize(max_length);
    _sum3.resize(max_length);
    _lazy_add.resize(max_length);
    _lazy_multiply.resize(max_length);
    _lazy_change.resize(max_length);
}

void SegmentTree::build(int left, int right, int root) {
    _sum[root] = _sum2[root] = _sum3[root] = 0;
    _lazy_add[root] = _lazy_change[root] = 0;
    _lazy_multiply[root] = 1; 
    if (left != right) {
        int mid = (left + right) >> 1;
        build(left, mid, root<<1);
        build(mid+1, right, root<<1|1);
    }
}

void SegmentTree::change_node(int root, int left, int right, int value) {
    int cur_length = (right - left + 1);
    int value2 = value * value % MOD;
    int value3 = value * value2 % MOD;
    _sum[root] = value * cur_length % MOD;
    _sum2[root] = value2 * cur_length % MOD;
    _sum3[root] = value3 * cur_length % MOD;
    _lazy_add[root] = 0;
    _lazy_multiply[root] = 1;
    _lazy_change[root] = value;
}

void SegmentTree::multiply_node(int root, int left, int right, int value) {
    int cur_length = (right - left + 1);
    int value2 = value * value % MOD;
    int value3 = value * value2 % MOD;   
    _sum[root] = _sum[root] * value % MOD;
    _sum2[root] = _sum2[root] * value2 % MOD;
    _sum3[root] = _sum3[root] * value3 % MOD;
    _lazy_add[root] = (_lazy_add[root] * value) % MOD;
    _lazy_multiply[root] = _lazy_multiply[root] * value % MOD;
}

void SegmentTree::add_node(int root, int left, int right, int value) {
    int cur_length = right - left + 1;
    int value2 = value * value % MOD;
    int value3 = value * value2 % MOD;
    // a1^3 + a2^3 + a3^3 + ... => (a1+x)^3 + (a2+x)^3 + (a3+x)^3 + ...  = a^3 + 3 * a^2 * x + 3 * a * x^2 + x^3
    // dif = 3 * sum2[root] * value  +  3 * sum[root] * value2 + value3 * curlength
    _sum3[root] = (_sum3[root] + 3 * _sum2[root] * value % MOD + 3 * _sum[root] * value2 % MOD + value3 * cur_length % MOD) % MOD;
    // a1^2 + a2^2 + a3^2 + ... => (a1+x)^2 + (a2+x)^2 + (a3+x)^2 + ... = a^2 + 2 * a * x + x^2
    // dif = 2 * sum[root] * value + value2 * cur_length
    _sum2[root] = (_sum2[root] + 2 * _sum[root] * value % MOD + value2 * cur_length % MOD) % MOD;   
    _sum[root] = (_sum[root] + (value * cur_length % MOD)) % MOD;
    _lazy_add[root] = (_lazy_add[root] + value) % MOD;
}

//调用之前应该令op_vale %= MOD
void SegmentTree::update(int left, int right, int root, int update_left, int update_right, int update_type, int op_value) {
    if (update_left > right || update_right < left) {
        return;
    } else if (update_left <= left && right <= update_right) {
        if (update_type == 3) { // 区间赋值操作
            change_node(root, left, right, op_value);
        } else if (update_type == 2) { // 区间元素都去乘以一个数
            multiply_node(root, left, right, op_value);
        } else if (update_type == 1) { // 区间元素都加上一个数
            add_node(root, left, right, op_value);
        }
    } else {
        push_down(root, left, right);
        int mid = (left + right) >> 1;
        update(left, mid, root<<1, update_left, update_right, update_type, op_value);
        update(mid+1, right, root<<1|1, update_left, update_right, update_type, op_value);
        push_up(root);
    }
} 

int SegmentTree::query(int left, int right, int root, int query_left, int query_right, int query_type) {
    if (query_left > right || query_right < left) {
        return 0;
    } else if (query_left <= left && right <= query_right) {
        if (query_type == 1) {
            return _sum[root];
        } else if (query_type == 2) {
            return _sum2[root];
        } else {
            return _sum3[root];
        }
    } else {
        push_down(root, left, right);
        int mid = (left + right) >> 1;
        int sum_lson = query(left, mid, root<<1, query_left, query_right, query_type);
        int sum_rson = query(mid+1, right, root<<1|1, query_left, query_right, query_type);
        return (sum_lson + sum_rson) % MOD;
    }
}

void SegmentTree::push_up(int root) {
    int lson = root<<1;
    int rson = root<<1|1;
    _sum[root] = (_sum[lson] + _sum[rson] ) % MOD;
    _sum2[root] = (_sum2[lson] + _sum2[rson]) % MOD;
    _sum3[root] = (_sum3[lson] + _sum3[rson]) % MOD;
}

void SegmentTree::push_down(int root, int left, int right) {
    int mid = (left + right) >> 1;
    int lson = root<<1;
    int rson = root<<1|1;
    int cur_length = right - left + 1;
    int lson_length = mid - left + 1;
    int rson_length = right - mid;

    if (_lazy_change[root] != 0) { // 说明有赋值操作需要push_down
        change_node(lson, left, mid, _lazy_change[root]);
        change_node(rson, mid+1, right, _lazy_change[root]);
        _lazy_change[root] = 0;
    }

    if (_lazy_multiply[root] != 1) {
        multiply_node(lson, left, mid, _lazy_multiply[root]);
        multiply_node(rson, mid+1, right, _lazy_multiply[root]);
        _lazy_multiply[root] = 1;
    }

    if (_lazy_add[root] != 0) {
        add_node(lson, left, mid, _lazy_add[root]);
        add_node(rson, mid+1, right, _lazy_add[root]);
        _lazy_add[root] = 0;
    }
}

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    
    SegmentTree * ptree = new SegmentTree();
    ptree->init(MAXN);

    int n, m, op, left, right, x;
    while (scanf("%d%d", &n, &m) != EOF && (m || n)) {
        ptree->build(1, n, SegmentTree::ROOT_INDEX);
        while (m--) {
            scanf("%d%d%d%d", &op, & left, & right, & x);
            switch (op) {
            case 1: case 2: case 3:
                x %= MOD;
                ptree->update(1, n, SegmentTree::ROOT_INDEX, left, right, op, x);
                break;
            case 4:
                printf("%d\n", ptree->query(1, n, SegmentTree::ROOT_INDEX, left, right, x));
                break;
            }
        }
    }

    delete ptree;

    if (argc == 2) {
        fclose(stdin);
    }
    
    return 0;
}
