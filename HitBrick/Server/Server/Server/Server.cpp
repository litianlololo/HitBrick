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
string ipaddr="192.168.1.102";
int i;
int main()
{
    cout << "��ѡ���Ƿ�����IP 1�� 0��Ĭ��192.168.1.102��" << endl;
    cin >> i;
    if (i == 1) {
        cout << "������IP��ַ��Ĭ��192.168.1.102��" << endl;
        cin >> ipaddr;
    }
    cout << "ready"<<endl;
    while (1) {
        //���巢�ͻ������ͽ��ܻ�����
        char sendBuffer[128] = { 0 };
        char recvBufferA[128] = { 0 };
        char recvBufferB[128] = { 0 };

        WORD wVersionRequested;
        WSADATA wsaData;

        //����˵�ַ
        SOCKADDR_IN addrSrv;
        //�ͻ��˵�ַ
        SOCKADDR_IN addrClient;
        //�ͻ��˵�ַ����
        map<SOCKET, sockaddr_in>addrplayers;
        addrSrv.sin_addr.S_un.S_addr = inet_addr(ipaddr.c_str());
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_port = htons(10000);

        wVersionRequested = MAKEWORD(2, 2);

        int err;
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

        //���ڼ��ɶ�ȡ����
        fd_set readfd;

        //�����׽���
        SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        unsigned long u1 = 1;
        //����Ϊ������
        ioctlsocket(server, FIONBIO, (unsigned long*)&u1);

        bind(server, (SOCKADDR*)&addrSrv, sizeof(addrSrv));  //�׽��ֺͻ����ϵ�һ���Ķ˿ڰ�

        //�����׽���Ϊ����״̬
        listen(server, 2);   //Ŀǰ��������������

        vector<SOCKET>players;//�ͻ��˼���
        vector<SOCKET>::iterator pl;  //players���ϵĵ�����
        pl = players.begin();         //��ָ���һ�����

        //����Ƿ�ʼ
        bool ifstart = 0;

        int ret = 0;
        while (1)
        {
            sockaddr_in addr;
            bool newrecv = 0;
            int len = sizeof(addr);
            int client = accept(server, (SOCKADDR*)&addr, &len);    //������������
            //׼���׶�
            if (players.size() != playersnum) {

                if (client != INVALID_SOCKET)
                {
                    players.push_back(client);
                    addrplayers[client] = addr;

                    send(players.back(), "connected", sizeof("connected"), 0);
                    cout << "�Ѽ�������" << endl << "��ǰ������: " << players.size() << endl;

                    //��ƥ����λ���

                    if (players.size() == playersnum)
                    {
                        Sleep(1000);
                        send(players[0], "matched", sizeof("matched"), 0);
                        send(players[1], "matched", sizeof("matched"), 0);
                        ifstart = 1;
                        cout << "matched" << endl;
                    }
                }
            }
            //��������
            if (ifstart) {
                FD_ZERO(&readfd);//��ʼ��
                for (int i = 0; i < players.size(); i++)
                {
                    FD_SET((int)(players[i]), &readfd);//���
                }

                //�鿴�Ƿ������ݷ���
                ret = 0;
                if (!players.empty())
                {
                    timeval tv = { 0,0 };
                    ret = select(players[players.size() - 1] + 1, &readfd, NULL, NULL, &tv);
                }
                //��������
                if (ret > 0)
                {
                    cout << "<<<<<" << ret << ">>>>>>" << endl;
                    //���˳����׽���
                    vector<SOCKET> deleteplayers;
                    //��ʽ��Ϸ�׶�
                    //����A�ͻ�����Ϣ
                    int recvLengthA = recv(players[0], recvBufferA, 128, 0);          //������Ϣ
                    string MsgA = string(recvBufferA);           //��ȡ����Ϣ����Msg
                    if (recvLengthA < 0)  //������Ϣ
                    {
                        newrecv = 0;
                    }
                    else
                    {
                        newrecv = 1;
                        cout << "playerA Message:" << recvBufferA << endl;
                        if (MsgA[0] == 'W')                      //����һ�ʤ
                        {
                            cout << "playerA win" << endl;
                            strcpy(sendBuffer, "Win");
                            send(players[0], sendBuffer, 127, 0);
                            send(players[1], sendBuffer, 127, 0);
                            closesocket(players[0]);
                            closesocket(players[1]);
                            return 0;
                            //deleteplayers.push_back(players[0]);
                            //deleteplayers.push_back(players[1]);
                        }
                        else if (ifstart) {
                            //���͸�B��� �˶���Ϣ
                            int ret1 = send(players[1], MsgA.c_str(), MsgA.size(), 0);
                            cout << "sendtoB" << MsgA.c_str() << endl;
                        }
                    }
                    //����B�ͻ�����Ϣ
       
                    int recvLengthB = recv(players[1], recvBufferB, 128, 0);          //������Ϣ
                    string MsgB = string(recvBufferB);           //��ȡ����Ϣ����Msg
                    if (recvLengthB < 0)  //������Ϣ
                    {
                        newrecv = 0;
                    }
                    else
                    {
                        newrecv = 1;
                        cout << "playerB Message:" << recvBufferB << endl;

                        if (MsgB[0] == 'W')                      //����һ�ʤ
                        {
                            cout << "playerB win" << endl;
                            strcpy(sendBuffer, "Win");
                            send(players[0], sendBuffer, 127, 0);
                            send(players[1], sendBuffer, 127, 0);
                            closesocket(players[0]);
                            closesocket(players[1]);
                            return 0;
                        }
                        else if (ifstart) {
                            //���͸�A��� �˶���Ϣ
                            int ret2 = send(players[0], MsgB.c_str(), MsgB.size(), 0);
                            cout << "sendtoA" << MsgB.c_str() << endl;

                        }
                    }
                    /*
                    //cout << "2" << endl;
                    for (pl = players.begin(); pl != players.end(); ++pl)
                    {
                        int recvLength = recv(players[0], recvBuffer, 128, 0);          //������Ϣ
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
                            /*
                            for (vector<SOCKET>::iterator pl2 = players.begin(); pl2 != players.end(); ++pl2) {    //����һ��ҷ����˳���Ϣ
                                if (pl2 != pl) {
                                    send(*pl2, recvBuffer, 128, 0);
                                }
                            }*/
                }
            }
        }
        //�ͷ�DLL��Դ
        WSACleanup();
    }
    return 0;
}
