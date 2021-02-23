/*
Author:XuZhichao
Date:2021.2.8
线段树单点更新，区间查询，维护区间和
Problem : 1166 ( 敌兵布阵 )     Judge Status : Accepted
RunId : 35314846    Language : G++    Author : apareHDU
Code Render Status : Rendered By HDOJ G++ Code Render Version 0.01 Beta
202ms
 */
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

const int ROOT_INDEX = 1;
const int MAX_NODE_COUNT = 131069+5; //经过测试，长度小于50000的数组建树需要的最大节点个数为13169

class SegmentTree {
public:
    void init(int array_length, int max_length) {
        sum.resize(max_length);
        arr.resize(array_length+1);
    }
    void input_array(int array_length) {
        for (int i = 1; i <= array_length; ++i) {
            std::cin>>arr[i];
        }
    }
    void build(int left,int right,int rt) {
        if(left==right) {
            sum[rt] = arr[left];
            return;    
        }
        int mid = (left+right) >> 1;
        build(left,mid,rt<<1);
        build(mid+1,right,rt<<1|1);
        sum[rt] = sum[rt<<1] + sum[rt<<1|1];
        return;
    }
    long long query(int left,int right,int rt,int query_left,int query_right) {
        if (query_left > right || query_right < left) {
            return 0;
        }
        if (query_left <= left && right <= query_right) {
            return sum[rt];
        }
        int mid = (left+right) >> 1;
        long long sum_lson = query(left,mid,rt<<1,query_left,query_right);
        long long sum_rson = query(mid+1,right,rt<<1|1,query_left,query_right);
        return sum_lson + sum_rson;
    }
    long long modify(int left, int right, int rt, int index, int add) {
        if (index < left || index > right) {
            return sum[rt];
        }
        if (left == right) {
            return sum[rt] = sum[rt] + add;
        }
        int mid = (left + right) >> 1;
        long long sum_lson = modify(left, mid, rt<<1, index,add);
        long long sum_rson = modify(mid+1, right, rt<<1|1, index,add); 
        return sum[rt] = sum_lson + sum_rson;
    }
private:
    std::vector<long long> sum;
    std::vector<int> arr;
};

int main(int argc,const char * argv[]) {
//    freopen("敌兵布阵.in","r",stdin);
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
                ptree->modify(1,n,ROOT_INDEX,index,add_val);
            } else if (op == "Sub") {
                int index;
                int sub_val;
                std::cin>>index>>sub_val;
                ptree->modify(1,n,ROOT_INDEX,index,-sub_val);
            }
        }
    }
    delete ptree;

    return 0;
}
