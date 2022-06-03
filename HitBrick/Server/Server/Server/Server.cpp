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
    //定义发送缓冲区和接受缓冲区
    char sendBuffer[128]={0};
    char recvBufferA[128] = { 0 };
    char recvBufferB[128] = { 0 };

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

    //服务端地址
    SOCKADDR_IN addrSrv;
    //客户端地址
    SOCKADDR_IN addrClient;
    //客户端地址集合
    map<SOCKET, sockaddr_in>addrplayers;
    //欲退出的套接字
    vector<SOCKET> deleteplayers;
    //用于检查可读取数据
    fd_set readfd;

    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(10000);

    //创建套接字
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    unsigned long u1 = 1;
    //设置为非阻塞
    ioctlsocket(server, FIONBIO, (unsigned long*)&u1);

    bind(server, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));  //套接字和机器上的一定的端口绑定

    //设置套接字为监听状态
    listen(server, 2);   //目前仅需接受两个玩家

    vector<SOCKET>players;//客户端集合
    vector<SOCKET>::iterator pl;  //players集合的迭代器
    pl = players.begin();         //先指向第一个玩家

    //标记是否开始
    bool Astart = false;
    bool Bstart = false;
    bool Astart2 = false;
    bool Bstart2 = false;

    int ret = 0;
    while (1)
    {
        bool newrecv = 0;
        int len = sizeof(SOCKADDR);
        int client = accept(server, (SOCKADDR*)&addrClient, &len);    //接受连接请求

        //准备阶段
        if (players.size()!=playersnum) {
            if (client != INVALID_SOCKET)
            {
                players.push_back(client);
                addrplayers[client] = addrClient;

                send(players.back(), "connected", sizeof("connected"), 0);
                cout << "已加入连接" << endl << "当前连接数: " << players.size() << endl;

                //已匹配两位玩家

                if (players.size() == playersnum)
                {
                    Sleep(300);
                    send(players[0], "matched", sizeof("matched"), 0);
                    send(players[1], "matched", sizeof("matched"), 0);
                    cout << "matched" << endl;
                }
            }
        }

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
            //正式游戏阶段
            if (players.size() == playersnum) {
                //接收A客户端信息
                Sleep(1000);
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

                    if (MsgA == "Quit")                         //玩家A退出
                    {
                        cout << "playerA quit" << endl;
                        strcpy(sendBuffer, "Quit");
                        send(players[1], sendBuffer, 128, 0);
                        deleteplayers.push_back(players[0]);
                    }
                    else if (MsgA == "Win")                      //有玩家获胜
                    {
                        cout << "playerA win" << endl;
                        strcpy(sendBuffer, "Win");
                        send(players[1], sendBuffer, 128, 0);
                        deleteplayers.push_back(players[0]);
                    }
                    else if (Bstart2) {
                        //发送给B玩家 运动信息
                        int ret1 = send(players[1], MsgA.c_str(), MsgA.size(), 0);
                        Sleep(100);
                    }
                }
                if (MsgA == "ready") {
                    Astart = 1;
                }
                //接收B客户端信息
                Sleep(1000);
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

                    if (MsgB == "Quit")                         //玩家A退出
                    {
                        cout << "playerB quit" << endl;
                        strcpy(sendBuffer, "Quit");
                        send(players[0], sendBuffer, 128, 0);
                        deleteplayers.push_back(players[1]);
                    }
                    else if (MsgB == "Win")                      //有玩家获胜
                    {
                        cout << "playerB win" << endl;
                        strcpy(sendBuffer, "Win");
                        send(players[0], sendBuffer, 128, 0);
                        deleteplayers.push_back(players[1]);
                    }
                    else if (Astart2) {
                        //发送给A玩家 运动信息
                        int ret2 = send(players[0], MsgB.c_str(), MsgB.size(), 0);
                        Sleep(100);

                    }

                }
                if (MsgB == "ready") {
                    Bstart = 1;
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

                        //}*/

                if (Astart && Bstart)
                {

                    send(players[0], "start", sizeof("start"), 0);
                    send(players[1], "start", sizeof("start"), 0);
                    Sleep(3000);
                    Astart2 = true;
                    Bstart2 = true;
                    Astart = false;
                    Bstart = false;//改掉避免持续发送
                }


                if (!deleteplayers.empty())  //已有玩家退出
                {
                    for (vector<SOCKET>::iterator pl = players.begin(); pl != players.end(); ++pl) {    //向玩家发出退出信息
                        //关闭所有客户端套接字
                        closesocket(*pl);
                    }
                }
            }
        }
    }
    
    //关闭套接字
    //closesocket(client);


    //释放DLL资源
    WSACleanup();

    return 0;
}
