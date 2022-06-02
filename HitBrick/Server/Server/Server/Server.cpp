#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <Winsock2.h>
#include <time.h>
#include<vector>
#include<map>
#include <string>
#pragma comment(lib,"ws2_32.lib")
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
        return 0;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        return 0;
    }

    //����˵�ַ
    SOCKADDR_IN addrSrv;
    //�ͻ��˵�ַ
    SOCKADDR_IN addrClient;
    //�ͻ��˵�ַ����
    map<SOCKET, sockaddr_in>addrplayers;
    //���˳����׽���
    vector<SOCKET> deleteplayers; 

    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(10000);  
    
    //�����׽���
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    unsigned long u1 = 1;
    //����Ϊ������
    ioctlsocket(server, FIONBIO, (unsigned long*)&u1);

    bind(server, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));  //�׽��ֺͻ����ϵ�һ���Ķ˿ڰ�

    //�����׽���Ϊ����״̬
    listen(server, 2);   //Ŀǰ��������������

    vector<SOCKET>players;//�ͻ��˼���
    vector<SOCKET>::iterator pl;  //players���ϵĵ�����
    pl = players.begin();         //��ָ���һ�����
    bool firstmatched = 1;

    //����Ƿ�ʼ
    bool Astart = false;        
    bool Bstart = false;

    while (1)
    {
        bool newrecv = 0;
        int len = sizeof(SOCKADDR);
        int client = accept(server, (SOCKADDR*)&addrClient, &len);    //������������

        //׼���׶�
        if (client != INVALID_SOCKET && firstmatched)
        {
            players.push_back(client);
            addrplayers[client] = addrClient;

            send(players.back(), "connected", sizeof("connected"), 0);
            cout <<"�Ѽ�������" << endl << "��ǰ������: " << players.size() << endl;

            //��ƥ����λ���
            
            if (players.size() == playersnum)
            {
                Sleep(300);
                firstmatched = 0;
                send(players[0], "matched", sizeof("matched"), 0);
                send(players[1], "matched", sizeof("matched"), 0);
                cout << "matched" << endl;
            }
           // else if(players.size() != playersnum && firstmatched) {
            //    send(players[0], "connected", sizeof("connected"), 0);
            //}
        }
        
        //��ʽ��Ϸ�׶�

        //if (players.size() == playersnum+1) {
            //���տͻ�����Ϣ
            for (pl = players.begin(); pl != players.end(); ++pl)
            {
                int recvLength = recv(*pl, recvBuffer, 128, 0);          //������Ϣ
                if (recvLength < 0)  //������Ϣ
                {
                    newrecv = 0;
                    //continue;
                }
                else {               //������Ϣ
                    newrecv = 1;
                    cout << "Client Message:" << recvBuffer << endl;
                    string Msg = string(recvBuffer);           //��ȡ����Ϣ����Msg
                    if (Msg == "Quit")                         //������˳�
                    {

                        for (vector<SOCKET>::iterator pl2 = players.begin(); pl2 != players.end(); ++pl2) {    //����һ��ҷ����˳���Ϣ
                            if (pl2 != pl) {
                                cout << "player quit" << endl;
                                strcpy(sendBuffer, "Quit");
                                send(*pl2, sendBuffer, 128, 0);
                            }
                        }
                        deleteplayers.push_back(*pl);
                    }
                    else if (Msg == "Win")                      //����һ�ʤ
                    {
                        for (vector<SOCKET>::iterator pl2 = players.begin(); pl2 != players.end(); ++pl2) {    //����һ��ҷ����˳���Ϣ
                            if (pl2 != pl) {
                                cout << "player win" << endl;
                                strcpy(sendBuffer, "Win");
                                send(*pl2, sendBuffer, 128, 0);
                            }
                        }
                        deleteplayers.push_back(*pl);
                    }
                    else if (Msg == "ready") {
                        if (pl == players.begin())
                        {
                            Astart = 1;
                        }
                        else {
                            Bstart = 1;
                        }
                    }
                    //������Ϸ��Ϣ
                    for (vector<SOCKET>::iterator pl2 = players.begin(); pl2 != players.end(); ++pl2) {    //����һ��ҷ����˳���Ϣ
                        if (pl2 != pl) {
                            send(*pl2, recvBuffer, 128, 0);
                        }
                    }

                }
                if (Astart && Bstart)
                {
                    send(players[0], "start", sizeof("start"), 0);
                    send(players[1], "start", sizeof("start"), 0);
                    Sleep(3000);
                    Astart = false;
                    Bstart = false;//�ĵ������������
                }
            }
       // }
        if (!deleteplayers.empty())  //��������˳�
        {
            for (vector<SOCKET>::iterator pl = players.begin(); pl != players.end(); ++pl) {    //����ҷ����˳���Ϣ
                //�ر����пͻ����׽���
                closesocket(*pl);
            }
        }

        //�ر��׽���
        closesocket(client);
    }

    //�ͷ�DLL��Դ
    WSACleanup();

    return 0;
}
