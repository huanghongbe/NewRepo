#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "Time.h"
#include "Path.h"

#define NULL 0

bool OpenGraph_T(Graph & G);
bool OpenGraph_P(Graph& G);
int  Main_Menu(Graph& GT, Graph& GP);
bool SaveGraph_T(Graph G);
bool SaveGraph_P(Graph G);

int main()
{
	Graph GT;      //火车交通图
	Graph GP;      //飞机交通图  

	CreateGraph(GT); //建立空的火车交通图
	CreateGraph(GP); //建立空的飞机交通图

	OpenGraph_T(GT);  //打开已经存在火车的数据
	OpenGraph_P(GP);  //打开已经存在飞机的数据

	Main_Menu(GT, GP);

	SaveGraph_T(GT);     //保存火车数据
	SaveGraph_P(GP);     //保存飞机数据

	DestoryGraph(GT);
	DestoryGraph(GP);

}

/******************< 迪杰斯特拉算法 求取最少钱数和最短时间 >********************/

void LeastMoneyPath(Graph G, int st, int nd, Path& pathA)
{
	int i;
	int dijkst[MAXVTXNUM];//存储最小花费 dijkst[i]表示起点到i的最小消费 
	bool final[MAXVTXNUM] = { false };
	Path path[MAXVTXNUM]; //存储路径 
	EdgeNode* p, * q;
	EdgeInfo t;          //边的信息
	bool found;
	int min = 9999, min_i, v, w;
	for (i = 0; i < MAXVTXNUM; i++)             //初始化
	{
		dijkst[i] = 9999;
		InitPath(path[i]);//path[i].len=0;
	}
	p = G.Adjlist[st].firstEdge;//指向与起点st相连的第一条边 
	while (p)//初始化dijkst数组，检测依附于起始点的每一条边
	{
		q = p->nextEdge;//st的下一条边 
		if (p->elem.Money < dijkst[p->elem.jvex])//如果该边的金额数值比dijkst数组里存的更小，改变dijkst数组的值 
		{
			dijkst[p->elem.jvex] = p->elem.Money;
			t = p->elem;//边的信息 
			SetPath(path[p->elem.jvex], st, p->elem.jvex, t);//设置边的起点到终点jvex的最短路径 
		}
		p = q;
	}
	found = false;
	while (!found)
	{
		//在所有未求得最短路径的顶点求使得dijkst[i]取最小 的i
		for (i = 0; i < MAXVTXNUM; i++)
		{
			if (final[i] == false && dijkst[i] < min)
				min_i = i;
		}

		final[min_i] = true;

		if (min_i == nd)
			found = true;
		else
		{
			v = min_i;
			p = G.Adjlist[v].firstEdge;
			while (p)
			{
				q = p->nextEdge;
				w = p->elem.jvex;
				if (final[w] == false && ((dijkst[v] + p->elem.Money) < dijkst[w]))
				{
					dijkst[w] = dijkst[v] + p->elem.Money;
					copyPath(path[w], path[v]);
					InsertPath(path[w], v, w, p->elem);
				}
				p = q;
			}
		}
	}

	copyPath(pathA, path[nd]);
}


