#include <iostream>
#include <vector>
#include <algorithm>

const int MAXN = 5E4+10;

class SegmentTree {
public:
    static const int ROOT_INDEX; 
    void build(int left, int right, int root);
    void init(int max_length);
    int query(int left, int right, int root, int query_left, int query_right);
    void update(int left, int right, int root, const int start, int& flower_count, int& left_pos, int& right_pos);
private:
    std::vector<int> _sum;
    std::vector<int> _lazy;
    void push_up(int root);
    void push_down(int root, int left, int right);
};

const int SegmentTree::ROOT_INDEX = 1;

void SegmentTree::init(int max_length) {
    max_length = std::max(max_length,0);
    max_length = max_length << 2 | 1;
    _sum.resize(max_length);
    _lazy.resize(max_length);
}

void SegmentTree::build(int left, int right, int root) {
    _sum[root] = 0;
    _lazy[root] = -1;
    if (left == right) {
        return;
    }
    int mid = (left + right) >> 1;
    build(left, mid, root<<1);
    build(mid+1, right, root<<1|1);
    push_up(root);
}

int SegmentTree::query(int left, int right, int root, int query_left, int query_right) {
    if (query_left > right || query_right < left) {
        return 0;
    } else if (query_left <= left && right <= query_right) {
        int ans = _sum[root];
        _sum[root] = 0;
        _lazy[root] = 0 ;
        return ans;
    } else {
        push_down(root, left, right);
        int mid = (left + right) >> 1;
        int sum_lson = query(left, mid, root<<1, query_left, query_right);
        int sum_rson = query(mid+1, right, root<<1|1, query_left, query_right);
        push_up(root); // 因为query有更新操作
        return sum_lson + sum_rson;
    }
}

void SegmentTree::update(int left, int right, int root, const int start, int& flower_count, int& left_pos, int& right_pos) {
    if (right < start || flower_count == 0) {
        return;
    }
    int mid = (left + right) >> 1;
    if (left >= start) {
        int len = right - left + 1;
        int remain = len - _sum[root];
        if (remain == 0) { // 该区间已经被放满了
            return;
        } else if (_sum[root] == 0) { //该区间完全为空
            if (remain <= flower_count) {
                _sum[root] = len;
                _lazy[root] = 1;
                flower_count -= len;
                if (left_pos == -1) {
                    left_pos = left;
                }
                if (right_pos < right) {
                    right_pos = right;
                }
                return;
            } else {
                push_down(root, left, right);
                update(left, mid, root<<1, start, flower_count, left_pos, right_pos);
                update(mid+1, right, root<<1|1, start, flower_count, left_pos, right_pos);
                push_up(root);
                return;
            }
        } else { //该区间有插花的空间
            push_down(root, left, right);
            update(left, mid, root<<1, start, flower_count, left_pos, right_pos);
            update(mid+1, right, root<<1|1,start, flower_count, left_pos, right_pos);
            push_up(root);
        }
    } else { //left < start && right >= start
        push_down(root, left, right);
        update(left, mid, root<<1, start, flower_count, left_pos, right_pos);
        update(mid+1, right, root<<1|1, start, flower_count, left_pos, right_pos);
        push_up(root);
        return;
    }
}

void SegmentTree::push_up(int root) {
    _sum[root] = _sum[root<<1] + _sum[root<<1|1];
}

void SegmentTree::push_down(int root, int left, int right) {
    if (_lazy[root] != -1) {
        int mid = (left + right) >> 1;
        int lson = root << 1;
        int rson = root << 1 | 1;
        _sum[lson] = _lazy[root] * (mid - left + 1);
        _sum[rson] = _lazy[root] * (right - mid);
        _lazy[lson] = _lazy[rson] = _lazy[root];
        _lazy[root] = -1;
    }
}

int main(int argc, const char  * argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }

    SegmentTree * ptree = new SegmentTree();
    ptree->init(MAXN);
    
    int T, n, m;
    std::cin>>T;
    while (T--) {
        std::cin>>n>>m;
        ptree->build(1, n, SegmentTree::ROOT_INDEX);
        while (m--) {
            int op, left, right, start_pos, flower_count;
            std::cin>>op;
            switch (op) {
            case 1:
                std::cin>>start_pos>>flower_count;
                ++start_pos;
                left = right = -1;
                ptree->update(1, n, SegmentTree::ROOT_INDEX, start_pos, flower_count, left, right);
                if (left != -1) {
                    std::cout<<left-1<<" "<<right-1<<"\n";
                } else {
                    std::cout<<"Can not put any one.\n";
                }
                break;
            case 2:
                std::cin>>left>>right;
                std::cout<<ptree->query(1, n, SegmentTree::ROOT_INDEX, left+1, right+1)<<"\n";
                break;
            }
        }
        std::cout<<"\n";
    }

    delete ptree;

    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}
