#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

const int MAXN = 1E5+10;

class SegmentTree {
public:
    static const int ROOT_INDEX;
    void build(int left, int right, int root);
    void init(int max_size);
    void update(int left, int right,int root, int update_left, int update_right, char who);
    int query(int left, int right, int root, int query_length);
    int query_ignore_ds(int left, int right, int root, int query_length);
    int get_max_length() {return _max_length[SegmentTree::ROOT_INDEX];}
    int get_max_length1() {return _max_length1[SegmentTree::ROOT_INDEX];}
private:
    std::vector<int> _max_length; // 区间最大连续可用长度, 也起了lazy的作用
    std::vector<int> _left_length; // 从区间左端点开始最大连续可用长度
    std::vector<int> _right_length; // 从去家右端点开始最大连续可用长度
    std::vector<int> _max_length1; // 无视DS的区间最大连续可用长度，也起了lazy的作用
    std::vector<int> _left_length1; // 无视DS的.. 左
    std::vector<int> _right_length1; // 无视DS的..右
    void push_up(int root, int left, int right);
    void push_down(int root, int left, int right);
};

const int SegmentTree::ROOT_INDEX = 1;

void SegmentTree::init(int max_size) {
    max_size = std::max(0, max_size);
    max_size = max_size << 2 | 1;
    _max_length.resize(max_size);
    _left_length.resize(max_size);
    _right_length.resize(max_size);
    _max_length1.resize(max_size);
    _left_length1.resize(max_size);
    _right_length1.resize(max_size);
}

void SegmentTree::build(int left, int right, int root) {
    _max_length[root] = _left_length[root] = _right_length[root] = right - left + 1;
    _max_length1[root] = _left_length1[root] = _right_length1[root] = right - left + 1;
    int mid = (left + right) >> 1;
    if (left != right) {
        build(left, mid, root<<1);
        build(mid+1, right, root<<1|1);
    }
}

void SegmentTree::push_up(int root, int left, int right) {
    int mid = (left + right) >> 1;
    int lson = root << 1;
    int rson = root << 1 | 1;
    int lson_length = mid - left + 1;
    int rson_length = right - mid;
    // 维护空闲区间
    _left_length[root] = _left_length[lson];
    _right_length[root] = _right_length[rson];
    if (_left_length[lson] == lson_length) {
        _left_length[root] += _left_length[rson];
    }
    if (_right_length[rson] == rson_length) {
        _right_length[root] += _right_length[lson];
    }
    _max_length[root] = std::max(_left_length[root], _right_length[root]);
    _max_length[root] = std::max(_max_length[root], _right_length[lson] + _left_length[rson]);
    _max_length[root] = std::max(_max_length[root], std::max(_max_length[lson], _max_length[rson]));     
    // 维护无视DS的空闲区间
    _left_length1[root] = _left_length1[lson]; 
    _right_length1[root] = _right_length1[rson];
    if (_left_length1[lson] == lson_length) {
        _left_length1[root] += _left_length1[rson];
    }
    if (_right_length1[rson] == rson_length) {
        _right_length1[root] += _right_length1[lson];
    }
    _max_length1[root] = std::max(_left_length1[root], _right_length1[root]);
    _max_length1[root] = std::max(_max_length1[root], _right_length1[lson] + _left_length1[rson]);
    _max_length1[root] = std::max(_max_length1[root], std::max(_max_length1[lson], _max_length1[rson]));
}

void SegmentTree::push_down(int root, int left, int right) {
    int mid = (left + right) >> 1;
    int lson = root << 1;
    int rson = root << 1 | 1;
    int root_length = right - left + 1;
    int lson_length = mid - left + 1;
    int rson_length = right - mid;
    
    if (_max_length[root] == 0) {
        _max_length[lson] = _left_length[lson] = _right_length[lson] = 0;
        _max_length[rson] = _left_length[rson] = _right_length[rson] = 0;
    } else if (_max_length[root] == root_length) { 
        _max_length[lson] = _left_length[lson] = _right_length[lson] = lson_length;
        _max_length[rson] = _left_length[rson] = _right_length[rson] = rson_length;
    }

    //无视DS的空闲区间
    if (_max_length1[root] == 0) {
        _max_length1[lson] = _left_length1[lson] = _right_length1[lson] = 0;
        _max_length1[rson] = _left_length1[rson] = _right_length1[rson] = 0;
    } else if (_max_length1[root] == root_length) { 
        _max_length1[lson] = _left_length1[lson] = _right_length1[lson] = lson_length;
        _max_length1[rson] = _left_length1[rson] = _right_length1[rson] = rson_length;
    }
}

