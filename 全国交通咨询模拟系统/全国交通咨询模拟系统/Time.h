#pragma once
#include <iostream>
using namespace std;
int TimeChange(int hour, int minute)
{   //�������Сʱ�ͷ�����ת���ɷ��ӵ���ʽ����������
	if (minute > 60)
	{
		hour = hour + minute / 60;
		minute = minute % 60;
	}

	if (hour > 24)
	{
		hour = hour % 24;
	}

	int change = hour * 60 + minute;
	return change;
}

void PrintTime(int m)
{   // �ѷ���ת����Сʱ�ͷ��ӣ������(������ʼʱ��,����ʱ��)
	int hour, minute;

	if (m > 1440)//һ����1440���� 
	{
		m = m % 1440;
	}

	hour = m / 60;
	minute = m % 60;

	cout << hour << "ʱ" << " " << minute << "��" << endl;

	//printf("%4dʱ%2d��",hour,minute);
}

int TimeSub(int a, int b)
{//���ز���������ʱ��ļ��ʱ�䣬��������
	int c;
	if (a - b < 0)
	{
		c = a + 1440 - b;
	}
	else if (a - b > 1440)
	{
		c = (a - b) % 1440;
	}
	else
		c = a - b;
	return c;
}