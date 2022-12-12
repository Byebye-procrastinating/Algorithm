# Louvain 数据格式

第1行，三个整数 **n,m,t** 

- **n** 表示点的个数
- **m** 表示边的条数
- **t** 表示图是否为有向图
  - `t=1` 表示图为有向图
  - `t=0` 表示图为无向图

第2到m+1行，三个整数 **u,v,w**，表示一条边
- **u** 表示边的起点
- **v** 表示边的终点
- **w** 表示边权