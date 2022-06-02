#include "cocos2d.h"
#include "onlineGame.h"
#include "store.h"
#include "Gamemenu2.h"
#include"Matching.h"

extern SOCKET client;
extern std::string IPAddr;
USING_NS_CC;

Scene* onlineGame::createScene() {

    srand((unsigned)time(NULL));

    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setAutoStep(true);


    scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
    auto layer = onlineGame::create();

    layer->setPhysicsWorld(scene->getPhysicsWorld());
    layer->setJoint();
    scene->addChild(layer);
    return scene;
}

bool onlineGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    UserDefault::getInstance()->flush();
    visibleSize = Director::getInstance()->getVisibleSize();                              //��ȡ��Ļ����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;//���������г�ʼ��
    serveraddr.sin_port = htons(10000);
    serveraddr.sin_addr.s_addr = inet_addr(IPAddr.c_str());
    /*
    while (1)
    {
        char dataRecv[128];
        if (recv(client, dataRecv, 1023, 0) != -1)
        {
            state = atoi(dataRecv);
            break;
        }
    }*/

    //��ӵ�ͼ
    addmap();
    //��Ӱ�
    addBoard();
    addboard2();
    //�����
    addball();
    addball2();
    //���Wall
    addWall();
    //���ש��
    addBricks();
    //���2xש��
    add2xbricks();
    //�����ͣ��ť
    addbutton();
    //��ӷ���
    addscore();
    //���ʱ��
    addtime();
    //��Ӽ��̼�����
    addKeyListener();
    //�����ײ������
    addcontactListener();

    scheduleUpdate();

    schedule(SEL_SCHEDULE(&onlineGame::timer), 1.0f);
    return true;
}

void onlineGame::addBackGround()
{
    //ʵ�ֱ���ͼƬ������������
    auto bg = Sprite::create("background/background.jpg");
    bg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    Size mywinsize = Director::getInstance()->getWinSize();
    float winw = mywinsize.width; //��ȡ��Ļ���
    float winh = mywinsize.height;//��ȡ��Ļ�߶�
    float spx = bg->getTextureRect().getMaxX();
    float spy = bg->getTextureRect().getMaxY();
    bg->setScaleX(winw / spx); //���þ��������ű���
    bg->setScaleY(winh / spy);
    this->addChild(bg);

    return;
}

void onlineGame::addBoard()
{
    board = Sprite::create("board/board.png");
    board->setAnchorPoint(Vec2(0.5f, 0.5f));
    board->setPosition(Vec2(visibleSize.width / 2 + origin.x+50+ board->getContentSize().width/2, origin.y + board->getContentSize().height / 2+10));
//
    board->setTag(tagboard);           //�����̼�������λ


    //��Ӱ�ĸ�������
    auto BoardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));

    BoardBody->setCategoryBitmask(0xFFFFFFFF);           //�������
    BoardBody->setCollisionBitmask(0xFFFFFFFF);           //����ײ��  ��ײ����
    BoardBody->setContactTestBitmask(0xFFFFFFFF);        //�ɽӵ�֪ͨ  �Ӵ�����
    BoardBody->setGravityEnable(false);                  //��������Ӱ��
    BoardBody->setTag(tagboard);

    board->setPhysicsBody(BoardBody);

    board->getPhysicsBody()->setRotationEnable(false);   //�趨����ת


    this->addChild(board, tagboard);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return;
}
void onlineGame::addboard2()
{
    //��Ӱ�ĸ�������    
    board2 = Sprite::create("board/board.png");
    board2->setAnchorPoint(Vec2(0, 0.5f));
    board2->setPosition(Vec2(board2->getContentSize().width / 2, origin.y + board2->getContentSize().height / 2));

    board2->setTag(tagboard2);           //�����̼�������λ

    auto BoardBody2 = PhysicsBody::createBox(board2->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));

    BoardBody2->setCategoryBitmask(0xFFFFFFFF);           //�������
    BoardBody2->setCollisionBitmask(0xFFFFFFFF);           //����ײ��  ��ײ����
    BoardBody2->setContactTestBitmask(0xFFFFFFFF);        //�ɽӵ�֪ͨ  �Ӵ�����
    BoardBody2->setGravityEnable(false);                  //��������Ӱ��
    BoardBody2->setTag(tagboard2);

    board2->setPhysicsBody(BoardBody2);

    board2->getPhysicsBody()->setRotationEnable(false);   //�趨����ת


    this->addChild(board2);
}

