#include "cocos2d.h"
#include "demo.h"
#include "store.h"
#include "Gamemenu.h"
//#include "Gamemenu.cpp"

USING_NS_CC;

Scene* HitBrick::createScene() {

    srand((unsigned)time(NULL));
    
    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setAutoStep(true);


    scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
    auto layer = HitBrick::create();
    
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    layer->setJoint();
    scene->addChild(layer);
    return scene;
}


bool HitBrick::init()
{
    if (!Layer::init())
    {
        return false;
    }
    UserDefault::getInstance()->flush();
    visibleSize = Director::getInstance()->getVisibleSize();                              //��ȡ��Ļ����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    /*
    std::string str = StringUtils::format("%ld",visibleSize.width / 2);
    auto label = Label::create();
    label->setString(str);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    */

    //��ӱ���
    //addBackGround();
    label = Label::create();
    label->setString(" ");
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
       300));
    this->addChild(label,5);
    
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
    //�����ͣ��ť
    addbutton();

    //��Ӽ��̼�����
    addKeyListener();
    //�����ײ������
    addcontactListener();

    scheduleUpdate();

    return true;
}

void HitBrick::addBackGround()
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

void HitBrick::addBoard()
{
    board = Sprite::create("board/board.png");
    board->setAnchorPoint(Vec2(0.5f, 0.5f));
    board->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y + 50));

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

void HitBrick::addball()
{
    ball = Sprite::create("ball/ball.png");
    ball->setScale(2.0f,2.0f);
    ball->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y + 50 + board->getContentSize().height));

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

void HitBrick::addmap()
{
    //ѡ���ͼ  
    UserDefault::getInstance()->flush();
    Gamechoice = UserDefault::getInstance()->getIntegerForKey("Gamechoice",0);
    switch (Gamechoice) {
    case 1:
        map = cocos2d::TMXTiledMap::create("map.tmx");
        break;
    case 2:
        map = cocos2d::TMXTiledMap::create("map2.tmx");
        break;
    default:
        map = cocos2d::TMXTiledMap::create("map3.tmx");
        break;
    }
    map->setAnchorPoint(Vec2(0.5f, 0));
    map->setPosition((Vec2(visibleSize.width / 2 + origin.x + 28, 0)));
    this->addChild(map, -1);

}

void HitBrick::addWall()
{
    //�߽�
    int bottom = 0;
    int over = 282;
    int right = 453;
    int left = 95;
    //�߽�Size
    Size WallSize = Size(right-left,over-bottom);   
    
    auto WallBody = PhysicsBody::createEdgeBox(WallSize, PhysicsMaterial(0.0f, 1.0f, 0.0f),3);  //Wall�ĸ�������  ���ܶ� ��ȫ������ײ ��Ħ����  �����������Ǳ߿�Ŀ��

    Wall = Sprite::create();
    Wall->setAnchorPoint(Vec2(0.5,0));
    Wall->setPosition(visibleSize.width / 2 + origin.x+28,145);
    Wall->setPhysicsBody(WallBody);
    addChild(Wall);

}
//���̼�����
void HitBrick::addKeyListener()  
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(HitBrick::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(HitBrick::onKeyReleased, this);
     _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdate();
    /*
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* event) {
        CCLOG("pressed");
    };
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event* event) {
        CCLOG("released");
    };
    */
    return;
}

void HitBrick::addbutton() {
    auto pause = MenuItemImage::create("progress/pause.png","progress/pause.png",CC_CALLBACK_1(HitBrick::pauseClickCallBack, this));
    auto menu = Menu::create(pause, NULL);
    menu->setPosition(Vec2(visibleSize.width-30, visibleSize.height-20));
    addChild(menu);
}
void HitBrick::pauseClickCallBack(Ref* pSender)
{

}
//ש����ײ�����
void HitBrick::addcontactListener()
{
    EventListenerPhysicsContact* contactListener= EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HitBrick::onConcactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    scheduleUpdate();
    return;
}


