#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

const int MAX_NODE = 40003*4;

class SegmentTreeColor {
public:
    const static int ROOT_INDEX;
    void init(int max_length); 
    void build(int left, int right, int root);
    int update(int left, int right, int root, int update_left, int update_right, int col);
    void dfs(int left, int right, int root, std::vector<int>& vis, int& ans);
    
private:
    std::vector<int> color;
    std::vector<int> lazy;
    void push_down(int root, int left, int right);
};
const int SegmentTreeColor::ROOT_INDEX = 1;

void SegmentTreeColor::init(int max_length) {
    color.resize(max_length,0);
    lazy.resize(max_length,0);
}
void SegmentTreeColor::build(int left, int right, int root) {
    color[root] = lazy[root] = 0;
    if(left == right) {
        return;
    }
    int mid = (left + right) >> 1;
    build(left, mid, root<<1);
    build(mid+1, right, root<<1|1);
}

int SegmentTreeColor::update(int left, int right, int root, int update_left, int update_right, int col) {
    if (update_left > right || update_right < left) {
        return color[root];
    } else if(update_left <= left && right <= update_right) {
        lazy[root] = col;
        return color[root] = col; 
    } else {
        int mid = (left + right) >> 1;
        push_down(root, left, right);
        int col_lson = update(left, mid, root<<1, update_left, update_right, col);
        int col_rson = update(mid+1, right, root<<1|1, update_left, update_right, col); 
        if (col_lson == -1 || col_rson == -1 || col_lson != col_rson) {
            color[root] = -1;
        } else {
            color[root] = col_lson;
        }
        return color[root];
    }
}

void SegmentTreeColor::dfs(int left, int right, int root, std::vector<int>& vis, int& ans) {
   if (color[root] != -1) {
       if (color[root] != 0) {
           if (vis[color[root]] == 0) {
               ++ans;
               vis[color[root]] = 1;
           }
       }
       return;
   } 
   int mid = (left + right) >> 1;
   push_down(root, left, right);
   dfs(left, mid, root<<1, vis, ans);
   dfs(mid+1, right, root<<1|1, vis, ans);
}

void SegmentTreeColor::push_down(int root, int left, int right) {
    if (lazy[root] > 0) {
        int mid = (left + right) >> 1;
        int lson = root<<1;
        int rson = root<<1|1;
        lazy[lson] = color[lson] = lazy[root];
        lazy[rson] = color[rson] = lazy[root];
        lazy[root] = 0;
    }
}

int main(int argc, const char * argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    if (argc == 2) {
        freopen(argv[1],"r",stdin);
    }
    int T,n;
    std::cin>>T;
    SegmentTreeColor * ptree = new SegmentTreeColor();
    ptree->init(MAX_NODE);
    int * arr = new int[20001];
    std::vector<std::pair<int,int> > intervals;
    intervals.resize(10000);
    while (T--) {
        std::cin>>n;
        int index = 0;
        int start,end;
        for (int i = 0; i < n; ++i) {
            std::cin>>start>>end;
            arr[index++] = start;
            arr[index++] = end;
            intervals[i] = std::make_pair(start,end);
        }
        std::sort(arr,arr+index);
        int len = std::unique(arr,arr+index) - arr;
        std::map<int,int> mp;
        int cnt = 1;
        mp[arr[0]] = 1;
        for (int i = 1; i < len; ++i) {
            if (arr[i] == arr[i-1]+1) {
                mp[arr[i]] = ++cnt;
            } else {
                mp[arr[i]] = cnt+2;
                cnt += 2;
            }
        }
        for (int i = 0; i < n; ++i) {
            intervals[i].first = mp[intervals[i].first];
            intervals[i].second = mp[intervals[i].second];
        }
        int max_length = cnt;
        ptree->build(1, max_length, SegmentTreeColor::ROOT_INDEX);
        for (int i = 0; i < n; ++i) {
            ptree->update(1, max_length, SegmentTreeColor::ROOT_INDEX, intervals[i].first, intervals[i].second, i+1);
        }   
        std::vector<int> vis;
        vis.resize(n+10,0);
        int ans = 0;
        ptree->dfs(1, max_length, SegmentTreeColor::ROOT_INDEX, vis, ans);
        printf("%d\n",ans);
    }

    delete []arr;
    delete ptree;

    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}