void ShortestTimePath(Graph G, int st, int nd, int n, Path& pathA)
{
	int i;
	int dijkst[MAXVTXNUM];//dijkst[i]表示原点到i的最小时间间隔 
	bool final[MAXVTXNUM] = { false };
	int now[MAXVTXNUM];
	Path path[MAXVTXNUM];//路径 
	EdgeNode* p, * q;
	EdgeInfo t;
	bool found;
	int min = 99999, min_i, v, w, time;
	for (i = 0; i < MAXVTXNUM; i++)             //初始化
	{
		dijkst[i] = 99999;
		InitPath(path[i]);
		now[i] = 0;
	}
	now[st] = n;

	p = G.Adjlist[st].firstEdge;
	while (p) //初始化dijkst数组，检测依附于起始点的每一条边
	{
		q = p->nextEdge;
		if (p->elem.StartTime < now[st] && p->elem.EndTime >= now[st])
		{
			time = 1440 - TimeSub(p->elem.EndTime, now[st]);//同一天 
		}
		else
			time = TimeSub(p->elem.EndTime, now[st]);//订票第二天出发 


		if (time < dijkst[p->elem.jvex])
		{
			dijkst[p->elem.jvex] = time;
			t = p->elem;
			SetPath(path[p->elem.jvex], st, p->elem.jvex, t);
			now[p->elem.jvex] = p->elem.EndTime;
		}
		p = q;
	}
	found = false;
	while (!found)
	{
		//在所有未求得最短路径的顶点求使得dijkst[i]取最小的i
		for (i = 0; i < MAXVTXNUM; i++)
		{
			if (final[i] == false && dijkst[i] < min)
				min_i = i;
		}

		final[min_i] = true;

		if (min_i == nd)
			found = true;
		else
		{
			v = min_i;
			p = G.Adjlist[v].firstEdge;
			while (p)
			{
				q = p->nextEdge;
				w = p->elem.jvex;

				if (final[w] == false && (dijkst[v] + TimeSub(p->elem.EndTime, now[v]) < dijkst[w]))
				{
					dijkst[w] = dijkst[v] + TimeSub(p->elem.EndTime, now[v]);
					copyPath(path[w], path[v]);
					InsertPath(path[w], v, w, p->elem);
				}
				p = q;
			}
		}
	}
	copyPath(pathA, path[nd]);
}


bool OpenGraph_T(Graph& G)          //打开火车交通图
{
	FILE* fp;
	char c = '\n';
	int i = 0, j = 0;
	if ((fp = fopen("Train.txt", "r")) == NULL)
	{
		return false;
	}
	fscanf(fp, "%d", &G.vexNum); //图的顶点数    //按指定格式从文件中读出数据
	fscanf(fp, "%d", &G.edgeNum); //图的边数

	while (j < G.vexNum)
	{
		fscanf(fp, "%d,%d,%s", &G.Adjlist[i].cityNumber, &G.Flag[i], &G.Adjlist[i].cityName);
		G.Adjlist[i].firstEdge = NULL;
		if (G.Flag[i] == 1)
			j++;
		i++;
	}

	for (i = 0; i < G.edgeNum; i++)
	{
		EdgeNode* p = new EdgeNode;

		fscanf(fp, "%d,%d", &p->elem.ivex, &p->elem.jvex);
		fscanf(fp, "%d,%d,%d,%d", &p->elem.Money, &p->elem.StartTime, &p->elem.EndTime, &p->elem.Time);//在文档中转化为分钟 
		fscanf(fp, "%s", &p->elem.Number);//班次号 

	   //头插法建立起点的邻接表 
		p->nextEdge = G.Adjlist[p->elem.ivex].firstEdge;
		G.Adjlist[p->elem.ivex].firstEdge = p;
	}
	fclose(fp);
	return true;
}

bool OpenGraph_P(Graph& G)//打开飞机交通图
{
	FILE* fp;
	char c = '\n';
	int i = 0, j = 0;
	if ((fp = fopen("Plane.txt", "r")) == NULL)  //打开一个文件  r/w 只　读/写　方式打开一个文本文件
	{                                           //返回文件指针，打开失败返回NULL
		return false;
	}
	fscanf(fp, "%d", &G.vexNum);    //按指定格式从文件中读出数据
	fscanf(fp, "%d", &G.edgeNum);

	while (j < G.vexNum)
	{
		fscanf(fp, "%d,%d,%s", &G.Adjlist[i].cityNumber, &G.Flag[i], &G.Adjlist[i].cityName);
		G.Adjlist[i].firstEdge = NULL;
		if (G.Flag[i] == 1) //Flag==1表示该点为图中的节点 ,否则为弃用的节点 
			j++;
		i++;
	}

	for (i = 0; i < G.edgeNum; i++)//edgeNum表示边的数目 
	{
		EdgeNode* p = new EdgeNode;

		fscanf(fp, "%d,%d", &p->elem.ivex, &p->elem.jvex);
		fscanf(fp, "%d,%d,%d,%d", &p->elem.Money, &p->elem.StartTime, &p->elem.EndTime, &p->elem.Time);
		fscanf(fp, "%s", &p->elem.Number);

		p->nextEdge = G.Adjlist[p->elem.ivex].firstEdge;//头插法 
		G.Adjlist[p->elem.ivex].firstEdge = p;
	}
	fclose(fp);
	return true;
}

