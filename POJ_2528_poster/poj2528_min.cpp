/*
POJ 2528
Authon: XuZhichao
2021.2.8 18.29
*/

// 线段树维护区间最小值。
// 我们规定原来不贴海报的颜色为0，第i张海报的颜色为i(1<=i<=n)
// 我们将询问离散化。原来不相邻的点离散化后也至少要隔开一个单元(差值>=2)，因为这样可以保证不会把原来下放露出的海报完全遮挡
// 如果倒序处理海报。那么在贴每张海报的时候，关心的问题仅仅是当前区间还有没有空白的单元。如果有空白的单元，表示是后面海报没有遮挡的单元。我们直
// 接查询区级最小值，如果最小值M为0，则说明还有空白的地方。
// 查询之后如果M为0，我们答案就+1 然后我们进行更新。把这张海报覆盖的范围全部涂色(区间赋值)即可。涂什么颜色不重要，大于0即可。
// 所以离散化，倒叙处理以后，我们可以转化为简答的 线段树维护区间最小值。

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <map>
#include <cassert>

class SegmentTreeColor {
public:
    const static int ROOT_INDEX;
    const static int INF; 
    void _init(int max_length);
    SegmentTreeColor();
    ~SegmentTreeColor();
    void build(int left, int right, int root);
    int update(int left, int right, int root, int update_left, int update_right, int value); //区间赋值
    int query(int left, int right, int root, int query_left, int query_right); //查询区间最小值
private:
    int * _min_value; //记录区间最小值
    bool * _vis; //lazy标签，是否需要push_down 
    void push_down(int root); 
};

const int SegmentTreeColor::ROOT_INDEX = 1; //线段树默认根节点编号为1，后面则可以满足lson = root*2 rson = root*2+1
const int SegmentTreeColor::INF = 1E9; //大于离线询问总数（贴海报总数）即可
SegmentTreeColor::SegmentTreeColor() {
    _min_value = NULL;
    _vis = NULL;
}
void SegmentTreeColor::_init(int max_length) {
    _min_value = new int[max_length+1];
    _vis = new bool[max_length+1];
}
SegmentTreeColor::~SegmentTreeColor() {
    if (!_min_value) {
        delete [] _min_value;
    }
    if (!_vis) {
        delete [] _vis;
    }
}

void SegmentTreeColor::build(int left, int right, int root) {
    _min_value[root] = 0;
    _vis[root] = false;
    if (left == right) {
        return;
    }
    int mid = (left + right) >> 1;
    build(left, mid, root<<1);
    build(mid+1, right, root<<1|1); 
}

int SegmentTreeColor::update(int left, int right, int root, int update_left, int update_right, int value) {
    if (update_left > right || update_right < left) {
        return _min_value[root];     
    } else if (update_left <= left && right <= update_right) {
        _vis[root] = true;
        return _min_value[root] = value;

    } else {
        int mid = (left + right) >> 1;
        push_down(root);
        int min_lson = update(left, mid, root<<1, update_left, update_right, value);
        int min_rson = update(mid+1, right, root<<1|1, update_left, update_right, value);
        return  _min_value[root] = std::min(min_lson,min_rson);
    }
}

int SegmentTreeColor::query(int left, int right, int root, int query_left, int query_right) {
    if (query_left > right || query_right < left) {
        return SegmentTreeColor::INF;
    } else if (query_left <= left && right <= query_right) {
        return _min_value[root];
    } else {
        int mid = (left + right) >> 1;
        push_down(root);
        int min_lson = query(left, mid, root<<1, query_left, query_right);
        int min_rson = query(mid+1, right, root<<1|1, query_left, query_right);
        return std::min(min_lson, min_rson);
    }
}

void SegmentTreeColor::push_down(int root) {
    if (_vis[root]) {
        _min_value[root<<1] = _min_value[root<<1|1] = _min_value[root];
        _vis[root<<1] = _vis[root<<1|1] = true;
        _vis[root] = false;
    }
}

const int MAXN = 10000+5;
int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1],"r",stdin);
    }
    int T, n, left, right;
    scanf("%d", &T);
    std::pair<int,int> * intervals = new std::pair<int,int>[MAXN];
    int * arr = new int[MAXN*2];
    SegmentTreeColor * ptree = new SegmentTreeColor();
    ptree->_init(MAXN*16); //10000个区间，20000个数。离散化要求原先不相邻的也不能相邻，最坏情况为每个数之间都差2，40000,线段树4倍空间
    while (T--) {
        scanf("%d", &n);
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            scanf("%d%d", &left, &right);
            intervals[i] = std::make_pair(left, right);
            arr[cnt++] = left;
            arr[cnt++] = right;
        }
        std::sort(arr, arr+cnt);
        cnt = std::unique(arr, arr+cnt) - arr; //cnt为去重后元素的个数
        std::map<int,int> mp;
        mp[arr[0]] = 1;
        int max_length = 1;
        for (int i = 1; i < cnt; ++i) {
            if (arr[i] == arr[i-1] + 1) {
                mp[arr[i]] = ++max_length;
            } else {
                max_length +=2;
                mp[arr[i]] = max_length;
            }
        }
        ptree->build(1, max_length, SegmentTreeColor::ROOT_INDEX);
        int ans = 0;
        for (int i = n-1; i >= 0; --i) {
            left = mp[intervals[i].first];
            right = mp[intervals[i].second];
            int cur_min = ptree->query(1, max_length, SegmentTreeColor::ROOT_INDEX, left, right);
            if (cur_min == 0) { //0代表初始没贴海报的值。表示这个区间还有空的地方没有被后面的覆盖。post[i]可以露出来。
                ++ans;
            }
            //最后一个参数i+1代表当前海报的颜色。其实我们并不关心具体什么颜色。传递大于0的任何参数都可以
            ptree->update(1, max_length, SegmentTreeColor::ROOT_INDEX, left, right, i+1); 

        }
        printf("%d\n", ans);
    }

    delete ptree;
    delete []intervals;
    delete []arr;
    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}
