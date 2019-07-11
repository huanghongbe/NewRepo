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
	Graph GT;      //�𳵽�ͨͼ
	Graph GP;      //�ɻ���ͨͼ  

	CreateGraph(GT); //�����յĻ𳵽�ͨͼ
	CreateGraph(GP); //�����յķɻ���ͨͼ

	OpenGraph_T(GT);  //���Ѿ����ڻ𳵵�����
	OpenGraph_P(GP);  //���Ѿ����ڷɻ�������

	Main_Menu(GT, GP);

	SaveGraph_T(GT);     //���������
	SaveGraph_P(GP);     //����ɻ�����

	DestoryGraph(GT);
	DestoryGraph(GP);

}

/******************< �Ͻ�˹�����㷨 ��ȡ����Ǯ�������ʱ�� >********************/

void LeastMoneyPath(Graph G, int st, int nd, Path& pathA)
{
	int i;
	int dijkst[MAXVTXNUM];//�洢��С���� dijkst[i]��ʾ��㵽i����С���� 
	bool final[MAXVTXNUM] = { false };
	Path path[MAXVTXNUM]; //�洢·�� 
	EdgeNode* p, * q;
	EdgeInfo t;          //�ߵ���Ϣ
	bool found;
	int min = 9999, min_i, v, w;
	for (i = 0; i < MAXVTXNUM; i++)             //��ʼ��
	{
		dijkst[i] = 9999;
		InitPath(path[i]);//path[i].len=0;
	}
	p = G.Adjlist[st].firstEdge;//ָ�������st�����ĵ�һ���� 
	while (p)//��ʼ��dijkst���飬�����������ʼ���ÿһ����
	{
		q = p->nextEdge;//st����һ���� 
		if (p->elem.Money < dijkst[p->elem.jvex])//����ñߵĽ����ֵ��dijkst�������ĸ�С���ı�dijkst�����ֵ 
		{
			dijkst[p->elem.jvex] = p->elem.Money;
			t = p->elem;//�ߵ���Ϣ 
			SetPath(path[p->elem.jvex], st, p->elem.jvex, t);//���ñߵ���㵽�յ�jvex�����·�� 
		}
		p = q;
	}
	found = false;
	while (!found)
	{
		//������δ������·���Ķ�����ʹ��dijkst[i]ȡ��С ��i
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
	int dijkst[MAXVTXNUM];//dijkst[i]��ʾԭ�㵽i����Сʱ���� 
	bool final[MAXVTXNUM] = { false };
	int now[MAXVTXNUM];
	Path path[MAXVTXNUM];//·�� 
	EdgeNode* p, * q;
	EdgeInfo t;
	bool found;
	int min = 99999, min_i, v, w, time;
	for (i = 0; i < MAXVTXNUM; i++)             //��ʼ��
	{
		dijkst[i] = 99999;
		InitPath(path[i]);
		now[i] = 0;
	}
	now[st] = n;

	p = G.Adjlist[st].firstEdge;
	while (p) //��ʼ��dijkst���飬�����������ʼ���ÿһ����
	{
		q = p->nextEdge;
		if (p->elem.StartTime < now[st] && p->elem.EndTime >= now[st])
		{
			time = 1440 - TimeSub(p->elem.EndTime, now[st]);//ͬһ�� 
		}
		else
			time = TimeSub(p->elem.EndTime, now[st]);//��Ʊ�ڶ������ 


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
		//������δ������·���Ķ�����ʹ��dijkst[i]ȡ��С��i
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


bool OpenGraph_T(Graph& G)          //�򿪻𳵽�ͨͼ
{
	FILE* fp;
	char c = '\n';
	int i = 0, j = 0;
	if ((fp = fopen("Train.txt", "r")) == NULL)
	{
		return false;
	}
	fscanf(fp, "%d", &G.vexNum); //ͼ�Ķ�����    //��ָ����ʽ���ļ��ж�������
	fscanf(fp, "%d", &G.edgeNum); //ͼ�ı���

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
		fscanf(fp, "%d,%d,%d,%d", &p->elem.Money, &p->elem.StartTime, &p->elem.EndTime, &p->elem.Time);//���ĵ���ת��Ϊ���� 
		fscanf(fp, "%s", &p->elem.Number);//��κ� 

	   //ͷ�巨���������ڽӱ� 
		p->nextEdge = G.Adjlist[p->elem.ivex].firstEdge;
		G.Adjlist[p->elem.ivex].firstEdge = p;
	}
	fclose(fp);
	return true;
}

bool OpenGraph_P(Graph& G)//�򿪷ɻ���ͨͼ
{
	FILE* fp;
	char c = '\n';
	int i = 0, j = 0;
	if ((fp = fopen("Plane.txt", "r")) == NULL)  //��һ���ļ�  r/w ֻ����/д����ʽ��һ���ı��ļ�
	{                                           //�����ļ�ָ�룬��ʧ�ܷ���NULL
		return false;
	}
	fscanf(fp, "%d", &G.vexNum);    //��ָ����ʽ���ļ��ж�������
	fscanf(fp, "%d", &G.edgeNum);

	while (j < G.vexNum)
	{
		fscanf(fp, "%d,%d,%s", &G.Adjlist[i].cityNumber, &G.Flag[i], &G.Adjlist[i].cityName);
		G.Adjlist[i].firstEdge = NULL;
		if (G.Flag[i] == 1) //Flag==1��ʾ�õ�Ϊͼ�еĽڵ� ,����Ϊ���õĽڵ� 
			j++;
		i++;
	}

	for (i = 0; i < G.edgeNum; i++)//edgeNum��ʾ�ߵ���Ŀ 
	{
		EdgeNode* p = new EdgeNode;

		fscanf(fp, "%d,%d", &p->elem.ivex, &p->elem.jvex);
		fscanf(fp, "%d,%d,%d,%d", &p->elem.Money, &p->elem.StartTime, &p->elem.EndTime, &p->elem.Time);
		fscanf(fp, "%s", &p->elem.Number);

		p->nextEdge = G.Adjlist[p->elem.ivex].firstEdge;//ͷ�巨 
		G.Adjlist[p->elem.ivex].firstEdge = p;
	}
	fclose(fp);
	return true;
}

bool SaveGraph_T(Graph G)    //�����ͼ��Ϣ
{
	FILE* fp;
	int i, j = 0;
	char c = '\n';
	EdgeNode* p;

	if ((fp = fopen("Train.txt", "w")) == NULL)
	{
		cout << ("���ܽ����ļ���Train.txt");
		return false;
	}
	fprintf(fp, "%d\n", G.vexNum);//����ʽ������д����ʽ�ļ���
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


bool SaveGraph_P(Graph G)         //����ɻ�ͼ��Ϣ
{
	FILE* fp;
	int i, j = 0;
	char c = '\n';
	EdgeNode* p;

	if ((fp = fopen("Plane.txt", "w")) == NULL)
	{
		cout << "���ܽ����ļ���Plane.txt" << endl;
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
		cout << "�ó��в����ڣ�\n";
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
		cout << "��·�߲����ڣ�\n";
		return false;
	}
}


void input_Money(Graph G, int& st, int& sn)
{
	while (1)
	{
		cout << "��������ʼ�������ƣ�";
		if (input_Vex(G, st) == true)
			break;
	}
	while (1)
	{
		cout << "�����뵽��������ƣ�";
		if (input_Vex(G, sn) == true)
			break;
	}
}

void input_Time(Graph G, int& st, int& sn, int& time)
{
	int hour, minute;
	while (1)
	{
		cout << "��������ʼ�������ƣ�";
		if (input_Vex(G, st) == true)
			break;
	}
	while (1)
	{
		cout << "�����뵽��������ƣ�";
		if (input_Vex(G, sn) == true)
			break;
	}
	cout << "�����������ʱ�䣺 " << "\n";
	cout << "����:";
	cin >> hour;
	cout << "����:";
	cin >> minute;
	time = TimeChange(hour, minute);
}


void print_Money(Graph GT, Path p)             //����Ǯ���
{
	int i;
	int sum = 0;
	cout << "��ʡǮ·��:" << endl << endl;
	for (i = 0; i < p.len; i++)
	{
		cout << "No:   " << i + 1;
		cout << p.edges[i].p.Number;
		cout << "\n\t\t";
		cout << "ʼ��: ";
		cout << "\t" << p.edges[i].p.ivex << "  " << GT.Adjlist[p.edges[i].p.ivex].cityName;
		PrintTime(p.edges[i].p.StartTime);
		cout << "\n\t\t";
		cout << "����:";
		cout << "\t" << p.edges[i].p.jvex << "  " << GT.Adjlist[p.edges[i].p.jvex].cityName;
		PrintTime(p.edges[i].p.EndTime);
		cout << "Ʊ��: " << p.edges[i].p.Money << "Ԫ";
		sum += p.edges[i].p.Money;
		cout << "\n\n";
	}
	cout << "�ܹ����ã�" << sum << " Ԫ " << "\n\n";

}

void print_Time(Graph GT, Path p, int now)    //now���û��Զ���Ĳ�ѯʱ��,����ȡʱ��Ĳ���nowһ��
{

	int i, sum_Time = 0, hour = 0, minute = 0;
	cout << "\n��쵽��·��:\n\n";
	for (i = 0; i < p.len; i++)                   //���ʱ�����
	{
		cout << "No:  " << i + 1;
		cout << "    " << p.edges[i].p.Number;
		cout << "\n\t\t";
		cout << "ʼ��:";
		cout << "\t" << p.edges[i].p.ivex << ":" << GT.Adjlist[p.edges[i].p.ivex].cityName;
		PrintTime(p.edges[i].p.StartTime);
		cout << "\n\t\t";
		cout << "����:";
		cout << "\t" << p.edges[i].p.jvex << ":" << GT.Adjlist[p.edges[i].p.jvex].cityName;
		PrintTime(p.edges[i].p.EndTime);
		cout << "Ʊ�ۣ�" << p.edges[i].p.Money << "  Ԫ ";

		if (p.edges[i].p.StartTime < now && p.edges[i].p.EndTime >= now)
			sum_Time += (1440 + TimeSub(p.edges[i].p.EndTime, now));
		else
			sum_Time += TimeSub(p.edges[i].p.EndTime, now);
		now = p.edges[i].p.EndTime;
		cout << "\n\n";
	}
	hour = sum_Time / 60;
	minute = sum_Time % 60;
	cout << "����" << hour << "Сʱ" << minute << "��" << "\n\n";

}


//��ʡǮ��ѯ
void Inquire_Money(Graph GT, Graph GP)
{
	Path p;
	int n = 5;
	int st, nd;
	while (1)
	{
		cout << "\t\t 1.��ѯ�ɻ�";
		cout << "\t 2.��ѯ��";
		cout << "\t 0.�˳�\n";
		cout << "\t   ��ѡ��";
		cin >> n;
		fflush(stdin);
		cout << "\n";
		if (n == 1)
		{
			input_Money(GP, st, nd);
			LeastMoneyPath(GP, st, nd, p);   //����Ǯ��
			print_Money(GP, p);
			break;
		}
		else if (n == 2)
		{
			input_Money(GT, st, nd);
			LeastMoneyPath(GT, st, nd, p);   //����Ǯ��
			print_Money(GT, p);
			break;
		}
		else if (n == 0)
			break;
		else
			cout << "�����������������룡\n\n";
	}
}
//��쵽����ѯ
void Inquire_Time(Graph GT, Graph GP)
{
	Path p;
	int n = 5;
	int st, nd;
	int time;
	while (1)
	{
		cout << "\t\t 1.��ѯ�ɻ�";
		cout << "\t 2.��ѯ��";
		cout << "\t 0.�˳�";
		cout << "\t    ��ѡ��";
		cin >> n;
		//fflush(stdin);
		cout << "\n";
		if (n == 1)
		{
			input_Time(GP, st, nd, time);
			ShortestTimePath(GP, st, nd, time, p);  //���ʱ��
			print_Time(GP, p, time);
			break;
		}
		else if (n == 2)
		{
			input_Time(GT, st, nd, time);
			ShortestTimePath(GT, st, nd, time, p);  //���ʱ��
			print_Time(GT, p, time);
			break;
		}
		else if (n == 0)
			break;
		else
			cout << "�����������������룡\n\n";
	}

}


//��ѯϵͳ�˵�
void Inquire_Menu(Graph GT, Graph GP)
{
	while (1)
	{
		int choice = 5;
		cout << "\t********************************************************************" << "\n";
		cout << "\t*        ��ѯϵͳ�˵�                                              *" << "\n";
		cout << "\t*                             1.��ʡǮ��ѯ                         *" << "\n";
		cout << "\t*                             2.��쵽����ѯ                       *" << "\n";
		cout << "\t*                             0.�˳�ϵͳ                           *" << "\n";
		cout << "\t********************************************************************" << "\n";
		cout << "           ��ѡ��: ";
		cin >> choice;
		fflush(stdin);
		if (choice == 0) break;
		switch (choice)
		{
		case 1: Inquire_Money(GT, GP); break;
		case 2: Inquire_Time(GT, GP); break;
		default:
		{
			cout << "	   �������ѡ��,�����ԡ�\n\n";
		}
		}
	}
}

//���б༭
void City_Edit(Graph& GT, Graph& GP)
{
	int n, i;
	char name[10];
	while (1)
	{
		cout << "\n";
		cout << "\t 1.��ӳ���      ";
		cout << "\t  2.ɾ������      ";
		cout << "\t  0.�˳�          ";

		cout << "\t\t\t\t ��ѡ��";
		cin >> n;
		fflush(stdin);
		if (n == 1)
		{
			cout << "\n��������ӳ��е����ƣ�";
			cin >> name;
			InsertVex(GT, name);
			InsertVex(GP, name);
			cout << "\n��ӳɹ���" << "\n";
			break;
		}
		else if (n == 2)
		{
			cout << "\n������ɾ�����е����ƣ�";
			if (input_Vex(GT, i) == true)
			{
				DeleteVex(GT, i);
				DeleteVex(GP, i);
				cout << "ɾ���ɹ���\n";
			}
			break;
		}
		else if (n == 0)
		{
			cout << "\n";
			break;
		}
		else
			cout << "�����������������룡";
	}
}

//�г�ʱ�̱�༭ 
void Train_Edit(Graph& GT, Graph GP)
{
	int n, st, sn;
	int hour, minute;
	char number[10];
	EdgeInfo q;
	while (1)
	{
		cout << "\n";

		cout << "\t 1.����г�      ";
		cout << "\t 2.ɾ���г�        ";
		cout << "\t 0.�˳�            ";
		cout << "\t\t\t ��ѡ��";
		cin >> n;
		fflush(stdin);
		if (n == 1)
		{
			while (1)
			{
				cout << "\n��������ʼ���е����ƣ�";
				if (input_Vex(GT, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n�������յ���е����ƣ�";
				if (input_Vex(GT, sn) == true)
					break;
			}

			cout << "\n�����복�Σ�";
			cin >> number;
			fflush(stdin);

			q.ivex = st;
			q.jvex = sn;
			strcpy(q.Number, number);

			cout << "\n������Ʊ�ۣ�";
			cin >> q.Money;
			fflush(stdin);

			cout << "\n��������ʼʱ�䣺";
			cout << "\n����: ";
			cin >> hour;
			fflush(stdin);
			cout << "����: ";
			cin >> minute;
			fflush(stdin);
			q.StartTime = TimeChange(hour, minute);

			cout << "\n�����뵽վʱ�䣺";
			cout << "\n����: ";
			cin >> hour;
			fflush(stdin);
			cout << "����: ";
			cin >> minute;
			fflush(stdin);

			q.EndTime = TimeChange(hour, minute);

			q.Time = TimeSub(q.EndTime, q.StartTime);

			cout << "\n�г�ʱ��:" << q.Time / 60 << "Сʱ " << q.Time % 60 << "����";

			InsertEdge(GT, q);

			cout << "\n��ӳɹ���" << "\n";
			break;
		}
		else if (n == 2)
		{
			while (1)
			{
				cout << "\n��������ʼ���е����ƣ�";
				if (input_Vex(GT, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n�������յ���е����ƣ�";
				if (input_Vex(GT, sn) == true)
					break;
			}

			cout << "\n�����복�Σ�";
			if (input_Number(GT, st, sn, number))
			{
				q.ivex = st;
				q.jvex = sn;
				strcpy(q.Number, number);
				DeleteEdge(GT, q);
				cout << "ɾ���ɹ���\n";
				break;
			}
		}
		else if (n == 0)
		{
			cout << ("\n");
			break;
		}
		else
			cout << "�����������������룡";
	}

}

//����ʱ�̱�༭
void Plane_Edit(Graph GT, Graph& GP)
{
	int n, st, sn;
	int hour, minute;
	char number[10];
	EdgeInfo q;
	while (1)
	{
		cout << ("\n");

		cout << "\t 1.��Ӻ���     ";
		cout << "\t 2.ɾ������       ";
		cout << "\t 0.�˳�           ";
		cout << "\t\t\t ��ѡ��";
		cin >> n;
		fflush(stdin);
		if (n == 1)
		{
			while (1)
			{
				cout << "\n��������ʼ���е����ƣ�";
				if (input_Vex(GP, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n�������յ���е����ƣ�";
				if (input_Vex(GP, sn) == true)
					break;
			}

			cout << "\n�����뺽��ţ�";

			cin >> number;
			fflush(stdin);

			q.ivex = st;
			q.jvex = sn;
			strcpy(q.Number, number);

			cout << "\n������Ʊ�ۣ�";
			cin >> q.Money;
			fflush(stdin);

			cout << "\n��������ʼʱ�䣺" << "\n";
			cout << "\n����: ";
			cin >> hour;
			fflush(stdin);
			cout << "����: ";
			cin >> minute;
			fflush(stdin);
			q.StartTime = TimeChange(hour, minute);

			cout << "\n�����뵽��ʱ�䣺";
			cout << "\n����: ";
			cin >> hour;
			fflush(stdin);
			cout << "����: ";
			cin >> minute;
			fflush(stdin);

			q.EndTime = TimeChange(hour, minute);

			q.Time = TimeSub(q.EndTime, q.StartTime);

			//cout << "\n�г�ʱ��:" << q.Time << "����";
			cout << "\n�г�ʱ��:" << q.Time / 60 << "Сʱ " << q.Time % 60 << "����";

			InsertEdge(GP, q);

			cout << "\n��ӳɹ���" << "\n";
			break;
		}
		else if (n == 2)
		{
			while (1)
			{
				cout << "\n��������ʼ���е����ƣ�";
				if (input_Vex(GT, st) == true)
					break;
			}
			while (1)
			{
				cout << "\n�������յ���е����ƣ�";
				if (input_Vex(GT, sn) == true)
					break;
			}

			cout << "\n�����뺽��ţ�";
			if (input_Number(GP, st, sn, number))
			{
				q.ivex = st;
				q.jvex = sn;
				strcpy(q.Number, number);
				DeleteEdge(GP, q);
				cout << "ɾ���ɹ���\n";
				break;
			}
		}
		else if (n == 0)
		{
			cout << "\n";
			break;
		}
		else
			cout << "�����������������룡";
	}

}


void Manage_Menu(Graph& GT, Graph& GP)
{
	while (1)
	{
		int choice = 5;
		cout << "\t********************************************************************" << "\n";
		cout << "\t*                             ����ϵͳ�˵�                         *" << "\n";
		cout << "\t*                             1.���б༭                           *" << "\n";
		cout << "\t*                             2.�г�ʱ�̱�༭                     *" << "\n";
		cout << "\t*                             3.����ʱ�̱�༭                     *" << "\n";
		cout << "\t*                             0.�˳�ϵͳ                           *" << "\n";
		cout << "\t********************************************************************" << "\n";
		cout << "           ��ѡ��: ";
		cin >> choice;
		fflush(stdin);
		if (choice == 0) break;
		switch (choice)
		{
		case 1: City_Edit(GT, GP); break;
		case 2: Train_Edit(GT, GP); break;
		case 3: Plane_Edit(GT, GP); break;
		default: {
			cout << "	   �������ѡ��,�����ԡ�\n\n";
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
		cout << "\t*                   ��ӭ����ȫ����ͨ��ѯϵͳ���˵�                 *" << "\n";
		cout << "\t*                            1.��ѯϵͳ                            *" << "\n";
		cout << "\t*                            2.����ϵͳ                            *" << "\n";
		cout << "\t*                            0.�˳�ϵͳ                            *" << "\n";
		cout << "\t********************************************************************" << "\n";
		cout << "\t\t\t           ��ѡ��: ";
		cin >> choicem;
		cout << "\n";
		fflush(stdin);//���������
		switch (choicem)
		{
		case 1: Inquire_Menu(GT, GP); break;
		case 2: Manage_Menu(GT, GP); break;
		case 0: return 0;
		default:
		{
			cout << "	   ��������,�����ԡ�\n\n";
		}
		}
	}
}


