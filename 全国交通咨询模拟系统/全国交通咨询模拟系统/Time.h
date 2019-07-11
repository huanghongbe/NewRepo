#pragma once
#include <iostream>
using namespace std;
int TimeChange(int hour, int minute)
{   //把输入的小时和分钟树转换成分钟的形式，忽略天数
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
{   // 把分钟转换成小时和分钟，并输出(包括起始时间,历经时间)
	int hour, minute;

	if (m > 1440)//一天有1440分钟 
	{
		m = m % 1440;
	}

	hour = m / 60;
	minute = m % 60;

	cout << hour << "时" << " " << minute << "分" << endl;

	//printf("%4d时%2d分",hour,minute);
}

int TimeSub(int a, int b)
{//返回参数的两个时间的间隔时间，忽略天数
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