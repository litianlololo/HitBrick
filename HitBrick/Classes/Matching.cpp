#include "cocos2d.h"
#include "Gamemodel.h"
#include "Matching.h"
#include "ranking.h"
#include "onlineGame.h"
USING_NS_CC;

SOCKET client;
std::string IPAddr = "192.168.1.100";
Scene* Matching::createScene()
{
    auto scene = Scene::create();
    auto layer = Matching::create();
    scene->addChild(layer);
    return scene;
}

bool Matching::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //��ȡ��Ļ����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    MenuItemFont::setFontSize(20);
    auto ur1 = FileUtils::getInstance()->getStringFromFile("online/matching.txt");
    match = MenuItemFont::create(ur1, CC_CALLBACK_1(Matching::menuClickCallBack, this));
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(Matching::menuClickCallBack, this));
    //MainmenuTag
    match->setTag(1);
    back->setTag(0);
    menu = Menu::create(match, back, NULL);
    menu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(menu);
    menu->alignItemsVertically();

    scheduleUpdate();
    return true;
}


void Matching::menuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    unsigned long u1 = 0;
    if (NULL != node)
    {
        auto Gamemodelscene = Gamemodel::createScene();
        switch (tag)
        {
        case 0:
            //����
            send(client, "quit", sizeof("quit"), 0);
            WSACleanup();
            Director::getInstance()->replaceScene(Gamemodelscene);
            break;
        case 1:
            //�ͻ���
            match->setEnabled(false);
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);

            sockaddr_in serveraddr;
            serveraddr.sin_family = AF_INET;//���������г�ʼ��
            serveraddr.sin_port = htons(10000);

            serveraddr.sin_addr.s_addr = inet_addr(IPAddr.c_str());
            client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            matchLabel = Label::createWithTTF("Failed", "fonts/Marker Felt.ttf", 24);
            matchLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 100));
            this->addChild(matchLabel);


            ioctlsocket(client, FIONBIO, (unsigned long*)&u1);
            connect(client, (sockaddr*)&serveraddr, sizeof(serveraddr));
            break;
        case 3:
            //ƥ����ɣ���ʼ��Ϸ
            send(client, "ready", sizeof("ready"), 0);
            match->setEnabled(false);
            matchLabel->setString("Waiting for your opponent...");
            break;
        default:
            break;
        }
    }
}

void Matching::update(float dt)
{
    char dataRecv[128];
    recv(client, dataRecv, 128, 0);
    std::string str = dataRecv;

    if (str == "quit")
    {
        matchLabel->setString("Your opponent has left.");
    }
    else if (str == "connected")
    {
        matchLabel->setString("Matching...");
    }
    else if (str == "matched")
    {
        matchLabel->setString("Matched! Push the button to start.");
        //����Ϊstart��ť
        match->setEnabled(true);
        match->setString("strat");
        match->setTag(3);
    }
    if (str == "start")
    {
        matchLabel->setString("???????");
        Start();
    }
}
void Matching::Start()
{
    auto Dscene = onlineGame::createScene();
    Director::getInstance()->replaceScene(Dscene);
}