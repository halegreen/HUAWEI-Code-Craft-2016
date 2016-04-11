#ifndef __ROUTE_H__
#define __ROUTE_H__

void search_route(char *graph[5000], int edge_num, char *condition);

#define INFINITY_MAX 10000
#define EXCEPTIONNODE 600 //任意设置的节点，该节点不在图中出现的节点范围内
#define MAXNODES 600   //最大总节点数
#define DEMANDNODES 51 //要求路过的结点个数
//#define MAXNODES 100   //最大总节点数
#define P 1000 //可能的通路个数

typedef struct {
  int metrix[MAXNODES][MAXNODES]; //邻接矩阵
  int num;                        //结点数
  int edge;                       //边数
} MGraph;

typedef struct { //结构体
  int data;
  int connect[8];
} Nodes;

void tran(char *topo[5000], int edge_num, int *info[]);
int getmetrix(char *topo[], int edge_num, MGraph G);
void metrix_tran(int *info[], int metrix[][MAXNODES]);
int demandPath(int vdemand[], int path[], int Finallypath[],
               int metrix[][MAXNODES]);
void ReadDemand(char *demand);
void get_line(int result[], int *info[]);
void getfinallypath(int qianqu[], int FinallyPath[]);
void changemetrix(int metrix[][MAXNODES]);
int ford(int metrix[][MAXNODES]);

void function(int start, int metrix[][MAXNODES]); //递归寻找所有通路
void printpath(int path[][MAXNODES]);
void demandPath(int vdemand[], int path[], int meetdemand[][MAXNODES]);
void findshortest(int shortest, int meetdemand[][MAXNODES],
                  int metrix[][MAXNODES], int FinallyPath[]);
// void ReadDemand(char *demand, int vdemand[DEMANDNODES]);
void get_line(int result[], int *info[]);
bool leaveOut(int metrix[][MAXNODES], int s);

=======
>>>>>>> 1013fb085a1bd030ccc4c12bd52a13d2cd053485
#endif