void HitBrick::update(float delta) {
    Node* board = this->getChildByTag(tagboard);
    //Node* ball  = this->getChildByTag(tagball);                    ???Ϊʲôball �� board ͬʱ���ᱨ������
    auto w = EventKeyboard::KeyCode::KEY_W;
    auto s = EventKeyboard::KeyCode::KEY_S;
    auto a = EventKeyboard::KeyCode::KEY_A;
    auto d = EventKeyboard::KeyCode::KEY_D;
    int offsetx = 0;
    int offsety = 0;

    //�޶�board�ƶ���Χ
    bool ifw = 1;            //�ܷ�wasd�ƶ�
    bool ifs = 1;
    bool ifa = 1;
    bool ifd = 1;

    int x = 0;
    int y = 0;

    x =board->getPositionX();
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

    if (offsetx == 0 && offsety == 0 && !keyMap[EventKeyboard::KeyCode::KEY_SPACE])
       return;
    auto movebyboard = MoveBy::create(0.2f, Vec2(offsetx,offsety));                  
    board->runAction(movebyboard);

    //SPACE  ball����
    if (ifstart==1 && startF<=100) {
        startF++;
        std::string str = StringUtils::format("%d %d %d", startF,x,y);
        label->setString(str);
    }

    return;
}

void HitBrick::addBricks()
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
        it->setAnchorPoint(Vec2(0,0));                       
        it->setPosition(60+x,y);


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

void HitBrick::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)                 
{
    //���̰���
    keyMap[keycode] = true;
    //��һ�ΰ��ո�ʼ����
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode && ifstart==0) {                      
        ifstart = 1;    //��update����������
    }
}

void HitBrick::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //�����ͷ�
    keyMap[keycode] = false;   

    //�ո�release ֹͣ�����ͷ�С��
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode) {
        if (ifstart == 1) {
            ifstart = -1;          //С��������
            HitBrick_world->removeJoint(joint);
            ball->getPhysicsBody()->setVelocity(Vec2(0, startF*4));
        }
    }
    return;
}

void HitBrick::setPhysicsWorld(PhysicsWorld* world)
{
    HitBrick_world = world; 
    return;
}


bool HitBrick::onConcactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    if (nodeA->getTag() == tagbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score++;
    }
    else if (nodeB->getTag() == tagbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score++;
    }

    if (bricksnum == 0)
    {
        Gameover();
    }
   
    return true;
}

/*
bool HitBrick::TouchBegan(cocos2d::Touch* pTouch, Event* pEvent)
{
    point = this->tilePosFromLocation(pTouch->getLocation(), (TMXTiledMap*)this->getChildByTag(1));
    return true;
}

Point HitBrick::tilePosFromLocation(Point location, TMXTiledMap* tileMap)
{
    //pos�ǵ�ͼ�ϵ����꣬��ǰ��Ļ������+��ͼ��ƫ����
    Point pos =Vec2(location, tileMap->getPosition());
    pos.x = (int)(pos.x / tileMap->getTileSize().width);
    //����ͼ�ܿ���*ÿ������� - ���ڵ�y���꣩/ ÿ�������
    //getMapSize().height�ǵ�ͼ�߶ȵĴ�ש��
    pos.y = (int)(((tileMap->getMapSize().height * tileMap->getTileSize().height - pos.y)) / tileMap->getTileSize().height);
    return pos;
}
*/
void HitBrick::setJoint() 
{
    joint = PhysicsJointFixed::construct(ball->getPhysicsBody(), board->getPhysicsBody(),board->getAnchorPoint());
    HitBrick_world->addJoint(joint);
}

void HitBrick::Gameover() 
{
    //ͣ�����м����
    _eventDispatcher->removeAllEventListeners();
    ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
    board->getPhysicsBody()->setVelocity(Vec2(0, 0));
    if (bricksnum==0) {
        //���success
        auto label1 = Label::createWithTTF("Success!~", "fonts/Marker Felt.ttf", 60);
        label1->setColor(Color3B(0, 0, 0));
        label1->setPosition(visibleSize.width / 2+ origin.x+28, visibleSize.height / 2);
        this->addChild(label1,1);

        //��ʱ���뷵��Gamemenu
        schedule(SEL_SCHEDULE(&HitBrick::backGamemenu), 2.0f);
    }
}

void HitBrick::backGamemenu(float dt) {
    auto Gamescene = Gamemenu::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}

// ���������水ť��Ӧ����
void HitBrick::replayCallback(Ref* pSender) {
    Director::getInstance()->replaceScene(HitBrick::createScene());
}

// �˳�
void HitBrick::exitCallback(Ref* pSender) {
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