void SegmentTree::update(int left, int right, int root, int update_left, int update_right, char who) {
    if (update_left > right || update_right < left) {
        return;
    } else if (update_left <= left && right <= update_right) {
        switch (who) {
        case 'S': // Study，清空操作
            _max_length[root] = _left_length[root] = _right_length[root] = right - left + 1;
            _max_length1[root] = _left_length1[root] = _right_length1[root] = right - left + 1;
            return;
        case 'D': // DS占用这段区间
            _max_length[root] = _left_length[root] = _right_length[root] = 0;
            return;
        case 'N': // NS占用这段区间 
            _max_length[root] = _left_length[root] = _right_length[root] = 0;
            _max_length1[root] = _left_length1[root] = _right_length1[root] = 0;
            return;
        }
    } else {
        int mid = (left + right) >> 1;
        push_down(root, left, right);
        update(left, mid, root<<1, update_left, update_right, who);
        update(mid+1, right, root<<1|1, update_left, update_right, who);
        push_up(root, left, right);
    }
}

int SegmentTree::query(int left, int right, int root, int query_length) {
    if (_max_length[root] < query_length) { // 没答案
        return 0;
    } else if (_left_length[root] >= query_length) { // 有答案，并且在最左边
        return left;
    } else { // 有答案，不在最左，说明不是叶子节点
        int mid = (left + right) >> 1;
        int lson = root << 1;
        int rson = root << 1 | 1;
        if (_max_length[lson] >= query_length) { // 左子区间有足够大的空闲
            return query(left, mid, root<<1, query_length);
        } else if (_right_length[lson] + _left_length[rson] >= query_length) { // 空闲区间跨了左右子树
            return mid - _right_length[lson] + 1;
        } else {
            return query(mid+1, right, rson, query_length);
        }
    }
}

int SegmentTree::query_ignore_ds(int left, int right, int root, int query_length) {
    if (_max_length1[root] < query_length) { // 没答案
        return 0;
    } else if (_left_length1[root] >= query_length) { // 有答案，并且在最左边
        return left;
    } else { // 有答案，不在最左，说明不是叶子节点
        int mid = (left + right) >> 1;
        int lson = root << 1;
        int rson = root << 1 | 1;
        if (_max_length1[lson] >= query_length) { // 左子区间有足够大的空闲
            return query_ignore_ds(left, mid, root<<1, query_length);
        } else if (_right_length1[lson] + _left_length1[rson] >= query_length) { // 空闲区间跨了左右子树
            return mid - _right_length1[lson] + 1;
        } else {
            return query_ignore_ds(mid+1, right, rson, query_length);
        }
    }
}

int main(int argc, const char * argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    
    SegmentTree * ptree = new SegmentTree();
    ptree->init(MAXN);

    int CASE, ca, n, m, ask_time;
    std::string option;
    std::cin>>CASE;
    for (int ca = 1; ca <= CASE; ++ca) {
        std::cin>>n>>m;
        ptree->build(1, n, SegmentTree::ROOT_INDEX);
        std::cout<<"Case "<<ca<<":\n";
        while (m--) {
            std::cin>>option;
            switch (option[0]) {
            case 'D':
                std::cin>>ask_time;
                if (ptree->get_max_length() < ask_time) {
                    std::cout<<"fly with yourself\n";
                } else {
                    int start_time = ptree->query(1, n, SegmentTree::ROOT_INDEX, ask_time);
                    ptree->update(1, n, SegmentTree::ROOT_INDEX, start_time, start_time + ask_time - 1, 'D');
                    std::cout<<start_time<<",let's fly\n";
                }
                break;
            case 'N':
                std::cin>>ask_time;
                if (ptree->get_max_length() >= ask_time) {
                    int start_time = ptree->query(1, n, SegmentTree::ROOT_INDEX, ask_time);
                    ptree->update(1, n, SegmentTree::ROOT_INDEX, start_time, start_time + ask_time - 1, 'N');
                    std::cout<<start_time<<",don't put my gezi\n";
                } else if (ptree->get_max_length1() >= ask_time) {
                    int start_time = ptree->query_ignore_ds(1, n, SegmentTree::ROOT_INDEX, ask_time);
                    ptree->update(1, n, SegmentTree::ROOT_INDEX, start_time, start_time + ask_time -1, 'N');
                    std::cout<<start_time<<",don't put my gezi\n";
                } else {
                    std::cout<<"wait for me\n";
                }
                break;
            case 'S':
                int c_left, c_right;
                std::cin>>c_left>>c_right;
                ptree->update(1, n, SegmentTree::ROOT_INDEX, c_left, c_right, 'S');
                std::cout<<"I am the hope of chinese chengxuyuan!!\n";
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
