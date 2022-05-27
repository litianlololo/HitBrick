#include "cocos2d.h"
#include "SpeedGame.h"
#include "store.h"
#include "Gamemenu2.h"

//#include "Gamemenu.cpp"

USING_NS_CC;

Scene* HitBrick2::createScene() {

    srand((unsigned)time(NULL));

    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setAutoStep(true);


    scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
    auto layer = HitBrick2::create();

    layer->setPhysicsWorld(scene->getPhysicsWorld());
    layer->setJoint();
    scene->addChild(layer);
    return scene;
}

bool HitBrick2::init()
{
    if (!Layer::init())
    {
        return false;
    }
    UserDefault::getInstance()->flush();
    visibleSize = Director::getInstance()->getVisibleSize();                              //��ȡ��Ļ����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /*
    //��ӱ���
    //addBackGround();
    label = Label::create();
    label->setString(" ");
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
       300));
    this->addChild(label,5);
    */

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
    //����±߽�
  //  addbottomborder();
    //�����ͣ��ť
    addbutton();
    //��ӷ���
    addscore();
    //�����߷�
    addhscore();
    //���ʱ��
    addtime();
    //��Ӽ��̼�����
    addKeyListener();
    //�����ײ������
    addcontactListener();

    scheduleUpdate();
    
    schedule(SEL_SCHEDULE(&HitBrick2::timer), 1.0f);
    return true;
}

void HitBrick2::addBackGround()
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

void HitBrick2::addBoard()
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

void HitBrick2::addball()
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

void HitBrick2::addmap()
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
    default:
        //map = cocos2d::TMXTiledMap::create("map3.tmx");
        break;
    }
    map->setAnchorPoint(Vec2(0.5f, 0));
    map->setPosition((Vec2(visibleSize.width / 2 + origin.x + 28, 0)));
    this->addChild(map, -1);

}

void HitBrick2::addtime()
{
    strtime = StringUtils::format("%d s", Gametime);
    timelabel = Label::createWithTTF(strtime, "fonts/Marker Felt.ttf", 24);
    timelabel->setPosition(Vec2(350, visibleSize.height - 20));
    this->addChild(timelabel);
}

void HitBrick2::addWall()
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
void HitBrick2::addKeyListener()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(HitBrick2::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(HitBrick2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdate();

    return;
}
//��ͣ��ť
void HitBrick2::addbutton() {
    auto pause = MenuItemImage::create("progress/pause.png", "progress/pause.png", CC_CALLBACK_1(HitBrick2::pauseClickCallBack, this));
    auto menu = Menu::create(pause, NULL);
    menu->setPosition(Vec2(visibleSize.width - 30, visibleSize.height - 20));
    addChild(menu);
}
//��ͣĿ¼
void HitBrick2::pauseClickCallBack(Ref* pSender)
{
    Director::getInstance()->stopAnimation();

    pausebg = Sprite::create("progress/bg.png");
    pausebg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausebg, 10);

    MenuItemFont::setFontSize(20);

    auto ur1 = FileUtils::getInstance()->getStringFromFile("progress/continue.txt");
    auto con = MenuItemFont::create(ur1, CC_CALLBACK_1(HitBrick2::pausemenuClickCallBack, this));

    auto ur2 = FileUtils::getInstance()->getStringFromFile("progress/back.txt");
    auto back = MenuItemFont::create(ur2, CC_CALLBACK_1(HitBrick2::pausemenuClickCallBack, this));

    auto ur3 = FileUtils::getInstance()->getStringFromFile("progress/restart.txt");
    auto restart = MenuItemFont::create(ur3, CC_CALLBACK_1(HitBrick2::pausemenuClickCallBack, this));

    //MainmenuTag
    con->setTag(1);
    back->setTag(2);
    restart->setTag(3);
    pausemenu = Menu::create(con, back, restart, NULL);
    pausemenu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausemenu, 11);
    pausemenu->alignItemsVertically();
}

void HitBrick2::pausemenuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();

    if (NULL != node)
    {
        auto Gamemenuscene = Gamemenu2::createScene();
        auto Gamescene = HitBrick2::createScene();
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

void HitBrick2::addscore()
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
void HitBrick2::addhscore()
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
    default:
        break;
    }
    auto hscorelabel = Label::createWithTTF(hstrscore, "fonts/Marker Felt.ttf", 24);
    hscorelabel->setPosition(Vec2(250, visibleSize.height - 20));
    this->addChild(hscorelabel);

}
//ש����ײ�����
void HitBrick2::addcontactListener()
{
    EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HitBrick2::onConcactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    scheduleUpdate();
    return;
}

