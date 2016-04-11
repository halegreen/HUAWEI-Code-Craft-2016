#include "route.h"
#include "lib_record.h"
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
void printpath();

int meetdemand[P][MAXNODES];
int start, end;
int visited[MAXNODES]; //走过的路
// void demandPath();
// int path[P][MAXNODES];//已有的通路
int nextline = 0, point = 0; //全局变量
Nodes nodes[MAXNODES];
int FinallyPath[MAXNODES];
int Node_Num = 0;
int shortest = INFINITY_MAX;
int map[MAXNODES][MAXNODES];
int way[MAXNODES][MAXNODES][MAXNODES];
int pass[MAXNODES][MAXNODES][MAXNODES];
int sumpoint;
int vdemand[DEMANDNODES];
int leaveOutNode;

// jmp_buf testjmp;
// int testjmp_num=0;
//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand) {
  MGraph G;
  //-------字符串转换成指针数组--------
  int *info[5000];

  int i, j;
  for (i = 0; i < MAXNODES; i++) {
    FinallyPath[i] = EXCEPTIONNODE;
    visited[i] = EXCEPTIONNODE;
  }

  for (i = 0; i < DEMANDNODES; i++) {
    vdemand[i] = EXCEPTIONNODE;
  }

  for (i = 0; i < MAXNODES; i++) {
    for (j = 0; j < MAXNODES; j++) {
      G.metrix[i][j] = INFINITY_MAX;
    }
  }

  for (i = 0; i < P; i++) {
    for (j = 0; j < MAXNODES; j++) {
      meetdemand[i][j] = EXCEPTIONNODE;
    }
  }

  tran(topo, edge_num, info);

  metrix_tran(info, G.metrix);

  ReadDemand(demand);

  if (Node_Num > 80)
    changemetrix(G.metrix);
  //-----------------------------------------------------------------
  sumpoint = 0;
  for (i = 0; vdemand[i] != EXCEPTIONNODE; i++) {
    sumpoint++;
  }
  // printf("%d\n",sumpoint);
  if (Node_Num < 80) {
    // printf("function\n");
    function(start, G.metrix);
    findshortest(shortest, meetdemand, G.metrix, FinallyPath);
    // printpath(meetdemand);
  } else {
    int finally = ford(G.metrix);
    printf("print 1st done\n");
    if (leaveOut(G.metrix, finally)) {
      printf("fix the matirx done\n");
      ford(G.metrix);
      printf("ford 2nd done\n");
    }
  }

  int PathNum = 0;
  for (i = 0; FinallyPath[i] != EXCEPTIONNODE; i++) {
    PathNum++;
  }
  get_line(FinallyPath, info); //将路径转化为边索引
  for (i = 0; i < PathNum - 1; i++) {
    record_result(FinallyPath[i]);
  }
}

void changemetrix(int metrix[][MAXNODES]) {
  int i, j = 0;
  while (vdemand[j] != EXCEPTIONNODE) {
    for (i = 0; i < Node_Num + 1; i++) {
      if (metrix[vdemand[j]][i] != INFINITY_MAX) {
        metrix[vdemand[j]][i] = metrix[vdemand[j]][i] - 1000;
      }
    }
    j++;
  }
}

/*void getfinallypath(int qianqu[],int FinallyPath[])
{
                int i=1;
                int buff[MAXNODES];
                buff[0]=qianqu[end];
                while(qianqu[buff[i-1]]!=EXCEPTIONNODE)
                {
                        buff[i]=qianqu[FinallyPath[i-1]];
                        i++;
                }
                int j=0;
                while(i!=0)
                {
                        FinallyPath[j++]=buff[i-1];
                        i--;
                }
}*/

void ReadDemand(char *demand) {
  int demand_else[DEMANDNODES + 2];
  for (int i = 0; i < DEMANDNODES + 2; i++) {
    demand_else[i] = EXCEPTIONNODE;
  }
  int a = 0, j;
  int buff[5000];
  int p = 0;
  while ((*(demand + a)) != '\0') {
    j = 0;
    while (((*(demand + a) - 48) >= 0) && ((*(demand + a) - 48) <= 9)) {
      buff[j] = *(demand + a) - 48;
      a++;
      j++;
    }
    int m = 0;
    *(demand_else + p) -= EXCEPTIONNODE;
    while (j != 0) {
      *(demand_else + p) =
          buff[m] * pow(double(10), double(j - 1)) + *(demand_else + p);
      m++;
      j--;
    }
    a++;
    p++;
  }
  // printf("%d %d\n",demand_else[49],demand_else[50]);
  start = demand_else[0];
  end = demand_else[1];
  for (j = 2; demand_else[j] != EXCEPTIONNODE; j++) {
    vdemand[j - 2] = demand_else[j];
  }
}