void onlineGame::addball()
{
    ball = Sprite::create("ball/ball.png");
    ball->setScale(2.0f, 2.0f);
    ball->setAnchorPoint(Vec2(0.5f, 0.5f));
    ball->setPosition(Vec2(visibleSize.width / 2 + origin.x + 50 + board->getContentSize().width / 2, origin.y + board->getContentSize().height+10));

    // ������ĸ�������
    auto ballBody = PhysicsBody::createCircle(ball->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

    ballBody->setCategoryBitmask(0xFFFFFFFF);             //�������
    ballBody->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
    ballBody->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
    ballBody->setGravityEnable(false);                    //��������Ӱ��

    ballBody->setRotationEnable(false);                  //�趨����ת

    ball->setPhysicsBody(ballBody);
    ball->setTag(tagball);
    addChild(ball);
}

void onlineGame::addball2() {
    ball2 = Sprite::create("ball/ball.png");
    ball2->setScale(2.0f, 2.0f);
    ball2->setAnchorPoint(Vec2(0.5f, 0.5f));
    ball2->setPosition(Vec2(28 + board2->getContentSize().width / 2, origin.y + board2->getContentSize().height - 3));

    // ������ĸ�������
    auto ballBody2 = PhysicsBody::createCircle(ball->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

    ballBody2->setCategoryBitmask(0xFFFFFFFF);             //�������
    ballBody2->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
    ballBody2->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
    ballBody2->setGravityEnable(false);                    //��������Ӱ��

    ballBody2->setRotationEnable(false);                  //�趨����ת

    ball2->setPhysicsBody(ballBody2);
    ball2->setTag(tagball2);
    addChild(ball2);
}
void onlineGame::addmap()
{
    map = cocos2d::TMXTiledMap::create("onlinemap.tmx");
    map->setAnchorPoint(Vec2(0, 0));
    map->setPosition((Vec2(visibleSize.width / 2 + origin.x + 50, 0)));
    this->addChild(map, -1);

    map2 = cocos2d::TMXTiledMap::create("onlinemap.tmx");
    map2->setAnchorPoint(Vec2(0, 0));
    map2->setPosition((Vec2(30, 0)));
    this->addChild(map2, -1);
}

void onlineGame::addtime()
{
    strtime = StringUtils::format("%d s", Gametime);
    timelabel = Label::createWithTTF(strtime, "fonts/Marker Felt.ttf", 24);
    timelabel->setPosition(Vec2(250, visibleSize.height - 20));
    this->addChild(timelabel);
}

void onlineGame::addWall()
{
    //�߽�
    float bottom = -50;
    float over = 282;
    float right = 453;
    float left = 300;
    //�߽�Size
    Size WallSize = Size(right - left, over - bottom);

    auto WallBody = PhysicsBody::createEdgeBox(WallSize, PhysicsMaterial(0.0f, 1.0f, 0.0f), 1.0f);  //Wall�ĸ�������  ���ܶ� ��ȫ������ײ ��Ħ����  �����������Ǳ߿�Ŀ��

    Wall = Sprite::create();
    Wall->setAnchorPoint(Vec2(0, 0));
    Wall->setPosition(visibleSize.width / 2 + origin.x + 125, 115);
    Wall->setPhysicsBody(WallBody);
    addChild(Wall);

    float bottom2 = -50;
    float over2 = 282;
    float right2 = 453;
    float left2 = 300;
    //�߽�Size
    Size WallSize2 = Size(right2 - left2, over2 - bottom2);

    auto WallBody2 = PhysicsBody::createEdgeBox(WallSize2, PhysicsMaterial(0.0f, 1.0f, 0.0f), 1.0f);  //Wall�ĸ�������  ���ܶ� ��ȫ������ײ ��Ħ����  �����������Ǳ߿�Ŀ��

    Wall2 = Sprite::create();
    Wall2->setAnchorPoint(Vec2(0, 0));
    Wall2->setPosition(visibleSize.width / 2 + origin.x + 125, 115);
    Wall2->setPhysicsBody(WallBody2);
    addChild(Wall2);

}
//���̼�����
void onlineGame::addKeyListener()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(onlineGame::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(onlineGame::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdate();

    return;
}
//��ͣ��ť
void onlineGame::addbutton() {
    auto pause = MenuItemImage::create("progress/pause.png", "progress/pause.png", CC_CALLBACK_1(onlineGame::pauseClickCallBack, this));
    auto menu = Menu::create(pause, NULL);
    menu->setPosition(Vec2(visibleSize.width + 10, visibleSize.height - 20));
    addChild(menu);
}
//��ͣĿ¼
void onlineGame::pauseClickCallBack(Ref* pSender)
{
    Director::getInstance()->stopAnimation();

    pausebg = Sprite::create("progress/bg.png");
    pausebg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausebg, 10);

    MenuItemFont::setFontSize(20);

    auto ur1 = FileUtils::getInstance()->getStringFromFile("progress/continue.txt");
    auto con = MenuItemFont::create(ur1, CC_CALLBACK_1(onlineGame::pausemenuClickCallBack, this));

    auto ur2 = FileUtils::getInstance()->getStringFromFile("progress/back.txt");
    auto back = MenuItemFont::create(ur2, CC_CALLBACK_1(onlineGame::pausemenuClickCallBack, this));

    auto ur3 = FileUtils::getInstance()->getStringFromFile("progress/restart.txt");
    auto restart = MenuItemFont::create(ur3, CC_CALLBACK_1(onlineGame::pausemenuClickCallBack, this));

    //MainmenuTag
    con->setTag(1);
    back->setTag(2);
    restart->setTag(3);
    pausemenu = Menu::create(con, back, restart, NULL);
    pausemenu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausemenu, 11);
    pausemenu->alignItemsVertically();
}

void onlineGame::pausemenuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();

    if (NULL != node)
    {
        auto Gamemenuscene = Gamemenu2::createScene();
        auto Gamescene = onlineGame::createScene();
        switch (tag)
        {
        case 1:
            //����
            removeChild(pausebg);
            removeChild(pausemenu);
            Director::getInstance()->startAnimation();
            break;
        case 2:
            //����
            Director::getInstance()->startAnimation();
            Director::getInstance()->replaceScene(Gamemenuscene);
            break;
        case 3:
            //����
            Director::getInstance()->startAnimation();
            Director::getInstance()->replaceScene(Gamescene);
            break;
        default:
            break;
        }
    }
}

