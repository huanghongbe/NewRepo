#pragma once

/**************************** ·������ ********************************/
typedef struct
{
	int vx, vy;  //vxΪ·�������,vyΪ·�����յ�
	EdgeInfo p;  //·���бߵ���Ϣ
}Edge;

typedef struct
{
	Edge edges[MAXVTXNUM];   //·���бߵ�����
	int len;                 //·���бߵ���Ŀ
}Path;//path[j] 
void InitPath(Path& pa)
{
	//��ʼ��paΪһ����·����pa.len=0��
	pa.len = 0;
}

void copyPath(Path& p1, Path& p2)
{
	//����·��p1=p2
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
	//��·��pa�в���һ���ߣ�v, w��,�ߵ���ϢΪt
	pa.edges[pa.len].vx = v;
	pa.edges[pa.len].vy = w;
	pa.edges[pa.len].p = t;
	pa.len++;

}
void SetPath(Path& pa, int v, int w, EdgeInfo t)
{
	//����pa��v��w�ĵ�һ����,�ߵ���ϢΪt 
	pa.edges[0].vx = v;//��� 
	pa.edges[0].vy = w;//�յ� 
	pa.edges[0].p = t;
	pa.len = 1;
}