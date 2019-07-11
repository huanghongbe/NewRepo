#pragma once
#define MAXVTXNUM 30      //ͼ�ж����������ֵ

/******************************���㡢�ߺ�ͼ����**********************************/
typedef struct         //��������μ��������Ϣ������������Ϣ
{
	int  ivex;			    //��ʼ���	
	int  jvex;			    //�յ��
	char Number[10];        // ���κ�
	int  Money;             //����
	int  StartTime;         //��ʼʱ�䣨�룩
	int  EndTime;           //��ֹʱ�䣨�룩
	int  Time;              //��;ʱ�䣨�룩
}EdgeInfo;  				//�ߵ���Ϣ

typedef struct  EdgeNode   //�ߵ���Ϣ     �����
{
	EdgeInfo  elem;
	EdgeNode* nextEdge;
}EdgeNode, * EdgePtr;		//�ߵĽ�����ͣ�ָ��ߵ�ָ��


typedef struct    //������Ϣ   ͷ���
{
	char cityName[10];
	int cityNumber;
	EdgePtr firstEdge;   //ָ���һ�������ö���ıߵ�ָ��
}Vnode;                      //��������


typedef struct       //ͼ�Ľṹ
{
	Vnode Adjlist[MAXVTXNUM];//�ڽӱ�
	int vexNum, edgeNum;     //ͼ�еĶ������ͱ���
	int Flag[MAXVTXNUM];     //��־ͼ�ж��� 
}Graph;                  //ͼ����



/***************************************ͼ�Ļ�������**********************/
void CreateGraph(Graph& G)
{
	//������ͼ,��ʼ��
	int i;
	G.edgeNum = 0;
	G.vexNum = 0;
	for (i = 0; i < MAXVTXNUM; i++)
		G.Flag[i] = 0;
}

bool LocateVex(Graph G, char* name, int& i)
{
	//��ͼ�в����䶥�����ƺ�name��ͬ�Ķ��㡣����������i���������ڽӱ��е�λ��
	//����з���true,��iΪλ�ã����û�з���false

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
	//��ͼ�в������Ϊst,�յ�Ϊsn,���κ�Ϊnumber�Ƿ����
	//����з���true,���û�з���false
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
	//	��ʼ������ͼG���ڣ�v��;�ж�������ͬ����
	//	�����������ͼG������¶���v

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
	//ɾ��G�ж���v������صı�
	int i;
	EdgeNode* pt, * p;

	//�����е�ivex���ȵ�·��ɾ��
	pt = G.Adjlist[ivex].firstEdge;

	while (pt)
	{
		p = pt->nextEdge;
		delete pt;
		pt = p;
		G.edgeNum--;
	}
	pt = G.Adjlist[ivex].firstEdge = NULL;

	//�ѵ�ivex���Ϊ��ʹ��
	G.Flag[ivex] = 0;
	G.vexNum--;

	//�����е�ivex��ȵ�·��ɾ��
	for (i = 0; i < MAXVTXNUM; i++)
	{
		if (G.Flag[i] == 1)
		{
			p = G.Adjlist[i].firstEdge;      //����ÿ����ĵ�һ����
			pt = p;                          //ptָ��ÿ������ĵ�һ����
			while (p)
			{
				if (p->elem.jvex == ivex)     //�жϴ˱ߵ��յ��Ƿ���ivex   ����ivex�ı�
				{
					if (p == G.Adjlist[i].firstEdge)//����ǡ��ж��Ƿ������ĵ�һ����
					{
						G.Adjlist[i].firstEdge = p->nextEdge;//�޸��ڽӱ�Ľṹ
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

void InsertEdge(Graph& G, EdgeInfo q)//��ʼվ���յ�վ�ı��
{
	//��ʼ������ ͼG���ڣ�v��w��G����������
	//��������� ��G����ӱ� (v, w)

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
{	//��ͼ��ɾ����
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
	//����ͼ�ṹ
	int i;
	for (i = 0; i < MAXVTXNUM; i++)
	{
		if (G.Flag[i] == 1)
			DeleteVex(G, i);
	}
	return true;
}