// char型转int型指针数组
void tran(char *topo[], int edge_num, int *info[]) {
  int i = 0;
  int buff[5000];
  while (i != edge_num) {
    int a = 0;
    int k = 0;
    int j = 0;
    info[i] = (int *)malloc(sizeof(int) * 4);
    while (k < 4) {
      while (((*(topo[i] + a) - 48) >= 0) && ((*(topo[i] + a) - 48) <= 9)) {
        buff[j] = *(topo[i] + a) - 48;
        a++;
        j++;
      }
      int m = 0;
      *(info[i] + k) = 0;
      while (j != 0) {
        *(info[i] + k) =
            buff[m] * pow(double(10), double(j - 1)) + *(info[i] + k);
        m++;
        j--;
      }
      k++;
      a++;
    }
    i++;
  }
  info[edge_num] = 0;
}

// int型数据变换为邻接矩阵
void metrix_tran(int *info[], int metrix[][MAXNODES]) {
  int x, y, i;
  i = 0;

  while (info[i] != NULL) {
    x = *(info[i] + 1);
    y = *(info[i] + 2);
    if (metrix[x][y] > (*(info[i] + 3))) {
      metrix[x][y] = *(info[i] + 3);
    }
    i++;
    if (x > Node_Num)
      Node_Num = x;
    if (y > Node_Num)
      Node_Num = y;
  }
  Node_Num += 1;
}

//------------------------------------------------------
int ford(int metrix[][MAXNODES]) {
  int s, i, j, k, y;

  for (i = 0; i < MAXNODES; i++) {
    FinallyPath[i] = EXCEPTIONNODE;
  }
  for (s = 0; s < Node_Num + 1; s++) {
    for (i = 0; i < Node_Num + 1; i++) {
      map[s][i] = INFINITY_MAX;
      for (j = 0; j < Node_Num + 1; j++) {
        way[s][i][j] = EXCEPTIONNODE;
        pass[s][i][j] = 0;
      }
    }
  }
  for (s = 0; s < Node_Num + 1; s++) {
    map[0][s] = metrix[start][s];
    if (metrix[start][s] != INFINITY_MAX) {
      way[0][s][0] = s;
      pass[0][s][s] = 1;
    }
  }

  for (s = 1; s < Node_Num; s++) {
    for (j = 0; j < Node_Num; j++) {
      for (k = 0; k < Node_Num; k++) {
        if (pass[s - 1][k][j] != 1) {
          if ((map[s][j] > map[s - 1][k] + metrix[k][j]) &&
              (map[s - 1][k] != INFINITY_MAX) &&
              (metrix[k][j] != INFINITY_MAX)) {

            map[s][j] = map[s - 1][k] + metrix[k][j];
            memcpy(way[s][j], way[s - 1][k], s * 8);
            way[s][j][s] = j;
            memcpy(pass[s][j], pass[s - 1][k], Node_Num * 8);
            pass[s][j][j] = 1;
          }
        }
      }
    }
  }
  /*for(i=0;i<200;i++)
  {
          printf("%d	",map[i][end]);
  }
  printf("\n");

  printf("%d ",map[109][end]);
  printf("\n");*/

  /*for(s=0;s<Node_Num+1;s++)
  {
                  k=0;
                  j=0;
                  for(i=0;vdemand[i]!=EXCEPTIONNODE;i++)
                  {
                                  for(y=0;way[s][end][y]!=EXCEPTIONNODE;y++)
                                  {
                                          if(vdemand[i]==way[s][end][y]){
                                          k++;
                                          break;}
                                  }
                                  if(k==j){break;}
                                  j=k;
                  }
                  if(j==sumpoint){
                  break;}
  }*/

  i = 0;
  for (y = 0; y < Node_Num; y++) {
    if (map[y][end] < i) {
      i = map[y][end];
      s = y;
    }
  }

  FinallyPath[0] = start;
  printf("%d %d   \n", s, map[s][end]);

  for (i = 1; way[s][end][i - 1] != EXCEPTIONNODE; i++) {
    FinallyPath[i] = way[s][end][i - 1];
  }
  return s;
}

void get_line(int result[], int *info[]) {
  int i = 0;
  int j = 0;
  while (result[j + 1] != EXCEPTIONNODE) {
    while ((*(info[i] + 1) == result[j]) && (*(info[i] + 2) == result[j + 1])) {
      result[j] = *(info[i]);
      j++;
      i = 0;
    }
    i++;
  }
}

