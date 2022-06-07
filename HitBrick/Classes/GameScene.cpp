#include "cocos2d.h"
#include "store.h"
#include "Gamemenu2.h"
#include "Gamemenu.h"
#include "AudioEngine.h"
#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    srand((unsigned)time(NULL));
    
    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setAutoStep(true);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
    auto layer = GameScene::create();

    layer->setPhysicsWorld(scene->getPhysicsWorld());
    layer->setJoint();
    scene->addChild(layer);
    return scene;
}
bool GameScene::init()
{
    Gamemodel = UserDefault::getInstance()->getIntegerForKey("Gamemodel");
    if (!Layer::init())
    {
        return false;
    }
    
    UserDefault::getInstance()->flush();
    visibleSize = Director::getInstance()->getVisibleSize();                              //��ȡ��Ļ����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    ifstart = 0;   //δ����
    startF = 0;    //

    //��ӵ�ͼ
    addmap();
    //��Ӱ�
    addBoard();
    //�����
    addball();
    //���Wall
    addWall();
    //���ש��
    addBricks();
    add2xbricks(); //���2��ש��
    addslowbricks(); //��Ӽ���ש��
    addcrazybricks(); //��ӷ��ש��
    addstaticbricks(); //��ӹ̶�ש��
    //�����ͣ��ť
    addbutton();
    //��ӷ���
    addscore();
    //�����߷�
    addhscore();
    //���ʱ��
    addtime();

    LeisureGame LG;
    SpeedGame SG;
    switch (Gamemodel) {
    case 1:
        //��Ӽ��̼�����
        LG.addKeyListener();
        //�����ײ������
        LG.addcontactListener();

        LG.addkuang();

        scheduleUpdate();
        break;
    case 2:
        //��Ӽ��̼�����
        SG.addKeyListener();
        //�����ײ������
        SG.addcontactListener();

        scheduleUpdate();

        schedule(SEL_SCHEDULE(&GameScene::timer), 1.0f);
        break;
    }
}
bool LeisureGame::myinit()
{
   

    return true;
}
bool SpeedGame::myinit()
{
    //��Ӽ��̼�����
    addKeyListener();
    //�����ײ������
    addcontactListener();

    scheduleUpdate();

    schedule(SEL_SCHEDULE(&GameScene::timer), 1.0f);
    return true;
}

void GameScene::addBackGround()
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

void GameScene::addBoard()
{
    board = Sprite::create("board/board.png");
    board->setAnchorPoint(Vec2(0.5f, 0.5f));
    board->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + 50));

    board->setTag(tagboard);           //�����̼�������λ


    //��Ӱ�ĸ�������
    auto BoardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));

    BoardBody->setCategoryBitmask(0xFFFFFFFF);           //�������
    BoardBody->setCollisionBitmask(0xFFFFFFFF);           //����ײ��  ��ײ����
    BoardBody->setContactTestBitmask(0xFFFFFFFF);        //�ɽӵ�֪ͨ  �Ӵ�����
    BoardBody->setGravityEnable(false);                  //��������Ӱ��
    BoardBody->setTag(tagboard);
    //ֻ�м�����CategoryBitmask����Է���CollisionBitmask�����㲻Ϊ0
    //���ҶԷ���CategoryBitmask���뼺����CollisionBitmask�����㲻Ϊ0���Żᷢ����ײ
    board->setPhysicsBody(BoardBody);

    board->getPhysicsBody()->setRotationEnable(false);   //�趨����ת


    this->addChild(board, tagboard);


    return;
}

void GameScene::addball()
{
    ball = Sprite::create("ball/ball.png");
    ball->setScale(2.0f, 2.0f);
    ball->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + 48 + board->getContentSize().height));

    // ������ĸ�������
    auto ballBody = PhysicsBody::createCircle(ball->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

    ballBody->setCategoryBitmask(0xFFFFFFFF);             //�������
    ballBody->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
    ballBody->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
    ballBody->setGravityEnable(false);                    //��������Ӱ��

    //ballBody->setTag(tagball);

    ballBody->setRotationEnable(false);                  //�趨����ת

    ball->setPhysicsBody(ballBody);
    ball->setTag(tagball);
    addChild(ball);
}