bool SaveGraph_T(Graph G)    //保存火车图信息
{
	FILE* fp;
	int i, j = 0;
	char c = '\n';
	EdgeNode* p;

	if ((fp = fopen("Train.txt", "w")) == NULL)
	{
		cout << ("不能建立文件：Train.txt");
		return false;
	}
	fprintf(fp, "%d\n", G.vexNum);//将格式化数据写入流式文件中
	fprintf(fp, "%d\n", G.edgeNum);
	j = 0;
	i = 0;
	while (j < G.vexNum)
	{
		fprintf(fp, "%d,%d,%s%c", i, G.Flag[i], G.Adjlist[i].cityName, c);
		if (G.Flag[i] == 1)
			j++;
		i++;
	}
	i = 0;
	j = 0;
	while (j < G.vexNum)
	{
		p = G.Adjlist[i].firstEdge;
		while (p != NULL)
		{
			fprintf(fp, "%d,%d\n", i, p->elem.jvex);
			fprintf(fp, "%d,%d,%d,%d\n", p->elem.Money, p->elem.StartTime, p->elem.EndTime, p->elem.Time);
			fprintf(fp, "%s%c", p->elem.Number, c);
			p = p->nextEdge;
		}
		if (G.Flag[i] == 1)
			j++;
		i++;

	}
	fclose(fp);
	return true;

}


bool SaveGraph_P(Graph G)         //保存飞机图信息
{
	FILE* fp;
	int i, j = 0;
	char c = '\n';
	EdgeNode* p;

	if ((fp = fopen("Plane.txt", "w")) == NULL)
	{
		cout << "不能建立文件：Plane.txt" << endl;
		return false;
	}
	fprintf(fp, "%d\n", G.vexNum);
	fprintf(fp, "%d\n", G.edgeNum);
	j = 0;
	i = 0;
	while (j < G.vexNum)
	{
		fprintf(fp, "%d,%d,%s%c", i, G.Flag[i], G.Adjlist[i].cityName, c);
		if (G.Flag[i] == 1)
			j++;
		i++;
	}
	i = 0;
	j = 0;
	while (j < G.vexNum)
	{
		p = G.Adjlist[i].firstEdge;
		while (p != NULL)
		{
			fprintf(fp, "%d,%d\n", i, p->elem.jvex);
			fprintf(fp, "%d,%d,%d,%d\n", p->elem.Money, p->elem.StartTime, p->elem.EndTime, p->elem.Time);
			fprintf(fp, "%s%c", p->elem.Number, c);
			p = p->nextEdge;
		}
		if (G.Flag[i] == 1)
			j++;
		i++;

	}
	fclose(fp);
	return true;

}

bool input_Vex(Graph G, int& i)
{
	char name[10];
	cin >> name;

	if (LocateVex(G, name, i) == true)
		return true;
	else
	{
		cout << "该城市不存在！\n";
		return false;
	}
}

bool input_Number(Graph G, int st, int sn, char* number)
{
	cin >> number;
	if (LocateEdge(G, st, sn, number) == true)
		return true;
	else
	{
		cout << "该路线不存在！\n";
		return false;
	}
}


void input_Money(Graph G, int& st, int& sn)
{
	while (1)
	{
		cout << "请输入起始城市名称：";
		if (input_Vex(G, st) == true)
			break;
	}
	while (1)
	{
		cout << "请输入到达城市名称：";
		if (input_Vex(G, sn) == true)
			break;
	}
}

void input_Time(Graph G, int& st, int& sn, int& time)
{
	int hour, minute;
	while (1)
	{
		cout << "请输入起始城市名称：";
		if (input_Vex(G, st) == true)
			break;
	}
	while (1)
	{
		cout << "请输入到达城市名称：";
		if (input_Vex(G, sn) == true)
			break;
	}
	cout << "请输入拟出发时间： " << "\n";
	cout << "几点:";
	cin >> hour;
	cout << "几分:";
	cin >> minute;
	time = TimeChange(hour, minute);
}