void onlineGame::addscore()
{
    auto label = Label::createWithTTF("score", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(300, visibleSize.height - 20));
    this->addChild(label);

    strscore = StringUtils::format("%d", score);
    scorelabel = Label::createWithTTF(strscore, "fonts/Marker Felt.ttf", 24);
    scorelabel->setPosition(Vec2(400, visibleSize.height - 20));
    this->addChild(scorelabel);

}

//ש����ײ�����
void onlineGame::addcontactListener()
{
    EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(onlineGame::onConcactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    scheduleUpdate();
    return;
}

void onlineGame::update(float delta) {
    Node* ball = this->getChildByTag(tagball);
    if (ball->getPositionY() <= 0)
    {
        ifstart = 0;
        score -= 10;
        removeChild(board);
        removeChild(ball);
        addBoard();
        addball();
        setJoint();
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }

    Node* board = this->getChildByTag(tagboard);

    //Node* ball  = this->getChildByTag(tagball);                 
    auto w = EventKeyboard::KeyCode::KEY_W;
    auto s = EventKeyboard::KeyCode::KEY_S;
    auto a = EventKeyboard::KeyCode::KEY_A;
    auto d = EventKeyboard::KeyCode::KEY_D;
    float offsetx = 0;
    float offsety = 0;

    //�޶�board�ƶ���Χ
    bool ifw = 1;            //�ܷ�wasd�ƶ�
    bool ifs = 1;
    bool ifa = 1;
    bool ifd = 1;

    float x = 0;
    float y = 0;

    x = board->getPositionX();
    y = board->getPositionY();

    float bottom = 5;
    float over = 282;
    float right = 385;
    float left = 290;

    if (y <= bottom) {
        board->setPositionY(bottom);
        ifs = 0;
    }
    if (y >= over) {
        board->setPositionY(over);
        ifw = 0;
    }
    if (x <= left) {
        board->setPositionX(left);
        ifa = 0;
    }
    if (x >= right) {
        board->setPositionX(right);
        ifd = 0;
    }
    if (keyMap[a] && ifa)
    {
        offsetx = -1 * speed;
    }
    if (keyMap[d] && ifd)
    {
        offsetx = speed;
    }
    if (keyMap[w] && ifw)
    {
        offsety = speed;
    }
    if (keyMap[s] && ifs)
    {
        offsety = -1 * speed;
    }

    if (offsetx == 0 && offsety == 0 && !keyMap[EventKeyboard::KeyCode::KEY_SPACE])
        return;
    auto movebyboard = MoveBy::create(0.2f, Vec2(offsetx, 0));
    board->runAction(movebyboard);

    DataSend();

    return;
}

void onlineGame::addBricks()
{
    TMXObjectGroup* objectGroup = map->getObjectGroup("bricks");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//�������ж���
    {
        ValueMap valueMap = value.asValueMap();//�������ֵ��Valueת����ValueMap
        float x = valueMap["x"].asFloat();//��ȡ���������:(asһ��ķ��� ��ת�����ͣ�
        float y = valueMap["y"].asFloat();
        //name/type/width/height
       //int hp = valueMap["hp"].asInt();
        int type = valueMap["type"].asInt();
        //���ש�龫��
        Sprite* it = Sprite::create("onlinegame/bluebrick.png"); //enemy = type == 4 ? Sprite::create("CloseNormal.png") : Sprite::create("CloseSelected.png");//����λ�����Դ�������
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(264 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //�������
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
        BrickBody->setGravityEnable(false);                    //��������Ӱ��

        //BrickBody->setTag(tagbrick);

        BrickBody->setRotationEnable(false);                  //�趨����ת
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);

        this->addChild(it);
        it->setTag(tagbrick);

        bricksnum++;
        Brickpath.push_back(Vec2(x, y));//��·���㱣�浽·����
    }

    TMXObjectGroup* objectGroup2 = map2->getObjectGroup("bricks");
    ValueVector values2 = objectGroup2->getObjects();

    for (Value value2 : values2)//�������ж���
    {
        ValueMap valueMap2 = value2.asValueMap();//�������ֵ��Valueת����ValueMap
        float x = valueMap2["x"].asFloat();//��ȡ���������:(asһ��ķ��� ��ת�����ͣ�
        float y = valueMap2["y"].asFloat();
        //name/type/width/height
       //int hp = valueMap["hp"].asInt();
        int type = valueMap2["type"].asInt();
        //���ש�龫��
        Sprite* it = Sprite::create("onlinegame/bluebrick.png"); //enemy = type == 4 ? Sprite::create("CloseNormal.png") : Sprite::create("CloseSelected.png");//����λ�����Դ�������
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(30 + x, y);


        auto BrickBody2 = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody2->setCategoryBitmask(0xFFFFFFFF);             //�������
        BrickBody2->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
        BrickBody2->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
        BrickBody2->setGravityEnable(false);                    //��������Ӱ��

        //BrickBody->setTag(tagbrick);

        BrickBody2->setRotationEnable(false);                  //�趨����ת
        BrickBody2->setDynamic(false);
        it->setPhysicsBody(BrickBody2);

        this->addChild(it);
        it->setTag(tagbrick);

        Brickpath.push_back(Vec2(x, y));//��·���㱣�浽·����
    }
}

void onlineGame::add2xbricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("2xbrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//�������ж���
    {
        ValueMap valueMap = value.asValueMap();//�������ֵ��Valueת����ValueMap
        float x = valueMap["x"].asFloat();//��ȡ���������:(asһ��ķ��� ��ת�����ͣ�
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //���ש�龫��
        Sprite* it = Sprite::create("onlinegame/brick2x.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(264 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //�������
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
        BrickBody->setGravityEnable(false);                    //��������Ӱ��


        BrickBody->setRotationEnable(false);                  //�趨����ת
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);


        addChild(it);
        it->setTag(tag2xbrick);

        bricksnum++;
        // Brickpath.push_back(Vec2(x, y));//��·���㱣�浽·����
    }
}

void onlineGame::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    //���̰���
    keyMap[keycode] = true;
    //��һ�ΰ��ո�ʼ����
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode) {

    }
}

