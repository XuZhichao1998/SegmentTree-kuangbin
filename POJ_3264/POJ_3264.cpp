#include <cstdio>
#include <algorithm>

const int MAX_INTERVAL = 50003;
class SegmentTreeMaxMin {
public:
    static const int ROOT_INDEX;
    static const int MAX_VALUE;
    static const int MIN_VALUE;

    SegmentTreeMaxMin(int max_length);
    ~SegmentTreeMaxMin();
    void _build(int left, int right, int root);
    std::pair<int,int> _query(int left, int right, int root, int query_left, int query_right);

private:
    int * _max;
    int * _min;
    void _push_up(int root);
};

const int SegmentTreeMaxMin::ROOT_INDEX = 1;
const int SegmentTreeMaxMin::MAX_VALUE = 1E6+10; //大于奶牛的最大身高1E6即可
const int SegmentTreeMaxMin::MIN_VALUE = 0; //小与奶牛的最小身高1即可

SegmentTreeMaxMin::SegmentTreeMaxMin(int max_length) {
    max_length = std::max(max_length, 0);
    _max = new int[max_length+1];
    _min = new int[max_length+1];
}

SegmentTreeMaxMin::~SegmentTreeMaxMin() {
    delete [] _max;
    delete [] _min;
}

void SegmentTreeMaxMin::_build(int left, int right, int root) {
    if (left == right) {
        scanf("%d",&_min[root]);
        _max[root] = _min[root];
        return; 
    }
    int mid = (left + right) >> 1;
    _build(left, mid, root<<1);
    _build(mid+1, right, root<<1|1);
    _push_up(root);
}

std::pair<int,int> SegmentTreeMaxMin::_query(int left,int right, int root, int query_left, int query_right) {
    if (query_left > right || query_right < left) {
        return std::make_pair(SegmentTreeMaxMin::MIN_VALUE, SegmentTreeMaxMin::MAX_VALUE);
    } else if (query_left <= left && right <= query_right) {
        return std::make_pair(_max[root], _min[root]);
    } else {
        int mid = (left + right) >> 1;
        std::pair<int,int> lson_info = _query(left, mid, root<<1, query_left, query_right);
        std::pair<int,int> rson_info = _query(mid+1, right, root<<1|1, query_left, query_right);
        return std::make_pair(std::max(lson_info.first, rson_info.first), std::min(lson_info.second, rson_info.second));     
    }
}

void SegmentTreeMaxMin::_push_up(int root) {
    _max[root] = std::max(_max[root<<1], _max[root<<1|1]);
    _min[root] = std::min(_min[root<<1], _min[root<<1|1]);
}

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1],"r",stdin);
    }
    SegmentTreeMaxMin * ptree = new SegmentTreeMaxMin(MAX_INTERVAL * 4);
    int n , Q, left, right;
    std::pair<int,int> result;
    while (scanf("%d%d", &n, &Q)!=EOF) {
        ptree->_build(1, n, SegmentTreeMaxMin::ROOT_INDEX);
        while (Q--) {
            scanf("%d%d", &left, &right);
            result = ptree->_query(1, n, SegmentTreeMaxMin::ROOT_INDEX, left, right);
            printf("%d\n", result.first - result.second);
        }
    }

    delete ptree;
    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
} 