void print_Money(Graph GT, Path p)             //最少钱输出
{
	int i;
	int sum = 0;
	cout << "最省钱路线:" << endl << endl;
	for (i = 0; i < p.len; i++)
	{
		cout << "No:   " << i + 1;
		cout << p.edges[i].p.Number;
		cout << "\n\t\t";
		cout << "始发: ";
		cout << "\t" << p.edges[i].p.ivex << "  " << GT.Adjlist[p.edges[i].p.ivex].cityName;
		PrintTime(p.edges[i].p.StartTime);
		cout << "\n\t\t";
		cout << "到达:";
		cout << "\t" << p.edges[i].p.jvex << "  " << GT.Adjlist[p.edges[i].p.jvex].cityName;
		PrintTime(p.edges[i].p.EndTime);
		cout << "票价: " << p.edges[i].p.Money << "元";
		sum += p.edges[i].p.Money;
		cout << "\n\n";
	}
	cout << "总共费用：" << sum << " 元 " << "\n\n";

}

void print_Time(Graph GT, Path p, int now)    //now是用户自定义的查询时间,和求取时候的参数now一致
{

	int i, sum_Time = 0, hour = 0, minute = 0;
	cout << "\n最快到达路线:\n\n";
	for (i = 0; i < p.len; i++)                   //最短时间输出
	{
		cout << "No:  " << i + 1;
		cout << "    " << p.edges[i].p.Number;
		cout << "\n\t\t";
		cout << "始发:";
		cout << "\t" << p.edges[i].p.ivex << ":" << GT.Adjlist[p.edges[i].p.ivex].cityName;
		PrintTime(p.edges[i].p.StartTime);
		cout << "\n\t\t";
		cout << "到达:";
		cout << "\t" << p.edges[i].p.jvex << ":" << GT.Adjlist[p.edges[i].p.jvex].cityName;
		PrintTime(p.edges[i].p.EndTime);
		cout << "票价：" << p.edges[i].p.Money << "  元 ";

		if (p.edges[i].p.StartTime < now && p.edges[i].p.EndTime >= now)
			sum_Time += (1440 + TimeSub(p.edges[i].p.EndTime, now));
		else
			sum_Time += TimeSub(p.edges[i].p.EndTime, now);
		now = p.edges[i].p.EndTime;
		cout << "\n\n";
	}
	hour = sum_Time / 60;
	minute = sum_Time % 60;
	cout << "共需" << hour << "小时" << minute << "分" << "\n\n";

}


//最省钱咨询
void Inquire_Money(Graph GT, Graph GP)
{
	Path p;
	int n = 5;
	int st, nd;
	while (1)
	{
		cout << "\t\t 1.查询飞机";
		cout << "\t 2.查询火车";
		cout << "\t 0.退出\n";
		cout << "\t   请选择：";
		cin >> n;
		fflush(stdin);
		cout << "\n";
		if (n == 1)
		{
			input_Money(GP, st, nd);
			LeastMoneyPath(GP, st, nd, p);   //最少钱数
			print_Money(GP, p);
			break;
		}
		else if (n == 2)
		{
			input_Money(GT, st, nd);
			LeastMoneyPath(GT, st, nd, p);   //最少钱数
			print_Money(GT, p);
			break;
		}
		else if (n == 0)
			break;
		else
			cout << "输入有误，请重新输入！\n\n";
	}
}
//最快到达咨询
void Inquire_Time(Graph GT, Graph GP)
{
	Path p;
	int n = 5;
	int st, nd;
	int time;
	while (1)
	{
		cout << "\t\t 1.查询飞机";
		cout << "\t 2.查询火车";
		cout << "\t 0.退出";
		cout << "\t    请选择：";
		cin >> n;
		//fflush(stdin);
		cout << "\n";
		if (n == 1)
		{
			input_Time(GP, st, nd, time);
			ShortestTimePath(GP, st, nd, time, p);  //最短时间
			print_Time(GP, p, time);
			break;
		}
		else if (n == 2)
		{
			input_Time(GT, st, nd, time);
			ShortestTimePath(GT, st, nd, time, p);  //最短时间
			print_Time(GT, p, time);
			break;
		}
		else if (n == 0)
			break;
		else
			cout << "输入有误，请重新输入！\n\n";
	}

}


