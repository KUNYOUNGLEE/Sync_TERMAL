// Sync_TERMAL.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//


#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;
typedef struct termal_info {
	char hour_min[9];
	int sec;
	int m_sec;
	int termal_value[100];
	termal_info():sec(0),m_sec(0)
	{
		for (int i = 0; i < 100; i++)
		{
			termal_value[i] = 0;
		}
	}
};

int main()
{
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	char filename[40] = "12-15 12��44��_������_4����.csv";
	char read_path[100] = "C:\\Users\\guy92\\Desktop\\��ȭ��CNN_data_�׽�Ʈ��\\����\\";
	char write_path[100] = "C:\\Users\\guy92\\Desktop\\��ȭ��CNN_data_�׽�Ʈ��\\";

	strcat(read_path, filename);
	strcat(write_path, filename);

	fp1 = fopen(read_path, "r");

	//fp1 = fopen("C:\\Users\\guy92\\Desktop\\��ȭ��CNN_data_�׽�Ʈ��\\12-15 12��51��_������_2�ǿ�.csv", "r");
	char temp[20] = { 0, };
	char line[620] = { 0, };
	char *str = NULL;

	vector<termal_info> data;
	vector<termal_info> data2;

	//���� Ȥ�� �����ϱ����� Data �о�ͼ� vector �ڷᱸ���� �����ϴ� �ڵ�
	while(1)
	{
		termal_info temp_data;
		str = NULL;

		str = fgets(line, sizeof(line), fp1);
		
		if (str == NULL) break;

		memcpy(temp, &str[0], 8);
		temp[8] = 0;
		strcpy(temp_data.hour_min, temp);
		memcpy(temp, &str[9], 3);
		temp[3] = 0;
		temp_data.sec = atoi(temp);
		memcpy(temp, &str[12], 3);
		temp[3] = 0;
		temp_data.m_sec = atoi(temp);
		for (int i = 0; i < 100; i++)
		{
			memcpy(temp, &str[16 + (6 * i)], 6);
			temp[6] = 0;
			temp_data.termal_value[i] = atoi(temp);
		}

		data.push_back(temp_data);
	}

	//����� Data ���� Ȥ �����ϴ� �ڵ�!!!114ȭ


	int count = 0;

	for (int i = 1; i < data.size(); i++)
	{
		if (abs(data[i - 1].m_sec - data[i].m_sec) > 80 && !((data[i - 1].m_sec < 1000 && data[i - 1].m_sec > 940) && data[i].m_sec < 30))
		{
			termal_info temp_data;
			count++;
			printf("count = %d //%d - %d		%s		%d;%d\n", count, data[i - 1].m_sec, data[i].m_sec, data[i].hour_min, data[i].sec, data[i].m_sec);

			strcpy(temp_data.hour_min, data[i - 1].hour_min);

			//sec ���� 59 -> 0 �Ѿ�� ����(950ms �� 50ms ���� ������ �� ���!)
			if (data[i - 1].m_sec < 1000 && data[i - 1].m_sec > 940)
			{
				if (data[i - 1].sec == 59)
				{
					temp_data.sec = 0;
				}
				else
				{
					temp_data.sec = data[i - 1].sec + 1;
				}
			}
			else
			{
				temp_data.sec = data[i - 1].sec;
			}


			if (data[i - 1].m_sec < 880)
			{
				temp_data.m_sec = (data[i - 1].m_sec + data[i].m_sec) / 2;
			}
			else if ((data[i - 1].m_sec < 1000 && data[i - 1].m_sec > 940) && ((data[i].m_sec > 30) && (data[i].m_sec < 80)))
			{
				temp_data.m_sec = 0;
			}
			else if ((data[i - 1].m_sec < 930 && data[i - 1].m_sec > 880) && ((data[i].m_sec - 30) < 0))
			{
				temp_data.m_sec = 950;
			}


			for (int j = 0; j < 100; j++)
			{
				temp_data.termal_value[j] = (data[i - 1].termal_value[j] + data[i].termal_value[j]) / 2;
			}

			data2.push_back(data[i - 1]);
			data2.push_back(temp_data);
		}
		else
		{
			data2.push_back(data[i-1]);
		}
	}

	//���� Ȥ�� ������ Data ���� �ڵ�
	fp2 = fopen(write_path, "a");

	for (int i = 0; i < data2.size(); i++)
	{
		fprintf(fp2, "%s,", data2[i].hour_min);
		fprintf(fp2, "%2d;%3d,", data2[i].sec, data2[i].m_sec);
		for (int j = 0; j < 100; j++)
		{
			fprintf(fp2, "%d,", data2[i].termal_value[j]);
		}
		fprintf(fp2, "\n");
	}

	return 0;
}

