#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <random>
#include <map>
#include <unordered_map>
#define R register
#define LL long long
using namespace std;
const double eps=1e-6;
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

int read()
{
	int x=0;
	char ch=0;
	bool w=true;
	while(!isdigit(ch)){if(ch=='-')w=false;ch=getchar();}
	while(isdigit(ch)){x=(x<<3)+(x<<1)+(ch^48);ch=getchar();}
	return w?x:-x;
}
void add_edge(int *H,EDGE *E,int &count,int u,int v,int w=1)              //加边
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
void data_output()                                                      //数据输出
{
    for(R int i=1;i<=n;++i)
        printf("%d is of community %d\n",i,id[i]);
}
void LPA()
{
    int cnt = 0;
    bool flag=true;
    random_device rd;
	mt19937 RNG(rd());
    while(flag)
    {
        ++cnt;
        flag = false;
        for(R int u=1;u<=n;++u)
        {
            unordered_map<int,int> Map;
            for(int i = head[u],v;i;i = edge[i].next)
            {
                v = edge[i].to;
                Map[id[v]] += edge[i].val;
            }
            int MAX=-1;
            for(unordered_map<int,int>::iterator it=Map.begin();it!=Map.end();++it)
                if(it->second > MAX)
                    MAX = it->second;
            vector<int> choices;
            for(unordered_map<int,int>::iterator it=Map.begin();it!=Map.end();++it)
                if(it->second == MAX)
                    choices.push_back(it->second);
            shuffle(choices.begin(),choices.end(),RNG);
            if(id[u] != id[choices[0]])
            {
                id[u] = id[choices[0]];
                flag = true;
            }
        }   
    }
}
int MAIN()
{
    data_input();
    Init();
    LPA();
    data_output();
    return 0;
}
int LGLJ=MAIN();
int main(){;}