//咨询系统菜单
void Inquire_Menu(Graph GT, Graph GP)
{
	while (1)
	{
		int choice = 5;
		cout << "\t********************************************************************" << "\n";
		cout << "\t*        咨询系统菜单                                              *" << "\n";
		cout << "\t*                             1.最省钱咨询                         *" << "\n";
		cout << "\t*                             2.最快到达咨询                       *" << "\n";
		cout << "\t*                             0.退出系统                           *" << "\n";
		cout << "\t********************************************************************" << "\n";
		cout << "           请选择: ";
		cin >> choice;
		fflush(stdin);
		if (choice == 0) break;
		switch (choice)
		{
		case 1: Inquire_Money(GT, GP); break;
		case 2: Inquire_Time(GT, GP); break;
		default:
		{
			cout << "	   输入错误选项,请重试。\n\n";
		}
		}
	}
}

//城市编辑
void City_Edit(Graph& GT, Graph& GP)
{
	int n, i;
	char name[10];
	while (1)
	{
		cout << "\n";
		cout << "\t 1.添加城市      ";
		cout << "\t  2.删除城市      ";
		cout << "\t  0.退出          ";

		cout << "\t\t\t\t 请选择：";
		cin >> n;
		fflush(stdin);
		if (n == 1)
		{
			cout << "\n请输入添加城市的名称：";
			cin >> name;
			InsertVex(GT, name);
			InsertVex(GP, name);
			cout << "\n添加成功！" << "\n";
			break;
		}
		else if (n == 2)
		{
			cout << "\n请输入删除城市的名称：";
			if (input_Vex(GT, i) == true)
			{
				DeleteVex(GT, i);
				DeleteVex(GP, i);
				cout << "删除成功！\n";
			}
			break;
		}
		else if (n == 0)
		{
			cout << "\n";
			break;
		}
		else
			cout << "输入有误，请重新输入！";
	}
}