//输出路径
void printpath(int path[][MAXNODES]) {
  int pp, qq;
  for (pp = 0; pp < P; pp++) {
    for (qq = 0; path[pp][qq] < EXCEPTIONNODE; qq++) {
      printf("%d    ", path[pp][qq]);
    }
    if (path[pp][0] != EXCEPTIONNODE)
      printf("\n");
  }
}

//递归寻路函数
void function(int start, int metrix[][MAXNODES]) {
  // printf("start=%d\n", start);
  bool flag1 = 0;
  int j = 0, i = 0;
  visited[point++] = start;              //将该点加入到不可用点集
  for (i = 0; i < Node_Num; i++)         //找到和其直接相关的点
    if (metrix[start][i] < INFINITY_MAX) //如果有直接相关的点，则做下标记
    {
      flag1 = 1;
      nodes[start].connect[j++] = i;
      nodes[start].connect[j] = EXCEPTIONNODE;
    }
  if (!flag1) //如果没有直接相连的点，则返回上一层
  {
    point--;
    return;
  }
  for (j = 0; nodes[start].connect[j] != EXCEPTIONNODE;
       j++) { //找出一个直接相连的点
    int flag2 = 0;
    if (nodes[start].connect[j] == end) //为终点，将此路径放入通路集合中
    {
      demandPath(vdemand, visited, meetdemand);
      continue;
    }
    for (i = 0; visited[i] != EXCEPTIONNODE;
         i++) //判断是否使用过，使用过就要返回上层
      if (visited[i] == nodes[start].connect[j]) {
        flag2 = 1;
        break;
      }
    if (!flag2)
      function(nodes[start].connect[j], metrix); //深入递归
  }
  point--;                        //将路过的路的指针退一个
  visited[point] = EXCEPTIONNODE; //恢复初值
}

//找符合要求的路径
void demandPath(int vdemand[], int visited[], int meetdemand[][MAXNODES]) {
  bool flag3 = 0, flag4 = 0;
  int i, qq;
  for (i = 0; vdemand[i] != EXCEPTIONNODE; i++) {
    for (qq = 0; visited[qq] < EXCEPTIONNODE; qq++) {
      flag3 = 0;
      if (vdemand[i] == visited[qq])
        break; //如果在该路径中所有需要的点都存在，则flag3都为0；若有一个需要的点不在路径中，则flag3为1
      flag3 = 1;
    }
    if (flag3) {
      flag4 = 1;
      break;
    } //只要有一个需要的点不在路径中，则判断下一条路径
  }
  if (!flag4) { //如果需要的点都在，则将此路径存入满足要求的路径集合中
    for (qq = 0; visited[qq] < EXCEPTIONNODE; qq++)
      meetdemand[nextline][qq] = visited[qq];
    meetdemand[nextline][qq] = end;
    nextline++;
  }
}

void findshortest(int shortest, int meetdemand[][MAXNODES],
                  int metrix[][MAXNODES], int FinallyPath[]) {
  int temp = 0, shortestpath = 0, pp = 0, qq;
  shortest = INFINITY_MAX;
  for (pp = 0; meetdemand[pp][0] != EXCEPTIONNODE; pp++) {
    temp = 0;
    for (qq = 1; meetdemand[pp][qq] != EXCEPTIONNODE; qq++)
      temp = temp + metrix[meetdemand[pp][qq - 1]][meetdemand[pp][qq]];
    if (temp < shortest) {
      shortest = temp;
      shortestpath = pp;
    }
  }
  //	printf("最终最短距离:%d\n",shortest);
  //	printf("最终最短路径为：");
  //	for(qq=0;meetdemand[shortestpath][qq]!=EXCEPTIONNODE;qq++)
  //		printf("%d  ",meetdemand[shortestpath][qq]);
  //	printf("\n\n\n");

  for (qq = 0; meetdemand[shortestpath][qq] != EXCEPTIONNODE; qq++)
    FinallyPath[qq] = meetdemand[shortestpath][qq];
}

bool leaveOut(int metrix[][MAXNODES], int s) {
  int i, j;
  bool flag1 = 0;
  leaveOutNode = EXCEPTIONNODE;
  for (i = 0; vdemand[i] != EXCEPTIONNODE; i++) {
    for (j = 0; way[s][end][j] != EXCEPTIONNODE; j++) {
      if (vdemand[i] == way[s][end][j])
        break;
      else {
        leaveOutNode = vdemand[i];
        flag1 = 1;
      }
    }
  }
  if (flag1) {
    for (i = 0; i < Node_Num; i++) {
      if (metrix[leaveOutNode][i] != EXCEPTIONNODE)
        metrix[leaveOutNode][i] -= 5000;
    }
  }
  return flag1;
}
