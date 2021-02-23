/*
Author:XuZhichao
date: 2021.2.8
线段树
*/
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

const int MAX_NODE_COUNT = 524285+10; //200000的区间建树需要的最多节点数
const int ROOT_INDEX = 1; //跟节点编号为1
class SegmentTreeMax {
public:
    void init(int arr_length, int max_length);
    void input_array(int arr_length);
    void build(int left, int right, int root);
    void modify(int left, int right, int root, int index, int new_value);
    int query(int left, int right, int root, int query_left, int query_right);
private:
    const static int MIN_SCORE; //成绩最低为0分
    std::vector<int> max_val;
    std::vector<int> arr;
    void push_up(int root);
};

const int SegmentTreeMax::MIN_SCORE = 0;

void SegmentTreeMax::init(int arr_length, int max_length) {
    arr.resize(arr_length+1);
    max_val.resize(max_length);
}
void SegmentTreeMax::input_array(int arr_length) {
    for (int i = 1; i <= arr_length; ++i) {
        std::cin>>arr[i];
    }
}

void SegmentTreeMax::build(int left, int right, int root) {
    if (left == right) {
        max_val[root] = arr[right];
        return;
    }
    int mid = (left + right) >> 1;
    build(left, mid, root<<1);
    build(mid+1,right,root<<1|1);
    push_up(root);
} 

void SegmentTreeMax::modify(int left, int right, int root, int index, int new_value) {
    if (index > right || index < left) {
        return;
    }
    if (left == right) {
        max_val[root] = new_value;
        return;
    }
    int mid = (left + right) >> 1;
    if (index <= mid) {
        modify(left, mid, root<<1, index, new_value);
    } else {
        modify(mid+1, right, root<<1|1, index, new_value);
    }
    push_up(root);
}

int SegmentTreeMax::query(int left, int right, int root, int query_left, int query_right) {
    if (query_left > right || query_right < left) {
        return SegmentTreeMax::MIN_SCORE; 
    }
    if (query_left <= left && right <= query_right) {
        return max_val[root];
    }
    int mid = (left + right) >> 1;
    int max_lson = query(left, mid, root<<1, query_left, query_right);
    int max_rson = query(mid+1, right, root<<1|1, query_left, query_right);
    return std::max(max_lson, max_rson);
}

void SegmentTreeMax::push_up(int root) {
    max_val[root] = std::max(max_val[root<<1], max_val[root<<1|1]);
}

int main(void) { 
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int arr_length, query_count;
    SegmentTreeMax * ptree = new SegmentTreeMax();
    ptree->init(200000, MAX_NODE_COUNT);
    while (std::cin>>arr_length>>query_count) {
        ptree -> input_array(arr_length);
        ptree -> build(1, arr_length, ROOT_INDEX);
        std::string option_type;
        while (query_count--) {
            std::cin>>option_type;
            if (option_type == "Q") {
                int query_left;
                int query_right;
                std::cin>>query_left>>query_right;
                std::cout<<ptree->query(1, arr_length, ROOT_INDEX, query_left, query_right)<<"\n";
            } else if (option_type == "U") {
                int index;
                int new_score;
                std::cin>>index>>new_score;
                ptree->modify(1, arr_length, ROOT_INDEX, index, new_score);
            }

        }
    }

    delete ptree;
    fclose(stdin);

    return 0;
}