void GameScene::addmap()
{
    //ѡ���ͼ  
    UserDefault::getInstance()->flush();
    Gamechoice = UserDefault::getInstance()->getIntegerForKey("Gamechoice", 0);
    switch (Gamechoice) {
    case 1:
        map = cocos2d::TMXTiledMap::create("map.tmx");
        break;
    case 2:
        map = cocos2d::TMXTiledMap::create("map2.tmx");
        break;
    case 3:
        map = cocos2d::TMXTiledMap::create("map3.tmx");
        break;
    case 4:
        map = cocos2d::TMXTiledMap::create("map4.tmx");
        break;
    default:
        break;
    }
    map->setAnchorPoint(Vec2(0.5f, 0));
    map->setPosition((Vec2(visibleSize.width / 2 + origin.x + 28, 0)));
    this->addChild(map, -1);

}

void GameScene::addtime()
{
    strtime = StringUtils::format("%d s", Gametime);
    timelabel = Label::createWithTTF(strtime, "fonts/Marker Felt.ttf", 24);
    timelabel->setPosition(Vec2(350, visibleSize.height - 20));
    this->addChild(timelabel);
}

void GameScene::addWall()
{
    //�߽�
    float bottom = -50;
    float over = 282;
    float right = 453;
    float left = 95;
    //�߽�Size
    Size WallSize = Size(right - left, over - bottom);

    auto WallBody = PhysicsBody::createEdgeBox(WallSize, PhysicsMaterial(0.0f, 1.0f, 0.0f), 0.1f);  //Wall�ĸ�������  ���ܶ� ��ȫ������ײ ��Ħ����  �����������Ǳ߿�Ŀ��

    Wall = Sprite::create();
    Wall->setAnchorPoint(Vec2(0.5, 0));
    Wall->setPosition(visibleSize.width / 2 + origin.x + 28, 115);
    Wall->setPhysicsBody(WallBody);
    addChild(Wall);

}

//���̼�����
void LeisureGame::addKeyListener()
{
    auto keyboardListener = EventListenerKeyboard::create();

    keyboardListener->onKeyPressed = CC_CALLBACK_2(LeisureGame::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(LeisureGame::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdate();

    return;
}
void SpeedGame::addKeyListener()
{
    auto keyboardListener = EventListenerKeyboard::create();

    keyboardListener->onKeyPressed = CC_CALLBACK_2(SpeedGame::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(SpeedGame::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdate();

    return;
}
void LeisureGame::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    //���̰���
    keyMap[keycode] = true;
    //��һ�ΰ��ո�ʼ����
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode && ifstart == 0) {
        ifstart = 1;    //��update����������
    }
}
void SpeedGame::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    //���̰���
    keyMap[keycode] = true;
    //��һ�ΰ��ո�ʼ����
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode && ifstart == 0) {
        ifstart = 1;    //��update����������
    }
}
void LeisureGame::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //�����ͷ�
    keyMap[keycode] = false;

    //�ո�release ֹͣ�����ͷ�С��
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode) {
        if (ifstart == 1) {
            ifstart = -1;          //С��������
            HitBrick_world->removeJoint(joint);
            ball->getPhysicsBody()->setVelocity(Vec2(0, startF * 4));
            AudioEngine::play2d("biu.mp3", false, 1.0);
            removeChild(kuang);
        }
    }
    return;
}
void SpeedGame::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //�����ͷ�
    keyMap[keycode] = false;

    //�ո�release ֹͣ�����ͷ�С��
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode) {
        if (ifstart == 1) {
            ifstart = -1;          //С��������
            HitBrick_world->removeJoint(joint);
            ball->getPhysicsBody()->setVelocity(Vec2(0, startF * 4));
            AudioEngine::play2d("biu.mp3", false, 1.0);
        }
    }
    return;
}


//��ͣ��ť
void GameScene::addbutton() {
    auto pause = MenuItemImage::create("progress/pause.png", "progress/pause.png", CC_CALLBACK_1(GameScene::pauseClickCallBack, this));
    auto menu = Menu::create(pause, NULL);
    menu->setPosition(Vec2(visibleSize.width - 30, visibleSize.height - 20));
    addChild(menu);
}
//��ͣĿ¼
void GameScene::pauseClickCallBack(Ref* pSender)
{
    Director::getInstance()->stopAnimation();

    pausebg = Sprite::create("progress/bg.png");
    pausebg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausebg, 10);

    MenuItemFont::setFontSize(20);

    auto ur1 = FileUtils::getInstance()->getStringFromFile("progress/continue.txt");
    auto con = MenuItemFont::create(ur1, CC_CALLBACK_1(GameScene::pausemenuClickCallBack, this));

    auto ur2 = FileUtils::getInstance()->getStringFromFile("progress/back.txt");
    auto back = MenuItemFont::create(ur2, CC_CALLBACK_1(GameScene::pausemenuClickCallBack, this));

    auto ur3 = FileUtils::getInstance()->getStringFromFile("progress/restart.txt");
    auto restart = MenuItemFont::create(ur3, CC_CALLBACK_1(GameScene::pausemenuClickCallBack, this));

    //MainmenuTag
    con->setTag(1);
    back->setTag(2);
    restart->setTag(3);
    pausemenu = Menu::create(con, back, restart, NULL);
    pausemenu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausemenu, 11);
    pausemenu->alignItemsVertically();
}

