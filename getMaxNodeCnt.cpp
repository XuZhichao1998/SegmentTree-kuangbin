#include <cstdio>
#include <iostream>

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

int main(void) {
    std::cout<<"请输入区间长度：";
    int len;
    std::cin>>len;
    double dv = 0;
    int maxLen = 0;
    int record = 0;
    for (int i = len; i >=1; --i) {
        if ( i*4 <= maxLen ) {
            break;
        }
        int xx = getMinSize(i);
        if(xx>maxLen) maxLen = xx,record = i;
        //dv = std::max(dv,1.0*xx/i);
    }
    std::cout<<dv<<std::endl;
    std::cout<<record<<"\n";
    std::cout<<"需要的节点数："<<maxLen<<"\n";
    return 0;
    int x;
    while (std::cin>>x) {
        std::cout<<x<<"个元素需要的最大index为:"<<getMinSize(x)<<std::endl;
    }

    return 0;
}
