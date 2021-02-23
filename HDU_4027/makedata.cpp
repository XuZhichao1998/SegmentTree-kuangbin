#include <iostream>
#include <cstdio>
#include <algorithm>
#include <ctime>

int main(int argc, const char * argv[]) {
    
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int n,m;
    //std::cout<<"请输入n(1~100000):";
    //std::cin>>n;
    n = 100000;
    freopen(argv[1], "w", stdout);
    std::cout<<n<<"\n";
    srand(time(NULL));
    long long mod = 1E13+7;
    for (int i= 1; i <= n; ++i) {
        std::cout<<1ll*rand()*rand() % mod<<" ";
    }
    std::cout<<"\n";
    m = 100000;
    std::cout<<m<<"\n";
    while (m--) {
        int op = rand()%2;
        int left = rand()%n+1;
        int right = rand()%n+1;
        std::cout<<op<<" "<<left<<" "<<right<<"\n";
    }
    fclose(stdout);

    return 0;
}
