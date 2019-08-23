#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MONTH 12
#define HEBDOMAD 7
typedef struct{
	int year;
	int month;
	int day;
	char *week[10];
	int total;
}date;
void ReturnTatol(date* Date, int *months, int weekindex);
void CalcuateDate(date *Date, int *months, int weekindex);
void IncrementCount(date *Date, int *months, int weekindex, int objective);
void DecrementCount(date *Date, int *months, int weekindex, int aim);
void Bissextile(int judge, int *months);		// ����Ĵ��� 
int main()
{
	date amateur[2] = {
		{
			0,
			0,
			0,
			.week = { "������", "����һ", "���ڶ�", "������", "������", "������", "������" },
			0,
		}
	};
	int sum = 0,acquire,ii;
	int months[MONTH] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, };  // ÿ���µ����� 
	while (1)
	{
		printf("�����������ʼ�����뵱ǰ���ڣ����������������������룺0\n");
		printf("������������������������֮�����ڣ������룺1\n");
		scanf("%d", &acquire);
		void(*choose[2])(date *Date, int *months, int weekindex) = { ReturnTatol, CalcuateDate };
		while (acquire >= 0 && acquire <= 1)
		{
			printf("��������ʼ����(�����ʽΪ��xxx-xxx-xxx xxx(��-��-�� ���ڼ�))��\n");
			scanf("%d-%d-%d %d", &amateur[0].year, &amateur[0].month, &amateur[0].day, &ii);
			choose[acquire](amateur, months, ii);
			break;
		}
		printf("Please input next date(empty line to quit)��\n");
		while (getchar() != '\n')
		{
			continue;
		}

		if (getchar() == '\n')
		{
			break;
		}
		else continue;
	}
	return 0;

}
void ReturnTatol(date *Date, int *months, int weekindex)
{
	date temp = Date[0];
	int ii;
	printf("�����뵱ǰ������(�����ʽΪ��xxx-xxx-xxx xxx(��-��-�� ���ڼ�))��\n");
	scanf("%d-%d-%d %d", &Date[1].year, &Date[1].month, &Date[1].day, &ii);
	Bissextile(temp.year, months);
	if (Date[1].year == temp.year && Date[1].month == temp.month)
	{
		temp.total = Date[1].day - temp.day;
	}
	else{
		for (temp.total = months[temp.month - 1] - temp.day; (Date + 1)->year != temp.year || Date[1].month != temp.month; temp.total += months[temp.month++])
		{
			if (temp.month == MONTH)
			{
				temp.year++;
				Bissextile(temp.year, months);
				temp.month = 0;
			}
			if (Date[1].year == temp.year && Date[1].month == temp.month + 1)
			{
				temp.total += Date[1].day;
				break;
			}
		}
	}
	printf("%d��%d��%d��(%s)��%d��%d��%d��(%s)�����%d��\n",
		Date[0].year, Date[0].month, Date[0].day, Date[0].week[weekindex], Date[1].year, Date[1].month, Date[1].day, temp.week[ii%HEBDOMAD], temp.total);
}
void CalcuateDate(date *Date, int *months, int weekindex)
{
	int objective;
	char ch;
	printf("����������Ҫ��������������Ǹ���������������������������:\n");
	scanf("%d", &objective);
	printf("����������ʼ����ǰ�����ڣ������� - ����\n");
	printf("����������ʼ���ں�����ڣ������� + ����\n");
	while ((ch = getchar()) != '\n')
	{
		continue;
	}
	switch (ch = getchar())
	{
	case '+':IncrementCount(&Date[0], months, weekindex, objective); break;
	case '-':DecrementCount(&Date[0], months, weekindex, objective); break;
	}
}
void IncrementCount(date *Date, int *months, int weekindex, int objective)
{
	date temp = Date[0];
	int sum = 0;
	Bissextile(temp.year, months);
	temp.total = weekindex;
	weekindex += objective%HEBDOMAD;
	if (temp.day + objective <= months[temp.month - 1])
	{
		temp.day += objective;
	}
	else{
		for (sum = months[temp.month - 1] - temp.day; sum <= objective; sum += months[temp.month++])
		{
			if (temp.month == MONTH)
			{
				temp.year++;
				Bissextile(temp.year, months);
				temp.month = 0;
			}
			if (sum + months[temp.month] >= objective)
			{
				temp.month++;
				temp.day = 0;
				temp.day += objective - sum;
				break;
			}
		}
	}
	printf("%d��%d��%d��(%s)�ĺ�%d����:%d��%d��%d��(%s)\n", Date[0].year, Date[0].month, Date[0].day, Date[0].week[temp.total%HEBDOMAD], objective, temp.year, temp.month, temp.day, temp.week[weekindex%HEBDOMAD]);
}
void DecrementCount(date *Date, int *months, int weekindex, int aim)
{
	date temp = Date[0];
	int sum = 0;
	Bissextile(temp.year, months);
	temp.total = weekindex;
	weekindex = HEBDOMAD - abs(weekindex%HEBDOMAD - aim%HEBDOMAD);
	//	weekindex=(temp.total=(aim%HEBDOMAD-weekindex)%HEBDOMAD)>=0?HEBDOMAD-temp.total:abs(temp.total);
	if (temp.day - aim > 0)
	{
		temp.day -= aim;
	}
	else{
		for (sum = temp.day; sum <= aim; sum += months[temp.month - 1])
		{
			if (temp.month - 1 == 0)
			{
				temp.year--;
				Bissextile(temp.year, months);
				temp.month = MONTH + 1;
			}
			if (sum + months[temp.month - 2] >= aim)
			{
				temp.day = 0;
				temp.day += months[temp.month - 2] - (aim - sum);
				temp.month--;
				break;
			}
			temp.month--;
		}
	}
	printf("%d��%d��%d��(%s)��ǰ%d����:%d��%d��%d��(%s)\n", Date[0].year, Date[0].month, Date[0].day, Date[0].week[temp.total], aim, temp.year, temp.month, temp.day, temp.week[weekindex]);
}
void Bissextile(int Date, int *months)		// ����Ĵ��� 
{
	months[1] = 28;
	if (Date % 4 == 0)
	{
		months[1]++;
	}
}
