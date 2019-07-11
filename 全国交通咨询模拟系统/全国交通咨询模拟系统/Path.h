#pragma once

/**************************** 路径类型 ********************************/
typedef struct
{
	int vx, vy;  //vx为路径的起点,vy为路径的终点
	EdgeInfo p;  //路径中边的信息
}Edge;

typedef struct
{
	Edge edges[MAXVTXNUM];   //路径中边的序列
	int len;                 //路径中边的数目
}Path;//path[j] 
void InitPath(Path& pa)
{
	//初始化pa为一条空路径（pa.len=0）
	pa.len = 0;
}

void copyPath(Path& p1, Path& p2)
{
	//复制路径p1=p2
	int i;
	for (i = 0; i < p2.len; i++)
	{
		p1.edges[i].vx = p2.edges[i].vx;
		p1.edges[i].vy = p2.edges[i].vy;
		p1.edges[i].p = p2.edges[i].p;
	}
	p1.len = p2.len;
}

void InsertPath(Path& pa, int v, int w, EdgeInfo t)
{
	//在路径pa中插入一条边（v, w）,边的信息为t
	pa.edges[pa.len].vx = v;
	pa.edges[pa.len].vy = w;
	pa.edges[pa.len].p = t;
	pa.len++;

}
void SetPath(Path& pa, int v, int w, EdgeInfo t)
{
	//设置pa从v到w的第一条边,边的信息为t 
	pa.edges[0].vx = v;//起点 
	pa.edges[0].vy = w;//终点 
	pa.edges[0].p = t;
	pa.len = 1;
}