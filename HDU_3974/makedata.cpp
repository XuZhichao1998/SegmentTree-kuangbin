#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <algorithm>
using namespace std;

int main(int argc, const char * argv[]) {
    int T = 1, n, m;
    cout<<"请输入n:";
    cin>>n;
    cout<<"请输入m:";
    cin>>m;

    freopen(argv[1], "w", stdout);
    cout<<T<<endl;
    cout<<n<<endl;
    srand(time(NULL));
    for(int i = 2; i<= n; ++i) {
        int v = i;
        int u = rand() % (i-1) + 1;
        printf("%d %d\n", v, u);
    }
    cout<<m<<endl;
    while (m--) {
        int op = rand()%2;
        if (op == 0) {
            printf("C %d\n", rand()%n+1);
        } else {
            printf("T %d %d\n", rand()%n+1, rand()%(1000000000)+1);
        }
    }

    fclose(stdout);

    return 0;
}