void GameScene::pausemenuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();

    if (NULL != node)
    {
        auto Gamemenuscene = Gamemenu2::createScene();
        auto Gamescene = GameScene::createScene();
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

void GameScene::addscore()
{
    auto label = Label::createWithTTF("score", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(50, visibleSize.height - 20));
    this->addChild(label);

    strscore = StringUtils::format("%d", score);
    scorelabel = Label::createWithTTF(strscore, "fonts/Marker Felt.ttf", 24);
    scorelabel->setPosition(Vec2(150, visibleSize.height - 20));
    this->addChild(scorelabel);

}
//�����߷�
void GameScene::addhscore()
{
    auto url = FileUtils::getInstance()->getStringFromFile("progress/hscore.txt");
    auto label = Label::create();
    label->setString(url);
    label->setPosition(Vec2(200, visibleSize.height - 20));
    this->addChild(label);

    //ȡ����߷�
    std::string hstrscore;
    switch (Gamechoice) {
    case 1:
        hstrscore = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s1hscore", 0));
        break;
    case 2:
        hstrscore = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s2hscore", 0));
        break;
    case 3:
        hstrscore = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s3hscore", 0));
        break;
    case 4:
        hstrscore = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s4hscore", 0));
        break;
    default:
        break;
    }
    auto hscorelabel = Label::createWithTTF(hstrscore, "fonts/Marker Felt.ttf", 24);
    hscorelabel->setPosition(Vec2(250, visibleSize.height - 20));
    this->addChild(hscorelabel);

}

//ש����ײ�����
void LeisureGame::addcontactListener()
{
    EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(LeisureGame::onConcactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    scheduleUpdate();
    return;
}
void SpeedGame::addcontactListener()
{
    EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(SpeedGame::onConcactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    scheduleUpdate();
    return;
}
bool LeisureGame::onConcactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
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
    //slowbrick
    else if (nodeA->getTag() == tagslowbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->setVelocity(120 * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagslowbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->setVelocity(120 * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    //crazybrick
    else if (nodeA->getTag() == tagcrazybrick && nodeB->getTag() == tagball) {
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagcrazybrick && nodeA->getTag() == tagball) {
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    else if (nodeA->getTag() == tagcrazybrick && nodeB->getTag() == tagbrick) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagcrazybrick && nodeA->getTag() == tagbrick) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    if (bricksnum == 0)
    {
        Gameover();
    }
    return true;
}
bool SpeedGame::onConcactBegin(PhysicsContact& contact) {
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
    //slowbrick
    else if (nodeA->getTag() == tagslowbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->setVelocity(120 * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagslowbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->setVelocity(120 * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    //crazybrick
    else if (nodeA->getTag() == tagcrazybrick && nodeB->getTag() == tagball) {
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagcrazybrick && nodeA->getTag() == tagball) {
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    else if (nodeA->getTag() == tagcrazybrick && nodeB->getTag() == tagbrick) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagcrazybrick && nodeA->getTag() == tagbrick) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����
        //����score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    if (bricksnum == 0)
    {
        Gameover();
    }

    return true;
}

void LeisureGame::update(float delta) {
    Node* board = this->getChildByTag(tagboard);
    //Node* ball  = this->getChildByTag(tagball);                    ???Ϊʲôball �� board ͬʱ���ᱨ������
    auto w = EventKeyboard::KeyCode::KEY_W;
    auto s = EventKeyboard::KeyCode::KEY_S;
    auto a = EventKeyboard::KeyCode::KEY_A;
    auto d = EventKeyboard::KeyCode::KEY_D;
    auto p = EventKeyboard::KeyCode::KEY_P;
    int offsetx = 0;
    int offsety = 0;

    //�޶�board�ƶ���Χ
    bool ifw = 1;            //�ܷ�wasd�ƶ�
    bool ifs = 1;
    bool ifa = 1;
    bool ifd = 1;

    int x = 0;
    int y = 0;

    x = board->getPositionX();
    y = board->getPositionY();

    int bottom = 5;
    int over = 100;
    int right = 385;
    int left = 95;

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
    if (keyMap[p]) {
        ifstart = 0;
        score -= 1;
        removeChild(board);
        removeChild(ball);
        addBoard();
        addball();
        setJoint();
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }

    if (offsetx == 0 && offsety == 0 && !keyMap[EventKeyboard::KeyCode::KEY_SPACE])
        return;
    auto movebyboard = MoveBy::create(0.2f, Vec2(offsetx, offsety));
    board->runAction(movebyboard);

    //SPACE  ball����
    if (ifstart == 1 && startF <= 150) {
        startF += 1;
        kuang->setColor(Color3B(220 * startF / 150, 255 - 235 * startF / 150, 255 - 195 * startF / 150));
    }


    return;
}
void SpeedGame::update(float delta) {
    Node* ball = this->getChildByTag(tagball);
    if (ball->getPositionY() <= 0)
    {
        unscheduleUpdate();
        Gameover();
    }

    Node* board = this->getChildByTag(tagboard);

    //Node* ball  = this->getChildByTag(tagball);                 
    auto w = EventKeyboard::KeyCode::KEY_W;
    auto s = EventKeyboard::KeyCode::KEY_S;
    auto a = EventKeyboard::KeyCode::KEY_A;
    auto d = EventKeyboard::KeyCode::KEY_D;
    auto p = EventKeyboard::KeyCode::KEY_P;
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
    float over = 100;
    float right = 385;
    float left = 95;

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
    if (keyMap[p]) {
        ifstart = 0;
        score -= 1;
        removeChild(board);
        removeChild(ball);
        addBoard();
        addball();
        setJoint();
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }

    if (offsetx == 0 && offsety == 0 && !keyMap[EventKeyboard::KeyCode::KEY_SPACE])
        return;
    auto movebyboard = MoveBy::create(0.2f, Vec2(offsetx, offsety));
    board->runAction(movebyboard);

    //SPACE  ball //ȡ������
    if (ifstart == 1 && startF <= 30) {
        //��ͬ�ؿ�С���ʼ�ٶȲ�ͬ
        switch (Gamechoice) {
        case 1:
            startF = 50;
            break;
        case 2:
            startF = 65;
            break;
        case 3:
            startF = 60;
            break;
        case 4:
            startF = 70;
            break;
        }

    }


    return;
}
//���ש��
void GameScene::addBricks()
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
        Sprite* it = Sprite::create("bluebrick.png"); //enemy = type == 4 ? Sprite::create("CloseNormal.png") : Sprite::create("CloseSelected.png");//����λ�����Դ�������
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


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
}
void GameScene::add2xbricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("2xbrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//�������ж���
    {
        ValueMap valueMap = value.asValueMap();//�������ֵ��Valueת����ValueMap
        float x = valueMap["x"].asFloat();//��ȡ���������:(asһ��ķ��� ��ת�����ͣ�
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //���ש�龫��
        Sprite* it = Sprite::create("brick2x.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


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
void GameScene::addslowbricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("slowbrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//�������ж���
    {
        ValueMap valueMap = value.asValueMap();//�������ֵ��Valueת����ValueMap
        float x = valueMap["x"].asFloat();//��ȡ���������:(asһ��ķ��� ��ת�����ͣ�
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //���ש�龫��
        Sprite* it = Sprite::create("orangebrick.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //�������
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
        BrickBody->setGravityEnable(false);                    //��������Ӱ��


        BrickBody->setRotationEnable(false);                  //�趨����ת
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);


        addChild(it);
        it->setTag(tagslowbrick);

        bricksnum++;
        // Brickpath.push_back(Vec2(x, y));//��·���㱣�浽·����
    }
}
void GameScene::addcrazybricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("crazybrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//�������ж���
    {
        ValueMap valueMap = value.asValueMap();//�������ֵ��Valueת����ValueMap
        float x = valueMap["x"].asFloat();//��ȡ���������:(asһ��ķ��� ��ת�����ͣ�
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //���ש�龫��
        Sprite* it = Sprite::create("crazybrick.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //�������
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
        BrickBody->setGravityEnable(false);                    //��������Ӱ��


        BrickBody->setRotationEnable(false);                  //�趨����ת
        //BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);

        addChild(it);
        it->setTag(tagcrazybrick);
    }
}
void GameScene::addstaticbricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("staticbrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//�������ж���
    {
        ValueMap valueMap = value.asValueMap();//�������ֵ��Valueת����ValueMap
        float x = valueMap["x"].asFloat();//��ȡ���������:(asһ��ķ��� ��ת�����ͣ�
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //���ש�龫��
        Sprite* it = Sprite::create("staticbrick.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //�������
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //����ײ��
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //�ɽӵ�֪ͨ
        BrickBody->setGravityEnable(false);                    //��������Ӱ��


        BrickBody->setRotationEnable(false);                  //�趨����ת
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);

        addChild(it);
        it->setTag(tagstaticbrick);
    }
}


void GameScene::setPhysicsWorld(PhysicsWorld* world)
{
    HitBrick_world = world;
    return;
}


void GameScene::setJoint()
{
    joint = PhysicsJointFixed::construct(ball->getPhysicsBody(), board->getPhysicsBody(), board->getAnchorPoint());
    HitBrick_world->addJoint(joint);
}

void LeisureGame::Gameover()
{
    //ͣ�����м����
    _eventDispatcher->removeAllEventListeners();
    ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
    board->getPhysicsBody()->setVelocity(Vec2(0, 0));
    if (bricksnum == 0) {
        //���success
        auto label1 = Label::createWithTTF("Success!~", "fonts/Marker Felt.ttf", 60);
        label1->setColor(Color3B(0, 0, 0));
        label1->setPosition(visibleSize.width / 2 + origin.x + 28, visibleSize.height / 2);
        this->addChild(label1, 1);

        switch (Gamechoice) {
        case 1:
            UserDefault::getInstance()->setIntegerForKey("y1hscore", score);
            break;
        case 2:
            UserDefault::getInstance()->setIntegerForKey("y2hscore", score);
            break;
        case 3:
            UserDefault::getInstance()->setIntegerForKey("y3hscore", score);
            break;
        default:
            break;
        }
        UserDefault::getInstance()->flush();
        AudioEngine::stopAll();
        AudioEngine::play2d("win.mp3", false, 1.0);
        //��ʱ���뷵��Gamemenu
        schedule(SEL_SCHEDULE(&GameScene::backGamemenu), 5.0f);
    }
}
void SpeedGame::Gameover()
{
    //ͣ�����м����
    _eventDispatcher->removeAllEventListeners();
    ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
    board->getPhysicsBody()->setVelocity(Vec2(0, 0));
    if (bricksnum == 0) {
        //���success
        auto label1 = Label::createWithTTF("Success!", "fonts/Marker Felt.ttf", 60);
        AudioEngine::stopAll();
        AudioEngine::play2d("win.mp3", true, 1.0);
        label1->setColor(Color3B(0, 0, 0));
        label1->setPosition(visibleSize.width / 2 + origin.x + 28, visibleSize.height / 2);
        this->addChild(label1, 1);
    }
    else {
        auto lab = Label::createWithTTF("DEFEAT!", "fonts/Marker Felt.ttf", 60);
        AudioEngine::stopAll();
        AudioEngine::play2d("defeat.mp3", true, 0.5);
        lab->setColor(Color3B(0, 0, 0));
        lab->setPosition(visibleSize.width / 2 + origin.x + 28, visibleSize.height / 2);
        this->addChild(lab, 1);
    }

    settlement();
    UserDefault::getInstance()->flush();

    //��ʱ���뷵��Gamemenu
    schedule(SEL_SCHEDULE(&GameScene::backGamemenu), 5.0f);
}
//���а����
void SpeedGame::settlement()
{
    ranks rank[6];

    switch (Gamechoice) {
    case 1:
        rank[0].score = score;
        rank[1].score = UserDefault::getInstance()->getIntegerForKey("s1hscore1");
        rank[2].score = UserDefault::getInstance()->getIntegerForKey("s1hscore2");
        rank[3].score = UserDefault::getInstance()->getIntegerForKey("s1hscore3");
        rank[4].score = UserDefault::getInstance()->getIntegerForKey("s1hscore4");
        rank[5].score = UserDefault::getInstance()->getIntegerForKey("s1hscore5");

        rank[0].times = Gametime;
        rank[1].times = UserDefault::getInstance()->getIntegerForKey("s1htime1");
        rank[2].times = UserDefault::getInstance()->getIntegerForKey("s1htime2");
        rank[3].times = UserDefault::getInstance()->getIntegerForKey("s1htime3");
        rank[4].times = UserDefault::getInstance()->getIntegerForKey("s1htime4");
        rank[5].times = UserDefault::getInstance()->getIntegerForKey("s1htime5");
        break;
    case 2:
        rank[0].score = score;
        rank[1].score = UserDefault::getInstance()->getIntegerForKey("s2hscore1");
        rank[2].score = UserDefault::getInstance()->getIntegerForKey("s2hscore2");
        rank[3].score = UserDefault::getInstance()->getIntegerForKey("s2hscore3");
        rank[4].score = UserDefault::getInstance()->getIntegerForKey("s2hscore4");
        rank[5].score = UserDefault::getInstance()->getIntegerForKey("s2hscore5");

        rank[0].times = Gametime;
        rank[1].times = UserDefault::getInstance()->getIntegerForKey("s2htime1");
        rank[2].times = UserDefault::getInstance()->getIntegerForKey("s2htime2");
        rank[3].times = UserDefault::getInstance()->getIntegerForKey("s2htime3");
        rank[4].times = UserDefault::getInstance()->getIntegerForKey("s2htime4");
        rank[5].times = UserDefault::getInstance()->getIntegerForKey("s2htime5");

        break;
    case 3:
        rank[0].score = score;
        rank[1].score = UserDefault::getInstance()->getIntegerForKey("s3hscore1");
        rank[2].score = UserDefault::getInstance()->getIntegerForKey("s3hscore2");
        rank[3].score = UserDefault::getInstance()->getIntegerForKey("s3hscore3");
        rank[4].score = UserDefault::getInstance()->getIntegerForKey("s3hscore4");
        rank[5].score = UserDefault::getInstance()->getIntegerForKey("s3hscore5");

        rank[0].times = Gametime;
        rank[1].times = UserDefault::getInstance()->getIntegerForKey("s3htime1");
        rank[2].times = UserDefault::getInstance()->getIntegerForKey("s3htime2");
        rank[3].times = UserDefault::getInstance()->getIntegerForKey("s3htime3");
        rank[4].times = UserDefault::getInstance()->getIntegerForKey("s3htime4");
        rank[5].times = UserDefault::getInstance()->getIntegerForKey("s3htime5");

        break;
    case 4:
        rank[0].score = score;
        rank[1].score = UserDefault::getInstance()->getIntegerForKey("s4hscore1");
        rank[2].score = UserDefault::getInstance()->getIntegerForKey("s4hscore2");
        rank[3].score = UserDefault::getInstance()->getIntegerForKey("s4hscore3");
        rank[4].score = UserDefault::getInstance()->getIntegerForKey("s4hscore4");
        rank[5].score = UserDefault::getInstance()->getIntegerForKey("s4hscore5");

        rank[0].times = Gametime;
        rank[1].times = UserDefault::getInstance()->getIntegerForKey("s4htime1");
        rank[2].times = UserDefault::getInstance()->getIntegerForKey("s4htime2");
        rank[3].times = UserDefault::getInstance()->getIntegerForKey("s4htime3");
        rank[4].times = UserDefault::getInstance()->getIntegerForKey("s4htime4");
        rank[5].times = UserDefault::getInstance()->getIntegerForKey("s4htime5");

        break;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5 - i; j++) {
            if (rank[j].score < rank[j + 1].score) {
                ranks temp = rank[j];
                rank[j] = rank[j + 1];
                rank[j + 1] = temp;
            }
            else if ((rank[j].score == rank[j + 1].score) && (rank[j].times > rank[j + 1].times))
            {
                ranks temp = rank[j];
                rank[j] = rank[j + 1];
                rank[j + 1] = temp;
            }
        }
    }

    switch (Gamechoice) {
    case 1:
        UserDefault::getInstance()->setIntegerForKey("s1hscore1", rank[0].score);
        UserDefault::getInstance()->setIntegerForKey("s1hscore2", rank[1].score);
        UserDefault::getInstance()->setIntegerForKey("s1hscore3", rank[2].score);
        UserDefault::getInstance()->setIntegerForKey("s1hscore4", rank[3].score);
        UserDefault::getInstance()->setIntegerForKey("s1hscore5", rank[4].score);

        UserDefault::getInstance()->setIntegerForKey("s1htime1", rank[0].times);
        UserDefault::getInstance()->setIntegerForKey("s1htime2", rank[1].times);
        UserDefault::getInstance()->setIntegerForKey("s1htime3", rank[2].times);
        UserDefault::getInstance()->setIntegerForKey("s1htime4", rank[3].times);
        UserDefault::getInstance()->setIntegerForKey("s1htime5", rank[4].times);

        break;
    case 2:
        UserDefault::getInstance()->setIntegerForKey("s2hscore1", rank[0].score);
        UserDefault::getInstance()->setIntegerForKey("s2hscore2", rank[1].score);
        UserDefault::getInstance()->setIntegerForKey("s2hscore3", rank[2].score);
        UserDefault::getInstance()->setIntegerForKey("s2hscore4", rank[3].score);
        UserDefault::getInstance()->setIntegerForKey("s2hscore5", rank[4].score);

        UserDefault::getInstance()->setIntegerForKey("s2htime1", rank[0].times);
        UserDefault::getInstance()->setIntegerForKey("s2htime2", rank[1].times);
        UserDefault::getInstance()->setIntegerForKey("s2htime3", rank[2].times);
        UserDefault::getInstance()->setIntegerForKey("s2htime4", rank[3].times);
        UserDefault::getInstance()->setIntegerForKey("s2htime5", rank[4].times);
        break;
    case 3:
        UserDefault::getInstance()->setIntegerForKey("s3hscore1", rank[0].score);
        UserDefault::getInstance()->setIntegerForKey("s3hscore2", rank[1].score);
        UserDefault::getInstance()->setIntegerForKey("s3hscore3", rank[2].score);
        UserDefault::getInstance()->setIntegerForKey("s3hscore4", rank[3].score);
        UserDefault::getInstance()->setIntegerForKey("s3hscore5", rank[4].score);

        UserDefault::getInstance()->setIntegerForKey("s3htime1", rank[0].times);
        UserDefault::getInstance()->setIntegerForKey("s3htime2", rank[1].times);
        UserDefault::getInstance()->setIntegerForKey("s3htime3", rank[2].times);
        UserDefault::getInstance()->setIntegerForKey("s3htime4", rank[3].times);
        UserDefault::getInstance()->setIntegerForKey("s3htime5", rank[4].times);
        break;
    case 4:
        UserDefault::getInstance()->setIntegerForKey("s4hscore1", rank[0].score);
        UserDefault::getInstance()->setIntegerForKey("s4hscore2", rank[1].score);
        UserDefault::getInstance()->setIntegerForKey("s4hscore3", rank[2].score);
        UserDefault::getInstance()->setIntegerForKey("s4hscore4", rank[3].score);
        UserDefault::getInstance()->setIntegerForKey("s4hscore5", rank[4].score);

        UserDefault::getInstance()->setIntegerForKey("s4htime1", rank[0].times);
        UserDefault::getInstance()->setIntegerForKey("s4htime2", rank[1].times);
        UserDefault::getInstance()->setIntegerForKey("s4htime3", rank[2].times);
        UserDefault::getInstance()->setIntegerForKey("s4htime4", rank[3].times);
        UserDefault::getInstance()->setIntegerForKey("s4htime5", rank[4].times);
        break;
    default:
        break;
    }
}

void GameScene::backGamemenu(float dt) {
    AudioEngine::stopAll();
    AudioEngine::play2d("Gamemusic.mp3", true, 1.0);
    auto Gamescene2 = Gamemenu2::createScene();
    auto Gamescene1 = Gamemenu::createScene();
    switch (Gamemodel) 
    {
    case 1:
        Director::getInstance()->replaceScene(Gamescene1);
        break;
    case 2:
        Director::getInstance()->replaceScene(Gamescene2);
        break;
    }
}

void GameScene::timer(float dt) {
    Gametime += 1;
    strtime = StringUtils::format("%d s", Gametime);
    timelabel->setString(strtime);
}

void LeisureGame::addkuang() {
    kuang = Sprite::create("kuang.png");
    kuang->setAnchorPoint(Vec2(0, 0));
    kuang->setPosition(Vec2(30, 10));
    kuang->setColor(Color3B::GREEN);
    kuang->setTag(tagkuang);           //�����̼�������λ
    addChild(kuang);
}