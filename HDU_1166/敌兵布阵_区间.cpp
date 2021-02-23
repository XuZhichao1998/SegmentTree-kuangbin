#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

const int MAX_NODE_COUNT = 131069+5; //经过测试，长度小于50000的数组建树需要的最大节点个数为131069
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
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    int T;
    std::cin>>T;
    SegmentTree * ptree = new SegmentTree();
    ptree->init(50000, MAX_NODE_COUNT);
    for (int ca = 1; ca <= T; ++ca) {
        int n;
        std::cin>>n;
        ptree->input_array(n);
        ptree->build(1,n,ROOT_INDEX);
        std::string op;
        std::cout<<"Case "<<ca<<":\n";
        while (std::cin>>op) {
            if (op == "End") {
                break;
            } else if (op == "Query") {
                int qleft;
                int qright;
                std::cin>>qleft>>qright;
                std::cout<<ptree->query(1,n,ROOT_INDEX,qleft,qright)<<"\n";
            } else if (op == "Add") {
                int index;
                int add_val;
                std::cin>>index>>add_val;
                ptree->update(1,n,ROOT_INDEX,index,index,add_val);
            } else if (op == "Sub") {
                int index;
                int sub_val;
                std::cin>>index>>sub_val;
                ptree->update(1,n,ROOT_INDEX,index,index,-sub_val);
            }
        }
    }
    delete ptree;
    
    return 0;
}