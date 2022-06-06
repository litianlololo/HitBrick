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
    cout << "请选择是否输入IP 1是 0否（默认192.168.1.102）" << endl;
    cin >> i;
    if (i == 1) {
        cout << "请输入IP地址（默认192.168.1.102）" << endl;
        cin >> ipaddr;
    }
    cout << "ready"<<endl;
    while (1) {
        //定义发送缓冲区和接受缓冲区
        char sendBuffer[128] = { 0 };
        char recvBufferA[128] = { 0 };
        char recvBufferB[128] = { 0 };

        WORD wVersionRequested;
        WSADATA wsaData;

        //服务端地址
        SOCKADDR_IN addrSrv;
        //客户端地址
        SOCKADDR_IN addrClient;
        //客户端地址集合
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

        //用于检查可读取数据
        fd_set readfd;

        //创建套接字
        SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        unsigned long u1 = 1;
        //设置为非阻塞
        ioctlsocket(server, FIONBIO, (unsigned long*)&u1);

        bind(server, (SOCKADDR*)&addrSrv, sizeof(addrSrv));  //套接字和机器上的一定的端口绑定

        //设置套接字为监听状态
        listen(server, 2);   //目前仅需接受两个玩家

        vector<SOCKET>players;//客户端集合
        vector<SOCKET>::iterator pl;  //players集合的迭代器
        pl = players.begin();         //先指向第一个玩家

        //标记是否开始
        bool ifstart = 0;

        int ret = 0;
        while (1)
        {
            sockaddr_in addr;
            bool newrecv = 0;
            int len = sizeof(addr);
            int client = accept(server, (SOCKADDR*)&addr, &len);    //接受连接请求
            //准备阶段
            if (players.size() != playersnum) {

                if (client != INVALID_SOCKET)
                {
                    players.push_back(client);
                    addrplayers[client] = addr;

                    send(players.back(), "connected", sizeof("connected"), 0);
                    cout << "已加入连接" << endl << "当前连接数: " << players.size() << endl;

                    //已匹配两位玩家

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
            //处理数据
            if (ifstart) {
                FD_ZERO(&readfd);//初始化
                for (int i = 0; i < players.size(); i++)
                {
                    FD_SET((int)(players[i]), &readfd);//检查
                }

                //查看是否有数据发送
                ret = 0;
                if (!players.empty())
                {
                    timeval tv = { 0,0 };
                    ret = select(players[players.size() - 1] + 1, &readfd, NULL, NULL, &tv);
                }
                //处理数据
                if (ret > 0)
                {
                    cout << "<<<<<" << ret << ">>>>>>" << endl;
                    //欲退出的套接字
                    vector<SOCKET> deleteplayers;
                    //正式游戏阶段
                    //接收A客户端信息
                    int recvLengthA = recv(players[0], recvBufferA, 128, 0);          //接收信息
                    string MsgA = string(recvBufferA);           //读取的信息存入Msg
                    if (recvLengthA < 0)  //无新信息
                    {
                        newrecv = 0;
                    }
                    else
                    {
                        newrecv = 1;
                        cout << "playerA Message:" << recvBufferA << endl;
                        if (MsgA[0] == 'W')                      //有玩家获胜
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
                            //发送给B玩家 运动信息
                            int ret1 = send(players[1], MsgA.c_str(), MsgA.size(), 0);
                            cout << "sendtoB" << MsgA.c_str() << endl;
                        }
                    }
                    //接收B客户端信息
       
                    int recvLengthB = recv(players[1], recvBufferB, 128, 0);          //接收信息
                    string MsgB = string(recvBufferB);           //读取的信息存入Msg
                    if (recvLengthB < 0)  //无新信息
                    {
                        newrecv = 0;
                    }
                    else
                    {
                        newrecv = 1;
                        cout << "playerB Message:" << recvBufferB << endl;

                        if (MsgB[0] == 'W')                      //有玩家获胜
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
                            //发送给A玩家 运动信息
                            int ret2 = send(players[0], MsgB.c_str(), MsgB.size(), 0);
                            cout << "sendtoA" << MsgB.c_str() << endl;

                        }
                    }
                    /*
                    //cout << "2" << endl;
                    for (pl = players.begin(); pl != players.end(); ++pl)
                    {
                        int recvLength = recv(players[0], recvBuffer, 128, 0);          //接收信息
                        if (recvLength < 0)  //无新信息
                        {
                            newrecv = 0;
                            //continue;
                        }
                        else {               //有新信息
                            newrecv = 1;
                            cout << "Client Message:" << recvBuffer << endl;
                            string Msg = string(recvBuffer);           //读取的信息存入Msg
                            if (Msg == "Quit")                         //有玩家退出
                            {

                                for (vector<SOCKET>::iterator pl2 = players.begin(); pl2 != players.end(); ++pl2) {    //向另一玩家发出退出信息
                                    if (pl2 != pl) {
                                        cout << "player quit" << endl;
                                        strcpy(sendBuffer, "Quit");
                                        send(*pl2, sendBuffer, 128, 0);
                                    }
                                }
                                deleteplayers.push_back(*pl);
                            }
                            else if (Msg == "Win")                      //有玩家获胜
                            {
                                for (vector<SOCKET>::iterator pl2 = players.begin(); pl2 != players.end(); ++pl2) {    //向另一玩家发出退出信息
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
                            //发送游戏信息
                            /*
                            for (vector<SOCKET>::iterator pl2 = players.begin(); pl2 != players.end(); ++pl2) {    //向另一玩家发出退出信息
                                if (pl2 != pl) {
                                    send(*pl2, recvBuffer, 128, 0);
                                }
                            }*/
                }
            }
        }
        //释放DLL资源
        WSACleanup();
    }
    return 0;
}
