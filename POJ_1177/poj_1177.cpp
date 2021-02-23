#include <iostream>
#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>
#include <cmath>
#include <deque>
const int MAXN = 5000+10;
struct Line {
    int x1;
    int x2;
    int y;
    int is_down; // isdown? 1 : -1
    bool operator < (const Line& rhs) const {
        return y == rhs.y ? is_down > rhs.is_down : y < rhs.y;
    }
    Line() {}
    Line(int _x1, int _x2, int _y, int _is_down) : x1(_x1), x2(_x2), y(_y), is_down(_is_down) {}
};

struct Rectangle {
    int x1, y1, x2, y2;
    Rectangle() {}
    Rectangle(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
    void input() {
        std::cin>>x1>>y1>>x2>>y2;
    }
};

class SegmentTree { //线段树的left和right记录的是区间的端点而不是最左最右的条
public:
    static const int ROOT_INDEX;
    void init(int max_size);
    void build(int left, int right, int root);
    void update(int left, int right, int root, int update_left, int update_right, int value, const int * vX);
    int get_covered_length() {return _covered_length[SegmentTree::ROOT_INDEX];}
    int get_line_count() {return _line_count[SegmentTree::ROOT_INDEX];}
private:
    std::vector<int> _sum; // 该区间被几条横向线段覆盖
    std::vector<int> _covered_length; // 该区间被覆盖的实际长度
    std::vector<int> _line_count; // 该区间有多少不连续的线段
    std::vector<int> _left_point; // 区间左端点是否被覆盖
    std::vector<int> _right_point; // 区间右端点是否被覆盖
    void push_up(int root);
};

const int SegmentTree::ROOT_INDEX = 1;
void SegmentTree::init(int max_size) {
    max_size = std::max(0, max_size);
    max_size = max_size << 2 | 1;
    _sum.resize(max_size);
    _line_count.resize(max_size);
    _left_point.resize(max_size);
    _right_point.resize(max_size);
    _covered_length.resize(max_size);
}

void SegmentTree::build(int left, int right, int root) {
    _sum[root] = _line_count[root] = _covered_length[root] = 0;
    _left_point[root] = _right_point[root] = 0;
    if (left < right-1) {
       int mid = (left + right) >> 1;
       build(left, mid, root<<1);
       build(mid, right, root<<1|1);
    }
}

void SegmentTree::push_up(int root) {
    int lson = root << 1;
    int rson = root << 1 | 1;
    _covered_length[root] = _covered_length[lson] + _covered_length[rson];
     _left_point[root] = _left_point[lson];
     _right_point[root] = _right_point[rson];
     _line_count[root] = _line_count[lson] + _line_count[rson] - (_right_point[lson] & _left_point[rson]);
}    

void SegmentTree::update(int left, int right, int root, int update_left, int update_right, int value, const int * vX) {
    if (update_left >= right || update_right <= left) {
        return;
    } else if (update_left <= left && right <= update_right) {
        _sum[root] += value;
        if (_sum[root] > 0) { // 有矩形的下边覆盖
            _left_point[root] = _right_point[root] = 1;
            _line_count[root] = 1;
            _covered_length[root] = vX[right] - vX[left];
            return;
        } else if (left == right - 1){ // 这是叶子节点，没有子树，下面也没有其他更短的下边
            _left_point[root] = _right_point[root] = 0;
            _line_count[root] = 0;
            _covered_length[root] = 0;
            return;           
        } else { // 我这一段变成0，但是下面还可能有更短的下边
            push_up(root);
        }
    } else {
        int mid = (left + right) >> 1;
        update(left, mid, root<<1, update_left, update_right, value, vX);
        update(mid, right, root<<1|1, update_left, update_right, value, vX);
        //push_up
        //能运行到这里，root一定不是叶子结点。
        if (_sum[root] == 0) { // 如果当前root表示的大区间没有下边覆盖，则用左右子树来更新，否则区间被完全覆盖，无须更新
            push_up(root);
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
    
    int n, x1, y1, x2, y2;
    static Rectangle vRec[MAXN];
    static Line vLine[MAXN * 2];
    static int vX[MAXN * 2];

    while (std::cin>>n) { 
        for (int i = 0; i < n; ++i) {
            vRec[i].input();
            vX[i] = vRec[i].x1;
            vX[i+n] = vRec[i].x2;
            vLine[i] = Line(vRec[i].x1, vRec[i].x2, vRec[i].y1, 1);
            vLine[i+n] = Line(vRec[i].x1, vRec[i].x2, vRec[i].y2, -1);
        }
        int tot = n << 1;
        std::sort(vLine, vLine + tot); //对2*n条横线按照y坐标从小到大排序
        std::sort(vX, vX + tot);
        int len = std::unique(vX, vX + tot) - vX;
        ptree->build(0, len-1, SegmentTree::ROOT_INDEX);
        int ans = 0;
        int last_length = 0;
        for (int i = 0; i < tot; ++i) {
            //xleft和xright为Line[i]这条横向线段离散化之后的x坐标
            int xleft = std::lower_bound(vX, vX + len, vLine[i].x1) - vX;
            int xright = std::lower_bound(vX, vX + len, vLine[i].x2) - vX;
            ptree->update(0, len-1, SegmentTree::ROOT_INDEX, xleft, xright, vLine[i].is_down, vX);
            ans += abs(ptree->get_covered_length() - last_length);
            if (i < tot-1) {
                ans += (vLine[i+1].y - vLine[i].y) * ptree->get_line_count() * 2;
            }
            last_length = ptree->get_covered_length();
         }
         std::cout<<ans<<"\n";
    }

    delete ptree;

    if (argc == 2) {
        fclose(stdin);
    } 
    return 0;
}
