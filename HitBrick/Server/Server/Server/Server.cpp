#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Winsock2.h>
#include<vector>
#include<map>

using namespace std;
const int playersnum = 2;
int main()
{
    //���巢�ͻ������ͽ��ܻ�����
    char sendBuffer[128];
    char recvBuffer[128];

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);//

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        return;
    }

    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
    {
        WSACleanup();
        return;
    }

    //����˵�ַ
    SOCKADDR_IN addrSrv;
    //�ͻ��˵�ַ
    SOCKADDR_IN addrClient;
    //�ͻ��˵�ַ����
    map<SOCKET, sockaddr_in>addrplayers;

    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(1000);  
    
    //�����׽���
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    unsigned long u1 = 1;
    ioctlsocket(server, FIONBIO, (unsigned long*)&u1);//����Ϊ������

    bind(server, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));  //�׽��ֺͻ����ϵ�һ���Ķ˿ڰ�

    //�����׽���Ϊ����״̬
    listen(server, 2);   //Ŀǰ��������������

    vector<SOCKET>players;//�ͻ��˼���
    vector<SOCKET>::iterator pl;  //players���ϵĵ�����
    pl = players.begin();         //��ָ���һ�����

    while (1)
    {
        int len = sizeof(SOCKADDR);
        SOCKET client = accept(server, (SOCKADDR*)&addrClient, &len);    //������������

        //׼���׶�
        if (client != INVALID_SOCKET && players.size() <= playersnum)
        {
            players.push_back(client);
            addrplayers[client] = addrClient;

            send(players.back(), "connected", sizeof("connected"), 0);
            cout << inet_ntoa(addrClient.sin_addr) << "�Ѽ�������" << endl << "��ǰ������: " << players.size() << endl;

            //��ƥ����λ���
            if (players.size() == playersnum)
            {
                //Sleep(300);
                send(*pl, "matched", sizeof("matched"), 0);
                send(*(++pl), "matched", sizeof("matched"), 0);
            }
        }
        if (players.size() == playersnum) {
            //���տͻ�����Ϣ
            for (pl = players.begin(); pl != players.end(); ++pl)
            {
                int recvLength = recv(*pl, recvBuffer, 128, 0);          //
            }
        }





        char sendBuf[50];
        sprintf(sendBuf, "Welcome %s to here!", inet_ntoa(addrClient.sin_addr));
        send(client, sendBuf, strlen(sendBuf) + 1, 0);
        char recvBuf[50];
        recv(client, recvBuf, 50, 0);
        printf("%s\n", recvBuf);
        //�ر��׽���
        closesocket(client);
    }

    //�ͷ�DLL��Դ
    WSACleanup();

    return 0;
}
