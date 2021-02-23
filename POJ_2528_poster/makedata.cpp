#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>

int main(int argc, const char * argv[]) {
    if(argc==1) {
        std::cout<<"没有输入期望的文件名比如1.in\n";
        return 0;
    }
    srand(time(NULL));
    std::cout<<"请输入n:(1~10000):";
    int T = 1, n;
    std::cin>>n;
    freopen(argv[1], "w", stdout);
    std::cout<<T<<"\n";
    for(int ca = 1; ca <= T; ++ca) {
        std::cout<<n<<"\n";
        for(int i = 1; i <= n; ++i) {
            int left = 1ll*rand()*rand()%10000000+1;
            int right = 1ll*rand()*rand()%10000000+1;
            if(left > right) {
                std::swap(left,right);
            }
            std::cout<<left<<" "<<right<<"\n";
        }
    } 

    fclose(stdin);
}
