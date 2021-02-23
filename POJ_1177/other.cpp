#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;
#define ls i<<1
#define rs i<<1|1
#define m(i) ((q[i].l+q[i].r)>>1)
typedef long long ll;
const int N = 5007;
const int X = 20007;
const int inf = 1<<29;
struct Edge{
	int l,r;
	int h;
	int f;
	bool operator < (const Edge &a)const{
		return h < a.h;
	}
}e[N*2];
struct Node{
	int l,r;
	int len;
	int s;
	bool lc,rc;//表示这个节点代表的区间左右两个端点是否被覆盖
	int num;//这个区间有多少不连续到线段
}q[X*4];
void pushup(int i){
	if(q[i].s){
		q[i].len = q[i].r - q[i].l + 1;
		q[i].lc = q[i].rc = 1;
		q[i].num = 1;
	}

    // q[i].s == 0 
	else if(q[i].l == q[i].r){
		q[i].len = 0;
		q[i].lc = q[i].rc = 0;
		q[i].num = 0;
	}
	else{

		q[i].len = q[ls].len + q[rs].len;
		q[i].lc = q[ls].lc;
		q[i].rc = q[rs].rc;
        printf("push_up(%d): num[i] = num[lson] + num[rson] - (lson.rc & rson.lc) = %d + %d - (%d & %d)", i, q[ls].num, q[rs].num, q[ls].rc, q[rs].lc);
		q[i].num = q[ls].num + q[rs].num - (q[ls].rc & q[rs].lc);
        printf(" = %d\n",q[i].num);
	}
}
void build(int i,int l,int r){
	q[i].l = l;
	q[i].r = r;
	q[i].s = q[i].len = 0;
	q[i].lc = q[i].rc = q[i].num = 0;
	if(l == r) return;
	int mid = m(i);
	build(ls,l,mid);
	build(rs,mid+1,r);
}
void update(int i,int l,int r,int xx){
	if(l == q[i].l && q[i].r == r){
		q[i].s += xx;
		pushup(i);
		return;
	}
	int mid = m(i);
	if(r <= mid) update(ls,l,r,xx);
	else if(l > mid) update(rs,l,r,xx);
	else{
		update(ls,l,mid,xx);
		update(rs,mid+1,r,xx);
	}
	pushup(i);
}
int main(){
	int n;
	while(scanf("%d",&n) != EOF){
		int x1,x2,y1,y2,mx = -inf,mn = inf;
		int tot = 0;
		for(int i = 0; i < n; i++){
			scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
			mx = max(mx,max(x1,x2));
			mn = min(mn,min(x1,x2));
			Edge &t1 = e[tot];
			Edge &t2 = e[tot+1];
			t1.l = t2.l = x1;
			t1.r = t2.r = x2;
			t1.h = y1;
			t1.f = 1;
			t2.h = y2;
			t2.f = -1;
			tot += 2;
		}
		sort(e,e+tot);
		//数据小不用离散化
		int ans = 0;
		int last = 0;
		build(1,mn,mx-1);
		for(int i = 0; i < tot; i++){
            printf("\ni = %d,update(%d, %d, %d) h = %d\n", i, e[i].l, e[i].r-1, e[i].f, e[i].h);
			update(1,e[i].l,e[i].r-1,e[i].f);
            printf("ans = ans + abs(q[1].len - last) = %d + abs(%d - %d)", ans, q[1].len, last);
            ans += abs(q[1].len - last);
            printf(" = %d\n", ans);
            printf("ans = ans + difh * 2 * q[1].num = %d + %d * 2 * %d", ans, e[i+1].h-e[i].h, q[1].num);
			ans += (e[i+1].h - e[i].h) * 2 * q[1].num;
            printf(" = %d\n", ans);
			last = q[1].len;
		}
		printf("%d\n",ans);
	}
	return 0;
}
