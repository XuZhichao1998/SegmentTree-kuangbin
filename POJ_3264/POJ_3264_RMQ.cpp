#include <cstdio>
#include <algorithm> //std::max std::min

const int MAXN = 50003;
namespace myRMQ {
    void initLog2(int * Log2, int n) {
        Log2[1] = 0;
        for (int i = 2; i <= n; ++i) {
            Log2[i] = Log2[i>>1] + 1;
        } 
    }
    void get_st_table(int * arr, int n, int st_max[][20], int st_min[][20]) {
        for (int i = 1; i <= n; ++i) {
            st_max[i][0] = st_min[i][0] = arr[i];
        }
        for (int j = 1; (1<<j) < n; ++j) {
            for (int i = 1; i+(1<<j)-1 <= n; ++i) {
                st_max[i][j] = std::max(st_max[i][j-1], st_max[i+(1<<(j-1))][j-1]);
                st_min[i][j] = std::min(st_min[i][j-1], st_min[i+(1<<(j-1))][j-1]);
            }  
        }        
    }
    int get_interval_max(int st_max[][20], int left, int right, int * Log2) {
        int len = right - left + 1;
        int m = Log2[len];
        return std::max(st_max[left][m], st_max[right-(1<<m)+1][m]);
    }
    int get_interval_min(int st_min[][20], int left, int right, int * Log2) {
        int len = right - left + 1;
        int m = Log2[len];
        return std::min(st_min[left][m], st_min[right-(1<<m)+1][m]);
    }
};

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    static int Log2[MAXN];
    static int arr[MAXN];
    static int st_max[MAXN][20];
    static int st_min[MAXN][20];

    myRMQ::initLog2(Log2,MAXN-1);

    int n, Q, left, right;
    while (scanf("%d%d", &n, &Q) != EOF) {
        for (int i = 1; i <= n; ++i) {
            scanf("%d", &arr[i]);
        }
        myRMQ::get_st_table(arr, n, st_max, st_min);
        while (Q--) {
            scanf("%d%d", &left, &right);
            int max_value = myRMQ::get_interval_max(st_max, left, right, Log2);
            int min_value = myRMQ::get_interval_min(st_min, left, right, Log2);
            printf("%d\n", max_value - min_value);
        }
    }
    
    return 0;
}
