#include <cstdio>
#include <algorithm>
#include <vector>

const int MAXN = 1005;

struct Line {
    double x1, x2, y;
    int is_down; // is_down = IsDown ? 1 : -1
    Line() {}
    Line(double _x1, double _x2, double _y, int _is_down) : x1(_x1), x2(_x2), y(_y), is_down(_is_down) {}
    bool operator < (const Line& rhs) const {
        return y == rhs.y ? is_down > rhs.is_down : y < rhs.y;
    }
};

class SegmentTree {
public:
    static const int ROOT_INDEX;
    void init(int max_size);
    void build(int left, int right, int root);
    void update(int left, int right, int root, int update_left, int update_right, int value, const double * vX);
    double get_covered_length() {return _covered_twice_length[SegmentTree::ROOT_INDEX];}
private:
    std::vector<int> _sum; //区间被几条入边覆盖
    std::vector<double> _covered_length; // 区间被覆盖的长度
    std::vector<double> _covered_twice_length; //区间被覆盖>=2的长度
};

const int SegmentTree::ROOT_INDEX = 1;

void SegmentTree::init(int max_size) {
    max_size = std::max(0, max_size);
    max_size = max_size << 2 | 1;
    _sum.resize(max_size);
    _covered_length.resize(max_size);
    _covered_twice_length.resize(max_size);
}

void SegmentTree::build(int left, int right, int root) {
    _sum[root] = 0;
    _covered_length[root] = 0.0;
    _covered_twice_length[root] = 0.0;
    int mid = (left + right) >> 1;
    if (left < right - 1) {
        build(left, mid, root<<1);
        build(mid, right, root<<1|1);
    }
}

void SegmentTree::update(int left, int right, int root, int update_left, int update_right, int value, const double * vX) {
    if (update_left >= right || update_right <= left) {
        return;
    } else if (update_left <= left && right <= update_right) {
        _sum[root] += value;
        if (_sum[root] >= 1) {
            _covered_length[root] = vX[right] - vX[left];
        } else if (left == right - 1) {
            _covered_length[root] = 0.0;
        } else {
            _covered_length[root] = _covered_length[root<<1] + _covered_length[root<<1|1];
        }

        if (_sum[root] >= 2) { // 区间全部被覆盖两次
            _covered_twice_length[root] = vX[right] - vX[left];
        } else if (left == right - 1) { //区间没被覆盖两次，并且是叶子节点 
            _covered_twice_length[root] = 0.0;
        } else if (_sum[root] == 1){
            _covered_twice_length[root] = _covered_length[root<<1] + _covered_length[root<<1|1];
        } else {
            _covered_twice_length[root]  =_covered_twice_length[root<<1] + _covered_twice_length[root<<1|1];
        }
        return;
    } else {
        int mid = (left + right) >> 1;
        update(left, mid, root<<1, update_left, update_right, value, vX);
        update(mid, right, root<<1|1, update_left, update_right, value, vX);
        if (_sum[root] == 0) {
            _covered_length[root] = _covered_length[root<<1] + _covered_length[root<<1|1];
            _covered_twice_length[root] = _covered_twice_length[root<<1] + _covered_twice_length[root<<1|1]; 
        } 
        if (_sum[root] == 1) {
            _covered_twice_length[root] = _covered_length[root<<1] + _covered_length[root<<1|1];
        }
    }
}

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    
    static Line vLine[MAXN * 2];
    static double vX[MAXN*2];
    int T, n;
    double x1, y1, x2, y2;

    SegmentTree * ptree = new SegmentTree();
    ptree->init(MAXN * 2);

    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
            vLine[i] = Line(x1, x2, y1, 1);
            vLine[i+n] = Line(x1, x2, y2, -1);
            vX[i] = x1;
            vX[i+n] = x2;
        }
        int tot = n * 2;
        std::sort(vLine, vLine + tot); //把水平的横向边（扫描线）按照y坐标从小到大排序，y一样的，入边在下
        std::sort(vX, vX + tot);
        int len = std::unique(vX, vX + tot) - vX;
        //离散化后所有点横坐标的范围是[0, len-1]
        ptree->build(0, len-1, SegmentTree::ROOT_INDEX);
        double area = 0.0;
        for (int i = 0; i < tot; ++i) {
            int xleft = std::lower_bound(vX, vX + len, vLine[i].x1) - vX;
            int xright = std::lower_bound(vX, vX + len, vLine[i].x2) - vX;
            ptree->update(0, len-1, SegmentTree::ROOT_INDEX, xleft, xright, vLine[i].is_down, vX);
            area += (vLine[i+1].y - vLine[i].y) * ptree->get_covered_length();
        }
        printf("%.2lf\n", area);
    }

    delete ptree;

    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}
