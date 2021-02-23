#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#define For(i,a,b) for(register int i=(a);i<=(b);++i)
#define Rep(i,a,b) for(register int i=(a);i>=(b);--i)
#define Mst(a,b) memset(a,(b),sizeof(a))
#define LL long long
using namespace std;
///dfs重新编号，使得每个节点的所有子节点连续
///然后区间更新，单点查询
const int maxn = 5e4+100;
int head[maxn],n;
int lastSon[maxn]; ///记录最后一个儿子的id
int ID[maxn]; ///记录每个节点新分配的id
bool vis[maxn];
struct Edge{ ///链式前向星
    int to,Next;
}edge[maxn];
int tot;
void init()
{
    memset(vis,0,sizeof(bool)*(n+2));
    tot = 0;
    memset(head,-1,sizeof(int)*(n+2));
    //Mst(vis,0);
   // Mst(head,-1);
}
void addedge(int fa,int son)
{
    edge[tot].to = son;
    edge[tot].Next = head[fa];
    head[fa] = tot++;
}
int cnt;
void dfs(int root)
{
    ID[root] = ++cnt;
    //vis[root] = true;
    for(int i=head[root];i!=-1;i=edge[i].Next)
    {
        int to = edge[i].to;
        //if(vis[to]) continue;
       // vis[to] = true;
        dfs(to);
    }
    lastSon[root] = cnt;
}
struct SegmentTree{
    int left,right,val;
    bool lazy; ///lazy为true表示需要下放，false表示不需要下放
    int mid(){return (left+right)>>1;}
}tree[maxn<<2];
void build(int left,int right,int pos=1)
{
    tree[pos].left = left;
    tree[pos].right = right;
    tree[pos].val = -1;
    tree[pos].lazy = false; ///lazy==-1表示不需要下放了
    if(left==right) return;
    int mid = (left+right)>>1;
    build(left,mid,pos<<1);
    build(mid+1,right,pos<<1|1);
}
void push_down(int pos)
{
    if(!tree[pos].lazy) return;
    tree[pos<<1].val = tree[pos<<1|1].val = tree[pos].val;
    tree[pos<<1].lazy = tree[pos<<1|1].lazy = true;
    tree[pos].lazy = false;
}
void update(int left,int right,int task,int pos=1)
{   ///只有单点查询，貌似不用push_up
    if(left>tree[pos].right || right<tree[pos].left)
        return; ///不在要更新的范围内
    if(left<=tree[pos].left && right>=tree[pos].right)
    {
        tree[pos].lazy = true;
        tree[pos].val = task;
        return;
    }
    push_down(pos);
    update(left,right,task,pos<<1);
    update(left,right,task,pos<<1|1);
}
int query(int x,int pos=1)
{
    if(tree[pos].left==tree[pos].right)
        return tree[pos].val;
    push_down(pos);
    int mid = tree[pos].mid();
    if(x<=mid) return query(x,pos<<1);
    return query(x,pos<<1|1);
}
int main(int argc, const char * argv[])
{
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    //freopen("out.txt","w",stdout);
    int T,x,task,m,fa,son;
    char op[3];
    scanf("%d",&T);
    For(ca,1,T)
    {
        scanf("%d",&n);
        init();
        For(i,2,n)
        {
            scanf("%d%d",&son,&fa);
            vis[son] = true;
            addedge(fa,son);
        }
        int root = 1;
        For(i,1,n)
        {
            if(!vis[i])
            {
                root = i;break;
            }
        }
       // memset(vis,false,sizeof(bool)*(n+2));
        cnt = 0;
        dfs(root);
        build(1,n);
        scanf("%d",&m);
        printf("Case #%d:\n",ca);
        while(m--)
        {
            scanf("%s%d",op,&x);
            switch(op[0])
            {
                case 'C':printf("%d\n",query(ID[x]));break;
                default:scanf("%d",&task);update(ID[x],lastSon[x],task);break;
            }
        }
    }

    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}