//列车时刻表编辑 
void Train_Edit(Graph& GT, Graph GP)
{
	int n, st, sn;
	int hour, minute;
	char number[10];
	EdgeInfo q;
	while (1)
	{
		cout << "\n";

		cout << "\t 1.添加列车      ";
		cout << "\t 2.删除列车        ";
		cout << "\t 0.退出            ";
		cout << "\t\t\t 请选择：";
		cin >> n;
		fflush(stdin);
		if (n == 1)
		{
			while (1)
			{
				cout << "\n请输入起始城市的名称：";
				if (input_Vex(GT, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n请输入终点城市的名称：";
				if (input_Vex(GT, sn) == true)
					break;
			}

			cout << "\n请输入车次：";
			cin >> number;
			fflush(stdin);

			q.ivex = st;
			q.jvex = sn;
			strcpy(q.Number, number);

			cout << "\n请输入票价：";
			cin >> q.Money;
			fflush(stdin);

			cout << "\n请输入起始时间：";
			cout << "\n几点: ";
			cin >> hour;
			fflush(stdin);
			cout << "几分: ";
			cin >> minute;
			fflush(stdin);
			q.StartTime = TimeChange(hour, minute);

			cout << "\n请输入到站时间：";
			cout << "\n几点: ";
			cin >> hour;
			fflush(stdin);
			cout << "几分: ";
			cin >> minute;
			fflush(stdin);

			q.EndTime = TimeChange(hour, minute);

			q.Time = TimeSub(q.EndTime, q.StartTime);

			cout << "\n行车时间:" << q.Time / 60 << "小时 " << q.Time % 60 << "分钟";

			InsertEdge(GT, q);

			cout << "\n添加成功！" << "\n";
			break;
		}
		else if (n == 2)
		{
			while (1)
			{
				cout << "\n请输入起始城市的名称：";
				if (input_Vex(GT, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n请输入终点城市的名称：";
				if (input_Vex(GT, sn) == true)
					break;
			}

			cout << "\n请输入车次：";
			if (input_Number(GT, st, sn, number))
			{
				q.ivex = st;
				q.jvex = sn;
				strcpy(q.Number, number);
				DeleteEdge(GT, q);
				cout << "删除成功！\n";
				break;
			}
		}
		else if (n == 0)
		{
			cout << ("\n");
			break;
		}
		else
			cout << "输入有误，请重新输入！";
	}

}

//航班时刻表编辑
void Plane_Edit(Graph GT, Graph& GP)
{
	int n, st, sn;
	int hour, minute;
	char number[10];
	EdgeInfo q;
	while (1)
	{
		cout << ("\n");

		cout << "\t 1.添加航班     ";
		cout << "\t 2.删除航班       ";
		cout << "\t 0.退出           ";
		cout << "\t\t\t 请选择：";
		cin >> n;
		fflush(stdin);
		if (n == 1)
		{
			while (1)
			{
				cout << "\n请输入起始城市的名称：";
				if (input_Vex(GP, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n请输入终点城市的名称：";
				if (input_Vex(GP, sn) == true)
					break;
			}

			cout << "\n请输入航班号：";

			cin >> number;
			fflush(stdin);

			q.ivex = st;
			q.jvex = sn;
			strcpy(q.Number, number);

			cout << "\n请输入票价：";
			cin >> q.Money;
			fflush(stdin);

			cout << "\n请输入起始时间：" << "\n";
			cout << "\n几点: ";
			cin >> hour;
			fflush(stdin);
			cout << "几分: ";
			cin >> minute;
			fflush(stdin);
			q.StartTime = TimeChange(hour, minute);

			cout << "\n请输入到达时间：";
			cout << "\n几点: ";
			cin >> hour;
			fflush(stdin);
			cout << "几分: ";
			cin >> minute;
			fflush(stdin);

			q.EndTime = TimeChange(hour, minute);

			q.Time = TimeSub(q.EndTime, q.StartTime);

			//cout << "\n行车时间:" << q.Time << "分钟";
			cout << "\n行车时间:" << q.Time / 60 << "小时 " << q.Time % 60 << "分钟";

			InsertEdge(GP, q);

			cout << "\n添加成功！" << "\n";
			break;
		}
		else if (n == 2)
		{
			while (1)
			{
				cout << "\n请输入起始城市的名称：";
				if (input_Vex(GT, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n请输入终点城市的名称：";
				if (input_Vex(GT, sn) == true)
					break;
			}

			cout << "\n请输入航班号：";
			if (input_Number(GP, st, sn, number))
			{
				q.ivex = st;
				q.jvex = sn;
				strcpy(q.Number, number);
				DeleteEdge(GP, q);
				cout << "删除成功！\n";
				break;
			}
		}
		else if (n == 0)
		{
			cout << "\n";
			break;
		}
		else
			cout << "输入有误，请重新输入！";
	}

}


void Manage_Menu(Graph& GT, Graph& GP)
{
	while (1)
	{
		int choice = 5;
		cout << "\t********************************************************************" << "\n";
		cout << "\t*                             管理系统菜单                         *" << "\n";
		cout << "\t*                             1.城市编辑                           *" << "\n";
		cout << "\t*                             2.列车时刻表编辑                     *" << "\n";
		cout << "\t*                             3.航班时刻表编辑                     *" << "\n";
		cout << "\t*                             0.退出系统                           *" << "\n";
		cout << "\t********************************************************************" << "\n";
		cout << "           请选择: ";
		cin >> choice;
		fflush(stdin);
		if (choice == 0) break;
		switch (choice)
		{
		case 1: City_Edit(GT, GP); break;
		case 2: Train_Edit(GT, GP); break;
		case 3: Plane_Edit(GT, GP); break;
		default: {
			cout << "	   输入错误选项,请重试。\n\n";
		}
		}
	}
}

int Main_Menu(Graph& GT, Graph& GP)
{
	while (1)
	{
		int choicem = 3;
		cout << ("\n");
		cout << "\t********************************************************************" << "\n";
		cout << "\t*                   欢迎进入全国交通咨询系统主菜单                 *" << "\n";
		cout << "\t*                            1.咨询系统                            *" << "\n";
		cout << "\t*                            2.管理系统                            *" << "\n";
		cout << "\t*                            0.退出系统                            *" << "\n";
		cout << "\t********************************************************************" << "\n";
		cout << "\t\t\t           请选择: ";
		cin >> choicem;
		cout << "\n";
		fflush(stdin);//清除输入流
		switch (choicem)
		{
		case 1: Inquire_Menu(GT, GP); break;
		case 2: Manage_Menu(GT, GP); break;
		case 0: return 0;
		default:
		{
			cout << "	   输入有误,请重试。\n\n";
		}
		}
	}
}


