#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

const int MAX_NODE_COUNT = 262141+5; //经过测试，长度小于100000的数组建树需要的最大节点个数为262141
const int ROOT_INDEX = 1;

class SegmentTree {
public:
    void init(int arr_length, int max_length);
    void input_array(int array_size);
    void build(int left, int right, int rt);
    long long update(int left, int right, int rt, int update_left, int update_right, int add);
    long long query(int left, int right, int rt, int query_left, int query_right); 
private:
    std::vector<long long> sum;
    std::vector<long long> lazy;
    std::vector<int> arr;
    void push_down(int rt, int left, int right);
};
void SegmentTree::init(int arr_length, int max_length) {
    arr.resize(arr_length+1);
    sum.resize(max_length);
    lazy.resize(max_length);
}
void SegmentTree::input_array(int array_size) {
    for (int i = 1; i <= array_size; ++i) {
        std::cin>>arr[i];
    }
}

void SegmentTree::build(int left,int right,int rt) {
    lazy[rt] = 0;
    if (left == right) {
        sum[rt] = arr[right];
        return;    
    }
    int mid = (left + right) >> 1;
    build(left, mid, rt<<1);
    build(mid+1, right, rt<<1|1);
    sum[rt] = sum[rt<<1] + sum[rt<<1|1];
    return;
}
long long SegmentTree::update(int left, int right, int rt, int update_left, int update_right, int add) {
    if (update_left > right || update_right < left) {
        return sum[rt];
    }
    if(update_left <= left && right <= update_right) {
        sum[rt] += 1ll * add * (right - left + 1);
        lazy[rt] += add;
        return sum[rt];
    }
    int mid = (left + right) >> 1;
    push_down(rt, left, right);
    long long sum_lson = update(left, mid, rt<<1, update_left, update_right, add);
    long long sum_rson = update(mid+1, right, rt<<1|1, update_left, update_right, add);
    return sum[rt] = sum_lson + sum_rson;
}
long long SegmentTree::query(int left, int right, int rt, int query_left, int query_right) {
    if (query_left > right || query_right < left) {
        return 0;
    }
    if (query_left <= left && right <= query_right) {
        return sum[rt];
    }
    int mid = (left + right) >> 1;
    push_down(rt, left, right);
    long long sum_lson = query(left, mid, rt<<1, query_left, query_right);
    long long sum_rson = query(mid+1, right, rt<<1|1, query_left, query_right);
    return sum_lson + sum_rson;
}
void SegmentTree::push_down(int rt,int left,int right) {
    if (lazy[rt] != 0) {   
        int mid = (left + right) >> 1;
        int lson = rt << 1;
        int rson =  rt << 1 | 1;
        lazy[lson] += lazy[rt];
        lazy[rson] += lazy[rt];
        sum[lson] += lazy[rt] * (mid - left + 1);
        sum[rson] += lazy[rt] * (right - mid);
        lazy[rt] = 0;
    }
}

void getMaxSize(int left,int right,int rt,int & maxx) {
    maxx = std::max(maxx,rt);
    if (left == right) {
        return;
    } 
    int mid = (left + right) >> 1;
    getMaxSize(left,mid,rt<<1,maxx);
    getMaxSize(mid+1,right,rt<<1|1,maxx);
}
int getMinSize(int n) {
    int maxx = 0;
    getMaxSize(1,n,1,maxx);
    return maxx;
}

int main(int argc,const char * argv[]) {
//    freopen("in.txt", "r", stdin);

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    SegmentTree * ptree = new SegmentTree();
    ptree->init(100000, MAX_NODE_COUNT);
    
    int arr_length, query_count;
    while (std::cin>>arr_length>>query_count) {
        ptree->input_array(arr_length);
        ptree->build(1,arr_length,ROOT_INDEX);
        std::string op;
        while (query_count--) {
            std::cin>>op;
            if (op == "Q") {
                int qleft;
                int qright;
                std::cin>>qleft>>qright;
                std::cout<<ptree->query(1, arr_length, ROOT_INDEX, qleft, qright)<<"\n";
            } else if (op == "C") {
                int update_left;
                int update_right;
                int add_val;
                std::cin>>update_left>>update_right>>add_val;
                ptree->update(1, arr_length, ROOT_INDEX, update_left, update_right, add_val);
            }
        }
    }
    delete ptree;
    
    return 0;
}
