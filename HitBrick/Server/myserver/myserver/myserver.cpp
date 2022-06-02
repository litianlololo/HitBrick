#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WINSOCK2.H>
#include <iostream>
#include<vector>
#include<map>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>
using namespace std;



int main()
{
	sockaddr_in serveraddr;
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	serveraddr.sin_family = AF_INET;//���������г�ʼ��
	serveraddr.sin_port = htons(10000);
	std::string ipaddr="192.168.1.100";
	serveraddr.sin_addr.s_addr = inet_addr(ipaddr.c_str());


vector<SOCKET>clientset;//�ͻ��˼���
map<SOCKET, sockaddr_in>s2addr;//��ַ
fd_set readfd;//���ڼ��ɶ�ȡ����
int ret = 0;

SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
unsigned long u1 = 1;
ioctlsocket(server, FIONBIO, (unsigned long*)&u1);//����Ϊ������

bind(server, (sockaddr*)&serveraddr, sizeof(serveraddr));//��
listen(server, 2);
bool Astart = false;
bool Bstart = false;
while (1)
{
	sockaddr_in addr;
	int len = sizeof(addr);
	int client = accept(server, (sockaddr*)&addr, &len);
	if (client != INVALID_SOCKET)
	{
		clientset.push_back(client);
		s2addr[client] = addr;
		send(clientset.back(), "connected", sizeof("connected"), 0);
		cout << inet_ntoa(addr.sin_addr) << "�Ѽ�������" << "   ��ǰ������: " << clientset.size() << endl;
		if (clientset.size() == 2)
		{
			Sleep(300);
			send(clientset[0], "matched", sizeof("matched"), 0);
			send(clientset[1], "matched", sizeof("matched"), 0);
		}
	}
	FD_ZERO(&readfd);//��ʼ��
	for (int i = 0; i < clientset.size(); i++)
	{
		FD_SET((int)(clientset[i]), &readfd);//���
	}

	//�鿴�Ƿ������ݷ���
	ret = 0;
	if (!clientset.empty())
	{
		timeval tv = { 0,0 };
		ret = select(clientset[clientset.size() - 1] + 1, &readfd, NULL, NULL, &tv);
	}
	//��������
	if (ret > 0)
	{
		vector<SOCKET> deleteclient;//Ҫ�˳����׽��֣���ɾ�����׽���vector

		for (int i = 0; i < clientset.size(); i++)
		{
			if (FD_ISSET((int)(clientset[i]), &readfd))
			{
				char data[1024] = { 0 };
				
				//������Ϣ
				int ret = recv(clientset[i], data, 1023, 0);
				string recvdata = data;
				//��Ҫ���н�����������Ϣ
				if (recvdata == "quit" || recvdata == "win" || recvdata == "lose")
				{
					if (recvdata == "win" || recvdata == "lose")
					{
						if (clientset.size() == 2)
						{
							if (FD_ISSET((int)(clientset[0]), &readfd))
							{
								//֪ͨB�����Ϸ����
								int ret1 = send(clientset[1], recvdata.c_str(), recvdata.size(), 0);
								Sleep(5000);
							}
							if (FD_ISSET((int)(clientset[1]), &readfd))
							{
								//֪ͨA�����Ϸ����
								int ret2 = send(clientset[0], recvdata.c_str(), recvdata.size(), 0);
								Sleep(5000);
							}
						}
					}
					//��Ҫ�˳�����Ҽ���deleteclient����
					deleteclient.push_back(clientset[i]);
				}
				else
				{
					//������LOG
					std::cout << "����" << inet_ntoa(s2addr[clientset[i]].sin_addr) << " : " << data << endl;
				}
				//��Ϸ����
				if (clientset.size() == 2)
				{
					//A���
					if (FD_ISSET((int)(clientset[0]), &readfd))
					{

						//����Ϣ����B���
						int ret1 = send(clientset[1], recvdata.c_str(), recvdata.size(), 0);

						if (recvdata == "ready")
						{
							Astart = true;{}
						}
					}
					//B���
					if (FD_ISSET((int)(clientset[1]), &readfd))
					{
						//����Ϣ����A���
						int ret2 = send(clientset[0], recvdata.c_str(), recvdata.size(), 0);

						if (recvdata == "ready")
						{
							Bstart = true;
						}
					}
				}
				//
			}
		}
		if (Astart && Bstart)
		{
			send(clientset[0], "start", sizeof("start"), 0);
			send(clientset[1], "start", sizeof("start"), 0);
			Sleep(3000);
			srand((unsigned)time(NULL));
			char a = char(rand() % 4 + 49);
			string level = "";
			level += a;
			send(clientset[0], level.c_str(), level.size(), 0);
			send(clientset[1], level.c_str(), level.size(), 0);
			Astart = false;
			Bstart = false;//�ĵ������������
		}
		if (!deleteclient.empty())
		{
			for (int i = 0; i < deleteclient.size(); i++)
			{
				if (i == 0)
					std::cout << "���A" << "���˳����ӣ�ʣ��������: " << clientset.size() - 1 << endl;
				else
					std::cout << "���B" << "���˳����ӣ�ʣ��������: " << clientset.size() - 1 << endl;
				closesocket(deleteclient[i]);
				vector<SOCKET>::iterator it = find(clientset.begin(), clientset.end(), deleteclient[i]);
				clientset.erase(it);
			}
		}
	}
}
}