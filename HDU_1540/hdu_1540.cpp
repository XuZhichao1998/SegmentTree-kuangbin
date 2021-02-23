#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>

const int MAXN = 5E4+3; 

class SegmentTree {
public:
    static const int ROOT_INDEX;
    std::vector<int> _tunnels;
    void init(int max_length);
    void build(int left, int right, int root);
    void modify(int left, int right, int root, int index,int option_type);
    int query(int left, int right, int root, int index);

private:
    std::vector<int> _cnt_left; // 区间从左边起连续的长度
    std::vector<int> _cnt_right; // 区间从右边起连续的长度
    void push_up(int root, int left, int right);
};

const int SegmentTree::ROOT_INDEX = 1;

void SegmentTree::init(int max_length) {
    max_length = std::max(max_length, 0);
    _tunnels.resize(max_length+1);
    int max_node_count = max_length<<2|1;
    _cnt_left.resize(max_node_count);
    _cnt_right.resize(max_node_count);
}

void SegmentTree::build(int left, int right, int root) {
    _cnt_left[root] = _cnt_right[root] = right - left + 1;
    if (left == right) {
        _tunnels[left] = 1; // 一开始地道是完好的
        return;
    }
    int mid = (left + right) >> 1;
    build(left, mid, root<<1);
    build(mid+1, right, root<<1|1);
}

void SegmentTree::modify(int left, int right, int root, int index, int option_type) {
    if (left == right) {
        _tunnels[left] = _cnt_left[root] = _cnt_right[root] =  option_type; 
        return;
    }
    int mid = (left + right) >> 1;
    if (index <= mid) {
        modify(left, mid, root<<1, index, option_type);
    } else {
        modify(mid+1, right, root<<1|1, index, option_type);
    }
    push_up(root,left, right);
}

void SegmentTree::push_up(int root, int left, int right) {
    int mid = (left + right) >> 1;
    int lson = root<<1;
    int rson = root<<1|1;
    int length = right - left + 1;
    int lson_length = mid - left + 1;
    int rson_length = right - mid;
    _cnt_left[root] = (_cnt_left[lson] == lson_length) ? lson_length + _cnt_left[rson] : _cnt_left[lson];
    _cnt_right[root] = (_cnt_right[rson] == rson_length) ? rson_length + _cnt_right[lson] : _cnt_right[rson];
}

int SegmentTree::query(int left, int right, int root, int index) {
    int left_rightest_pos = left + _cnt_left[root] - 1;
    int right_leftest_pos = right - _cnt_right[root] + 1;
    if (index <= left_rightest_pos) {
        return _cnt_left[root];
    } else if (index >= right_leftest_pos) {
        return _cnt_right[root];
    } else {
        int mid = (left + right) >> 1;
        int lson = root<<1;
        int rson = root<<1|1;
        if (index < mid - _cnt_right[lson] + 1) {
            return query(left, mid, lson, index);
        } else if (index > mid + _cnt_left[rson]) {
            return query(mid+1, right, rson, index);
        } else {
            return query(left, mid, lson, index) + query(mid+1, right, rson, index);
        }
    }
}

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    SegmentTree * ptree = new SegmentTree();
    ptree->init(MAXN);
    int n, m, index;
    char op[3];
    while(scanf("%d%d", &n, &m) != EOF) {
        std::stack<int> st;
        ptree -> build(1, n, SegmentTree::ROOT_INDEX);
        while (m--) {
            scanf("%s", op);
            switch (op[0]) {
            case 'D':
                scanf("%d", &index);
                ptree->modify(1, n, SegmentTree::ROOT_INDEX, index, 0);
                st.push(index);
                break;
            case 'Q':
                scanf("%d", &index);
                if (ptree->_tunnels[index] == 0) {
                    printf("0\n");
                } else {
                    printf("%d\n", ptree->query(1, n, SegmentTree::ROOT_INDEX, index));
                }
                break;
            case 'R': 
                index = st.top();
                st.pop();
                ptree->modify(1, n, SegmentTree::ROOT_INDEX, index, 1);
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