void HitBrick2::update(float delta) {
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

    if (offsetx == 0 && offsety == 0 && !keyMap[EventKeyboard::KeyCode::KEY_SPACE])
        return;
    auto movebyboard = MoveBy::create(0.2f, Vec2(offsetx, offsety));
    board->runAction(movebyboard);

    //SPACE  ball //ȡ������
    if (ifstart == 1 && startF <=30 ) {
        startF=50;
    }
   

    return;
}

void HitBrick2::addBricks()
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

void HitBrick2::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    //���̰���
    keyMap[keycode] = true;
    //��һ�ΰ��ո�ʼ����
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode && ifstart == 0) {
        ifstart = 1;    //��update����������
    }
}

void HitBrick2::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //�����ͷ�
    keyMap[keycode] = false;

    //�ո�release ֹͣ�����ͷ�С��
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode) {
        if (ifstart == 1) {
            ifstart = -1;          //С��������
            HitBrick_world->removeJoint(joint);
            ball->getPhysicsBody()->setVelocity(Vec2(0, startF * 4));
        }
    }
    return;
}

void HitBrick2::setPhysicsWorld(PhysicsWorld* world)
{
    HitBrick_world = world;
    return;
}


bool HitBrick2::onConcactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    Node* ball = getChildByTag(tagball);
    //�˶�����
    float angle = ball->getPhysicsBody()->getRotation() * 2 / 360 * 3.1415f;
    Vec2 dir = dir = Vec2(sin(angle), cos(angle));
    if (nodeA->getTag() == tagbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score++;
       
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //����

        //����score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score++;
       
        ball->getPhysicsBody()->applyImpulse(ballspeedup*dir);   //����
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

void HitBrick2::setJoint()
{
    joint = PhysicsJointFixed::construct(ball->getPhysicsBody(), board->getPhysicsBody(), board->getAnchorPoint());
    HitBrick_world->addJoint(joint);
}

void HitBrick2::Gameover()
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
        
        switch (Gamechoice) {
        case 1:
            if (score > UserDefault::getInstance()->getIntegerForKey("s1hscore"))
                UserDefault::getInstance()->setIntegerForKey("s1hscore", score);
            if (Gametime > UserDefault::getInstance()->getIntegerForKey("s1time"))
                UserDefault::getInstance()->setIntegerForKey("s1time", Gametime);
            break;
        case 2:
            if (score > UserDefault::getInstance()->getIntegerForKey("s2hscore"))
                UserDefault::getInstance()->setIntegerForKey("s2hscore", score);
            if (Gametime > UserDefault::getInstance()->getIntegerForKey("s2time"))
                UserDefault::getInstance()->setIntegerForKey("s2time", Gametime);
            break;
        case 3:
            if (score > UserDefault::getInstance()->getIntegerForKey("s3hscore"))
                UserDefault::getInstance()->setIntegerForKey("s3hscore", score);
            if (Gametime > UserDefault::getInstance()->getIntegerForKey("s2time"))
                UserDefault::getInstance()->setIntegerForKey("s2time", Gametime);
            break;
        default:
            break;
        }
        UserDefault::getInstance()->flush();

        //��ʱ���뷵��Gamemenu
        schedule(SEL_SCHEDULE(&HitBrick2::backGamemenu), 2.0f);
    }
    else {
        auto lab = Label::createWithTTF("DEFEAT!", "fonts/Marker Felt.ttf", 60);
        lab->setColor(Color3B(0, 0, 0));
        lab->setPosition(visibleSize.width / 2 + origin.x + 28, visibleSize.height / 2);
        this->addChild(lab, 1);

        switch (Gamechoice) {
        case 1:
            UserDefault::getInstance()->setIntegerForKey("s1hscore", score);
            break;
        case 2:
            UserDefault::getInstance()->setIntegerForKey("s2hscore", score);
            break;
        case 3:
            UserDefault::getInstance()->setIntegerForKey("s3hscore", score);
            break;
        default:
            break;
        }
        UserDefault::getInstance()->flush();

        //��ʱ���뷵��Gamemenu
        schedule(SEL_SCHEDULE(&HitBrick2::backGamemenu), 2.0f);
    }
}

void HitBrick2::backGamemenu(float dt) {
    auto Gamescene = Gamemenu2::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}

void HitBrick2::timer(float dt) {
    Gametime += 1;
    strtime = StringUtils::format("%d s", Gametime);
    timelabel->setString(strtime);
}