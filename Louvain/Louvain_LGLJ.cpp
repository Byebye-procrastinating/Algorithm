#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <map>
#define R register
#define LL long long
using namespace std;
struct EDGE
{
    int next,to,val;
}edge[200050]={0},comm_edge[200050]={0};
int n,m;
LL summ=0;                              //图中所有边权和
bool flag;
int head[100050]={0},tot=0;
int innum[100050]={0};                  //指向单点的边权和
int id[100050]={0};
int comm_head[100050]={0},comm_tot=0;   //社团边 
int id_num[100050]={0},cnt=0;           //各个点所属社团
int interval[100050]={0};               //社团内部边权
int inval[100050]={0};                  //指向社团边权
int read()
{
	int x=0;
	char ch=0;
	bool w=true;
	while(!isdigit(ch)){if(ch=='-')w=false;ch=getchar();}
	while(isdigit(ch)){x=(x<<3)+(x<<1)+(ch^48);ch=getchar();}
	return w?x:-x;
}
void add_edge(int *H,EDGE *E,int &count,int u,int v,int w)              //加边
{
    E[++count].next=H[u];
    E[count].to=v;
    E[count].val=w;
    H[u]=count;
}
void data_input()                                                       //数据输入
{
    n=read();m=read(),flag=read();
    for(R int i=1,u,v,w;i<=m;++i)
    {
        u=read(),v=read(),w=read();
        add_edge(head,edge,tot,u,v,w);
        innum[v]+=w;summ+=w;
        if(!flag)
        {
            add_edge(head,edge,tot,v,u,w);
            innum[u]+=w;summ+=w;
        }
    }
}
void Init()                                                             //算法初始工作
{
    for(R int i=1;i<=n;++i)
        id[i]=i;
}
void Init_Louvain()
{
    comm_tot=0;
    for(R int u=1;u<=n;++u)
    {
        int ID=id[u];
        for(R int i=head[u];i;i=edge[i].next)
        {
            int v=edge[i].to,w=edge[i].val;
            if(ID==id[v])                                               //相同社团
            {
                interval[ID]+=w;
            }
            else                                                        //不同社团
            {
                add_edge(comm_head,comm_edge,comm_tot,ID,id[v],w);
                inval[id[v]]+=w;
            }
        }
    }
}
int get_Q(int u,int IDv,int kuin)
{
    return (LL)summ*kuin-(LL)inval[IDv]*innum[u];
}
int Louvain_work()                                                      //Louvain主体
{
    int sum=0;
    map<int,int> uout;                                                  //u指向社区的权值和
    priority_queue<pair<int,int> > que;
    for(R int u=1;u<=n;++u)
    {
        for(R int i=head[u];i;i=edge[i].next)
        {
            int v=edge[i].to,IDv=id[v],w=edge[i].val;
            uout[IDv]+=w;
        }
        map<int,int>::iterator it=uout.begin();
        while(it!=uout.end())
            que.push(make_pair( get_Q( u, (*it).first, (*it).second ) , (*it).first )  );
        if(que.top().first<=0)
            continue;
        int IDv = que.top().second;
        id[u]=IDv;                                                      //待解决
        ++sum;                                                          //计数，更改点位
    }
    return sum;
}
void Louvain()                                                          //Louvain外部工作
{
    int tag;
    do
    {
        Init_Louvain();
        tag=Louvain_work();
    }while(tag);
}
void data_output()                                                      //数据输出
{
    for(R int i=1;i<=n;++i)
        printf("%d is of community %d\n",i,id[i]);
}
int MAIN()
{
    data_input();
    Init();
    Louvain();
    data_output();
    return 0;
}
int LGLJ=MAIN();
int main(){;}
