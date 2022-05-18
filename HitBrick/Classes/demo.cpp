#include "cocos2d.h"
#include "demo.h"



USING_NS_CC;

Scene* HitBrick::createScene() {

    srand((unsigned)time(NULL));
    
    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setAutoStep(true);


    scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
    auto layer = HitBrick::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    //layer->setJoint();
    scene->addChild(layer);
    return scene;
}


bool HitBrick::init()
{
    if (!Layer::init())
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();                              //��ȡ��Ļ����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //��ӵ�ͼ
    map->setAnchorPoint(Vec2(0.5f, 0));                                                
    map->setPosition((Vec2(visibleSize.width / 2 + origin.x,0)));
    this->addChild(map, -1);
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
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
       300));
    this->addChild(label,5);
    
    
    //��Ӱ�


    addBoard();
    //�����
    addball();
    //��Ӽ��̼�����
    addListener();

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

    board->setTag(0);           //�����̼�������λ

    
    //��Ӱ�ĸ�������
    auto BoardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));
  
    BoardBody->setCategoryBitmask(0xFFFFFFFF);           //�������
    BoardBody->setCollisionBitmask(0xFFFFFFFF);           //����ײ��
    BoardBody->setContactTestBitmask(0xFFFFFFFF);        //�ɽӵ�֪ͨ
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
    
    ballBody->setTag(tagball);
    
    ballBody->setRotationEnable(false);                  //�趨����ת

    ball->setPhysicsBody(ballBody);                

    addChild(ball, tagball);
}

void HitBrick::addListener()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* event) {
        CCLOG("pressed");
        keyMap[keycode] = true;
    };
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event* event) {
        CCLOG("released");
        keyMap[keycode] = false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdate();

    return;
}

void HitBrick::update(float delta) {
    Node* sprite = this->getChildByTag(0);
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

    x = sprite->getPositionX();
    y = sprite->getPositionY();
    std::string str = StringUtils::format("%d  %d", x, y);
    label->setString(str);

    if (y <= 10)
        ifs = 0;
    if (y >= 100)
        ifw = 0;
    if (x <= 100)
        ifa = 0;
    if (x >= 380)
        ifd = 0;

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

    if (offsetx == 0 && offsety == 0)
        return;
    auto moveto = MoveTo::create(0.2f, Vec2(sprite->getPositionX() + offsetx, sprite->getPositionY() + offsety));
    sprite->runAction(moveto);

    return;
}

void HitBrick::setPhysicsWorld(PhysicsWorld* world)
{
    HitBrick_world = world; 
    return;
}

/*
void HitBrick::setJoint() {
    Vec2 fixPoint = Vec2(ball->getAnchorPoint().x, ball->getAnchorPoint().y - 30);
    joint1 = PhysicsJointPin::construct(ball->getPhysicsBody(), player->getPhysicsBody(), fixPoint, player->getAnchorPoint());
    m_world->addJoint(joint1);
}
*/