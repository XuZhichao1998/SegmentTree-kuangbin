#include <cstdio>
#include <algorithm> //std::ma
const int MAX_INTERVAL = 100005;

class SegmentTreeSum {
public:
    static const int ROOT_INDEX;
    void build(int left, int right, int root);
    int update(int left, int right, int root, int update_left, int update_right, int value);
    int query(int left, int right, int root, int query_left, int query_right);
    SegmentTreeSum(int max_length);
    ~SegmentTreeSum();
    int get_tot_sum() {
        return _sum[SegmentTreeSum::ROOT_INDEX];
    }
private:
    int * _sum;
    int * _lazy;
    void push_down(int root, int left, int right);
};

const int SegmentTreeSum::ROOT_INDEX = 1;

SegmentTreeSum::SegmentTreeSum(int max_length) {
    max_length = std::max(max_length,0);
    _sum = new int[max_length+1];
    _lazy = new int[max_length+1];
}

SegmentTreeSum::~SegmentTreeSum() {
    delete [] _sum;
    delete [] _lazy;
}

void SegmentTreeSum::build(int left, int right, int root) {
    _sum[root] = (right - left + 1) * 1; //初始为1
    _lazy[root] = 0;
    int mid = (left + right) >> 1;
    if (left != right) {    
        build(left, mid, root<<1);
        build(mid+1, right, root<<1|1);
    }
}

int SegmentTreeSum::update(int left, int right, int root, int update_left, int update_right, int value) {
    if (update_left > right || update_right < left) {
        return _sum[root];
    } else if (update_left <= left && right <= update_right) {
        _lazy[root] = value;
        return _sum[root] = (right - left + 1) * value;
    } else {
        push_down(root, left, right);
        int mid = (left + right) >> 1;
        int sum_lson = update(left, mid, root<<1, update_left, update_right, value);
        int sum_rson = update(mid+1, right, root<<1|1, update_left, update_right, value);
        return _sum[root] = sum_lson + sum_rson;
    }
}

int SegmentTreeSum::query(int left, int right, int root, int query_left, int query_right) {
    if (query_left > right || query_right < left) {
        return 0;
    } else if (query_left <= left && right <= query_right) {
        return _sum[root];
    } else {
        push_down(root, left, right);
        int mid = (left + right) >> 1;
        int sum_lson = query(left, mid, root<<1, query_left, query_right);
        int sum_rson = query(mid+1, right, root<<1|1, query_left, query_right);
        return sum_lson + sum_rson;
    }
}

void SegmentTreeSum::push_down(int root, int left, int right) {
    if (_lazy[root] != 0) {
        int mid = (left + right) >> 1;
        _sum[root<<1] = _lazy[root] * (mid - left + 1);
        _sum[root<<1|1] = _lazy[root] * (right - mid);
        _lazy[root<<1] = _lazy[root<<1|1] = _lazy[root];
        _lazy[root] = 0;
    }
}

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1],"r",stdin);
    }
    int T, n, m, left, right, num;
    SegmentTreeSum * ptree = new SegmentTreeSum(MAX_INTERVAL*4); 
    scanf("%d", &T);
    for (int ca = 1; ca <= T; ++ca) {
        scanf("%d%d", &n, &m);
        ptree->build(1, n, SegmentTreeSum::ROOT_INDEX);
        while (m--) {
            scanf("%d%d%d", &left, &right, &num);
            ptree->update(1, n, SegmentTreeSum::ROOT_INDEX, left, right, num);
        }
        printf("Case %d: The total value of the hook is %d.\n", ca, ptree->get_tot_sum());
    }

    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}