void onlineGame::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //�����ͷ�
    keyMap[keycode] = false;

    //�ո�release �ͷ�С��
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode && ifstart == 0) {
        ifstart = 1;
        HitBrick_world->removeJoint(joint);
       ball->getPhysicsBody()->setVelocity(Vec2(0, startF));

    }
    return;
}

void onlineGame::setPhysicsWorld(PhysicsWorld* world)
{
    HitBrick_world = world;
    return;
}

bool onlineGame::onConcactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    Node* ball = getChildByTag(tagball);
    //�˶�����
    float angle = ball->getPhysicsBody()->getRotation() * 2 / 360 * 3.1415f;
    Vec2 dir = dir = Vec2(sin(angle), cos(angle));
    if (nodeA->getTag() == tagbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    else if (nodeA->getTag() == tagbrick && nodeB->getTag() == tagball2) {
        removeChild(nodeA);
        //����score
        strscore2 = StringUtils::format("%d", score2);
        scorelabel2->setString(strscore2);
    }
    else if (nodeB->getTag() == tagbrick && nodeA->getTag() == tagball2) {
        removeChild(nodeB);
        //����score
        strscore2 = StringUtils::format("%d", score2);
        scorelabel2->setString(strscore2);
    }
    //2xbrick
    else if (nodeA->getTag() == tag2xbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        perscore = 2;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tag2xbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        perscore = 2;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    else if (nodeA->getTag() == tag2xbrick && nodeB->getTag() == tagball2) {
        removeChild(nodeA);
        //����score
        strscore2 = StringUtils::format("%d", score2);
        scorelabel2->setString(strscore2);
    }
    else if (nodeB->getTag() == tag2xbrick && nodeA->getTag() == tagball2) {
        removeChild(nodeB);
        //����score
        strscore2 = StringUtils::format("%d", score2);
        scorelabel2->setString(strscore2);
    }
    if (bricksnum == 0)
    {
        Gameover();
    }

    return true;
}

void onlineGame::DataSend()
{
    std::string dataSend = "";
    //��map���½ǵ����λ��
    dataSend += DataCMP(static_cast<int>(board->getPositionX() - map->getPositionX()));
    dataSend += DataCMP(static_cast<int>(board->getPositionY() - map->getPositionY()));
    dataSend += DataCMP(static_cast<int>(ball->getPositionX() - map->getPositionX()));
    dataSend += DataCMP(static_cast<int>(ball->getPositionY() - map->getPositionY()));

    send(client, dataSend.c_str(), dataSend.size(), 0);
}

std::string onlineGame::DataCMP(int value)
{
    char buff[8];
    snprintf(buff, sizeof(buff), "%8d", value);
    return buff;
}
void onlineGame::setJoint()
{
    joint = PhysicsJointFixed::construct(ball->getPhysicsBody(), board->getPhysicsBody(), board->getAnchorPoint());
    HitBrick_world->addJoint(joint);
}

void onlineGame::Gameover()
{
    //ͣ�����м����
    _eventDispatcher->removeAllEventListeners();
    ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
    board->getPhysicsBody()->setVelocity(Vec2(0, 0));
    if (bricksnum == 0) {
        //���success
        auto label1 = Label::createWithTTF("Success!", "fonts/Marker Felt.ttf", 60);
        label1->setColor(Color3B(0, 0, 0));
        label1->setPosition(visibleSize.width / 2 + origin.x + 28, visibleSize.height / 2);
        this->addChild(label1, 1);
    }
    else {
        auto lab = Label::createWithTTF("DEFEAT!", "fonts/Marker Felt.ttf", 60);
        lab->setColor(Color3B(0, 0, 0));
        lab->setPosition(visibleSize.width / 2 + origin.x + 28, visibleSize.height / 2);
        this->addChild(lab, 1);
    }
    UserDefault::getInstance()->flush();

    //��ʱ���뷵��Gamemenu
    schedule(SEL_SCHEDULE(&onlineGame::backGamemenu), 2.0f);
}

void onlineGame::backGamemenu(float dt) {
    auto Gamescene = Gamemenu2::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}

void onlineGame::timer(float dt) {
    Gametime += 1;
    strtime = StringUtils::format("%d s", Gametime);
    timelabel->setString(strtime);
}