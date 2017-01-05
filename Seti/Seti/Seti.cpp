// Seti.cpp: ���������� ����� ����� ��� ����������� ����������.
//


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#include "winsock2.h"
#include <string>
#include <iostream>
#pragma comment(lib,"wsock32.lib")
bool CheckAddress(char* ip_)
{
	int points = 0,                  // ���������� �����
		numbers = 0;                    // �������� ������
	char* buff;                  // ����� ��� ������ ������
	buff = new char[3];
	for (int i = 0; ip_[i] != '\0'; i++)
	{
		if (ip_[i] <= '9'&&ip_[i] >= '0')
		{
			if (numbers > 3) return false;
			buff[numbers++] = ip_[i];
		}
		else
		{
			if (ip_[i] == '.')
			{
				if (atoi(buff) > 255)return false;
				if (numbers == 0) return false;
				numbers = 0;
				points++;
				buff = new char[3];
			}
			else return false;
		}
	}
	if (points != 3) return false;
	return true;

}

bool CheckMask(char* mask)
{
	int points = 0, // ���������������
		numbers = 0;	// �������� ������

	char* str = "";
	int prevok = 255;
	int mask_for_mask[8] = { 255, 254, 252, 248, 240, 224, 192, 128 };
	char* buff = new char[3];

	for (int i = 0; mask[i] != '\0'; i++)
	{
		if (mask[i] <= '9' && mask[i] >= '0')
		{
			if (numbers > 3) return false;
			buff[numbers++] = mask[i];
		}
		else if (mask[i] == '.') // ���� �����
		{
			if (atoi(buff) > 255) return false; // ��������� �������� ������

			if ((atoi(buff) != 255) && (atoi(buff) != 254) && (atoi(buff) != 252) && (atoi(buff) != 248)
				&& (atoi(buff) != 240) && (atoi(buff) != 224) && (atoi(buff) != 192) && (atoi(buff) != 128) && (atoi(buff) != 0))
			{
				return false;
			}

			if (((atoi(buff) == 255) || (atoi(buff) == 254) || (atoi(buff) == 252) || (atoi(buff) == 248)
				|| (atoi(buff) == 240) || (atoi(buff) == 224) || (atoi(buff) == 192) || (atoi(buff) == 128)) && (prevok != 255))
			{
				if (prevok != 0) return false;
			}

			if (atoi(buff) > prevok) return false;

			if (numbers == 0) return false;
			//���� ����� ��� - ������

			prevok = atoi(buff);
			numbers = 0;
			points++;
			for (int i = 0; i < sizeof(buff); i++) buff[i] = -1;
		}
		else return false;
	};
	if (points != 3) return false;
	// ���� ��������� ����� � ����� �� 3 - ������

	if (atoi(buff) == 0) return true;

	if ((atoi(buff) != 255) && (atoi(buff) != 254) && (atoi(buff) != 252) && (atoi(buff) != 248) && (atoi(buff) != 240)
		&& (atoi(buff) != 224) && (atoi(buff) != 192) && (atoi(buff) != 128) && (atoi(buff) != 0))
	{
		return false;
	};
	if (((atoi(buff) == 255) || (atoi(buff) == 254) || (atoi(buff) == 252) || (atoi(buff) == 248) || (atoi(buff) == 240)
		|| (atoi(buff) == 224) || (atoi(buff) == 192) || (atoi(buff) == 128) || (atoi(buff) == 0)) && (prevok != 255))
	{
		return false;
	}

	return true;
}


unsigned long CharToULong(char* ip_)
{
	unsigned long  out = 0;
	char *buff;
	buff = new char[3];
	for (int i = 0, j = 0; ip_[i] != '\0'; i++, j++)
	{
		if (ip_[i] != '.')
		{
			buff[j] = ip_[i];
		}
		if (ip_[i] == '.' || ip_[i + 1] == '\0')
		{
			out <<= 8;
			if (atoi(buff)>255) return NULL;

			out += (unsigned long)atoi(buff);
			j = -1;
			buff = new char[3];
		}
	}
	return out;
}
char* ULongToChar(unsigned long k)
{
	char* buff;
	unsigned long out;
	buff = new char[3];
	int save = 0;
	int value;
	char* ip = new char[15];
	int ip_pos = 0;
	for (int i = 0, n = 3; i < 4; i++, n--)
	{
		out = k;
		int q = n * 8;
		out >>= q;
		value = out - save * 256;
		save = out;
		_itoa(value, buff, 10);
		int j = 0;
		while (buff[j] != '\0')
		{
			ip[ip_pos++] = buff[j];
			j++;
		}
		if (i == 3)break;
		ip[ip_pos++] = '.';

	}
	ip[ip_pos] = '\0';
	return ip;
}

int main()
{
	setlocale(LC_ALL, "rus");
	char *ip_name = new char[255];             // �������� ip ������
	unsigned long ip = 0x1101;                 // ������� ����� ip �������
	char *mask_name = new char[255];           // �������� �����
	unsigned long mask = 0x1111;               // ������� ����� �������� �����
	unsigned long subnet;                      // ID �������
	unsigned long host;                        // ID �����

	std::cout << "������ IP-������:";
	std::cin >> ip_name;
	std::cout << "������� �����:";
	std::cin >> mask_name;
	if (!CheckAddress(ip_name))
	{
		std::cout << "������ IP";
		return 0;
	}
	if (!CheckMask(mask_name))
	{
		std::cout << "������ Mask";
		return 0;
	}
	ip = CharToULong(ip_name);
	mask = CharToULong(mask_name);
	subnet = ip & mask;
	host = ip & ~mask;



	std::cout << "IP: " << ULongToChar(ip) << "\n�����: " << ULongToChar(mask) << "\nID �������: " << ULongToChar(subnet) << "\nID �����: " << ULongToChar(host);
	return 0;
}

