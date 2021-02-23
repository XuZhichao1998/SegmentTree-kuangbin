#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <queue>
#include <vector>
#define Mst(a,b) memset(a,(b),sizeof(a))
#define For(i,a,b) for(register int i=(a);i<=(b);++i)
#define LL long long
using namespace std;
const int INF = 1e9;
const int maxn = 2e5 + 5;
struct Node{
    int left,right,Min,lazy;
    int mid(){return (left+right)>>1;}
}tree[maxn<<2];
void build(int left,int right,int pos=1)
{
    tree[pos].left = left;
    tree[pos].right = right;
    tree[pos].Min = 0;
    tree[pos].lazy = 0;
    if(left==right) return;
    int mid = tree[pos].mid();
    build(left,mid,pos<<1);
    build(mid+1,right,pos<<1|1);
}
void push_down(int pos)
{
    if(!tree[pos].lazy) return;
    tree[pos<<1].Min += tree[pos].lazy;
    tree[pos<<1].lazy += tree[pos].lazy;
    tree[pos<<1|1].Min += tree[pos].lazy;
    tree[pos<<1|1].lazy += tree[pos].lazy;
    tree[pos].lazy = 0;
}
int update(int left,int right,int pos=1) ///区间都加1
{
    if(left>tree[pos].right || right<tree[pos].left) //不是要add的区间 
        return tree[pos].Min;
    if(left<=tree[pos].left && right>=tree[pos].right)
    {
        ++tree[pos].lazy;
        return ++tree[pos].Min;
    }
    push_down(pos); 
    return tree[pos].Min = min(update(left,right,pos<<1),update(left,right,pos<<1|1));
}
int query(int left,int right,int pos=1)
{
    if(left>tree[pos].right || right<tree[pos].left)
        return INF;
    if(left<=tree[pos].left && right>=tree[pos].right)
        return tree[pos].Min;
    push_down(pos);
    return min(query(left,right,pos<<1),query(left,right,pos<<1|1));
}
vector<pair<int,int> > v;
map<int,int> mp;///10000000 to 10000
int a[200000+5];
int x[100000+5];
int y[100000+5];
int main(int argc, const char * argv[])
{
	if(argc==2) {
        freopen(argv[1],"r",stdin);
    }
	//freopen("in1.txt","r",stdin);
   //	freopen("out.txt","w",stdout);
    int T,n,left,right,MaxRight,ans,cnt;
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d",&n);
        MaxRight = 1;
        v.clear();
        mp.clear();
        cnt = 0;
        For(i,1,n)
        {
            scanf("%d%d",x+i,y+i);
            a[++cnt] = x[i];
            a[++cnt] = y[i];
        }
        sort(a+1,a+1+2*n);   ///离散化 
        a[0] = -1000000; 
        cnt = -1;
        For(i,1,2*n)
        {
            if(a[i]==a[i-1])
            {
                continue;
            }
            if(a[i]==a[i-1]+1)
            {
                mp[a[i]] = ++cnt;
            }
            else
            {
                cnt+=2;
                mp[a[i]] = cnt;
            }
        }
        For(i,1,n)
        {
            left = mp[x[i]];
            right = mp[y[i]];
            MaxRight = max(MaxRight,right);
            v.push_back(make_pair(left,right));
            //printf("(%d, %d)\n",left,right);
        }
        build(1,MaxRight);
        ans = 0;
        int len = v.size();
        for(int i=len-1;i>=0;--i)
        {
            int M = query(v[i].first,v[i].second);
            if(M==0)
            {
                ++ans;
            }
            update(v[i].first,v[i].second);
        }
        printf("%d\n",ans);
    }
    if(argc==2) {
        fclose(stdin);
    }

	return 0;
}
