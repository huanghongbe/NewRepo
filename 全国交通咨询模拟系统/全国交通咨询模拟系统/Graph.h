#pragma once
#define MAXVTXNUM 30      //图中顶点数的最大值

/******************************顶点、边和图类型**********************************/
typedef struct         //定义各车次及航班的信息　　　弧的信息
{
	int  ivex;			    //起始点号	
	int  jvex;			    //终点号
	char Number[10];        // 车次号
	int  Money;             //费用
	int  StartTime;         //起始时间（秒）
	int  EndTime;           //终止时间（秒）
	int  Time;              //中途时间（秒）
}EdgeInfo;  				//边的信息

typedef struct  EdgeNode   //边的信息     弧结点
{
	EdgeInfo  elem;
	EdgeNode* nextEdge;
}EdgeNode, * EdgePtr;		//边的结点类型，指向边的指针


typedef struct    //城市信息   头结点
{
	char cityName[10];
	int cityNumber;
	EdgePtr firstEdge;   //指向的一条依附该顶点的边的指针
}Vnode;                      //顶点类型


typedef struct       //图的结构
{
	Vnode Adjlist[MAXVTXNUM];//邻接表
	int vexNum, edgeNum;     //图中的顶点数和边数
	int Flag[MAXVTXNUM];     //标志图中顶点 
}Graph;                  //图类型



/***************************************图的基本操作**********************/
void CreateGraph(Graph& G)
{
	//建立空图,初始化
	int i;
	G.edgeNum = 0;
	G.vexNum = 0;
	for (i = 0; i < MAXVTXNUM; i++)
		G.Flag[i] = 0;
}

bool LocateVex(Graph G, char* name, int& i)
{
	//在图中查找其顶点名称和name相同的顶点。若存在则以i返回其在邻接表中的位置
	//如果有返回true,则i为位置，如果没有返回false

	i = 0;
	while (i < MAXVTXNUM)
	{
		if (G.Flag[i] == 1 && strcmp(G.Adjlist[i].cityName, name) == 0)
		{
			return true;
		}
		i++;
	}

	return false;
}

bool LocateEdge(Graph G, int st, int sn, char* number)
{
	//在图中查找起点为st,终点为sn,车次号为number是否存在
	//如果有返回true,如果没有返回false
	EdgeNode* p;
	p = G.Adjlist[st].firstEdge;
	while (p)
	{
		if (p->elem.jvex == sn && strcmp(p->elem.Number, number) == 0)
			break;
		p = p->nextEdge;
	}
	if (p)
		return true;
	else
		return false;
}

void InsertVex(Graph& G, char* name)
{
	//	初始条件：图G存在，v和途中顶点有相同特征
	//	操作结果：在图G中添加新顶点v

	int i;
	for (i = 0; i < MAXVTXNUM; i++)
	{
		if (G.Flag[i] != 1)
			break;
	}
	G.Adjlist[i].cityNumber = i;
	strcpy(G.Adjlist[i].cityName, name);
	G.Adjlist[i].firstEdge = NULL;
	G.Flag[i] = 1;
	G.vexNum++;
}

void DeleteVex(Graph& G, int ivex)
{
	//删除G中顶点v及其相关的边
	int i;
	EdgeNode* pt, * p;

	//把所有的ivex出度的路径删除
	pt = G.Adjlist[ivex].firstEdge;

	while (pt)
	{
		p = pt->nextEdge;
		delete pt;
		pt = p;
		G.edgeNum--;
	}
	pt = G.Adjlist[ivex].firstEdge = NULL;

	//把点ivex标记为不使用
	G.Flag[ivex] = 0;
	G.vexNum--;

	//把所有的ivex入度的路径删除
	for (i = 0; i < MAXVTXNUM; i++)
	{
		if (G.Flag[i] == 1)
		{
			p = G.Adjlist[i].firstEdge;      //先求每个点的第一条边
			pt = p;                          //pt指向每个顶点的第一条边
			while (p)
			{
				if (p->elem.jvex == ivex)     //判断此边的终点是否是ivex   即到ivex的边
				{
					if (p == G.Adjlist[i].firstEdge)//如果是　判断是否是它的第一条边
					{
						G.Adjlist[i].firstEdge = p->nextEdge;//修改邻接表的结构
						pt = G.Adjlist[i].firstEdge;
					}
					else
						pt->nextEdge = p->nextEdge;
					delete p;
					p = pt;
					G.edgeNum--;
				}
				pt = p;
				if (p)
					p = p->nextEdge;
			}
		}
	}

}

void InsertEdge(Graph& G, EdgeInfo q)//起始站，终点站的编号
{
	//初始条件： 图G存在，v和w是G中两个顶点
	//操作结果： 在G中添加边 (v, w)

	EdgeNode* p = new EdgeNode;

	p->elem.ivex = q.ivex;
	p->elem.jvex = q.jvex;
	strcpy(p->elem.Number, q.Number);
	p->elem.StartTime = q.StartTime;
	p->elem.EndTime = q.EndTime;
	p->elem.Time = q.Time;
	p->elem.Money = q.Money;
	p->nextEdge = G.Adjlist[q.ivex].firstEdge;
	G.Adjlist[q.ivex].firstEdge = p;

	G.edgeNum++;
}

void DeleteEdge(Graph& G, EdgeInfo q)
{	//在图中删除边
	EdgeNode* p, * pt;

	p = G.Adjlist[q.ivex].firstEdge;
	pt = p;

	while (p)
	{
		if (p->elem.jvex == q.jvex && strcmp(p->elem.Number, q.Number) == 0)
		{
			break;
		}
		pt = p;
		p = p->nextEdge;
	}
	if (p == G.Adjlist[q.ivex].firstEdge)
		G.Adjlist[q.ivex].firstEdge = p->nextEdge;
	else
		pt->nextEdge = p->nextEdge;
	delete p;
	G.edgeNum--;
}

bool DestoryGraph(Graph& G)
{
	//销毁图结构
	int i;
	for (i = 0; i < MAXVTXNUM; i++)
	{
		if (G.Flag[i] == 1)
			DeleteVex(G, i);
	}
	return true;
}



