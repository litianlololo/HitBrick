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

    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //添加地图
    map->setAnchorPoint(Vec2(0.5f, 0.5f));                                                
    map->setPosition((Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)));
    this->addChild(map, 2);
    /*
    std::string str = StringUtils::format("%ld",visibleSize.width / 2);
    auto label = Label::create();
    label->setString(str);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    */

    //添加背景
    //addBackGround();
    //添加板
    addBoard();
    //添加球
    addball();
    //添加键盘监听器
    addListener();

    scheduleUpdate();

    return true;
}

void HitBrick::addBackGround()
{
    //实现背景图片铺满整个窗口
    auto bg = Sprite::create("background/background.jpg");                                                 
    bg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    Size mywinsize = Director::getInstance()->getWinSize();
    float winw = mywinsize.width; //获取屏幕宽度
    float winh = mywinsize.height;//获取屏幕高度
    float spx = bg->getTextureRect().getMaxX();
    float spy = bg->getTextureRect().getMaxY();
    bg->setScaleX(winw / spx); //设置精灵宽度缩放比例
    bg->setScaleY(winh / spy);
    this->addChild(bg);

    return;
}

void HitBrick::addBoard()
{
    board = Sprite::create("board/board.png");
    board->setPosition(Vec2(visibleSize.width + origin.x, origin.y + 50));

    board->setTag(0);           //供键盘监听器定位

    
    //添加板的刚体属性
    auto BoardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));
  
    BoardBody->setCategoryBitmask(0xFFFFFFFF);           //类别掩码
    BoardBody->setCollisionBitmask(0xFFFFFFFF);           //允许撞我
    BoardBody->setContactTestBitmask(0xFFFFFFFF);        //可接到通知
    BoardBody->setGravityEnable(false);                  //不受重力影响
    BoardBody->setTag(tagboard);
    //只有己方的CategoryBitmask，与对方的CollisionBitmask与运算不为0
    //并且对方的CategoryBitmask，与己方的CollisionBitmask与运算不为0，才会发生碰撞
    board->setPhysicsBody(BoardBody);
    
    board->getPhysicsBody()->setRotationEnable(false);   //设定球不旋转


    this->addChild(board, tagboard);


    return;
}

void HitBrick::addball()
{
    ball = Sprite::create("ball/ball.png");
    ball->setScale(2.0f,2.0f);
    ball->setPosition(Vec2(visibleSize.width + origin.x, origin.y + 50 + board->getContentSize().height));

    // 设置球的刚体属性
    auto ballBody = PhysicsBody::createCircle(ball->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

    ballBody->setCategoryBitmask(0xFFFFFFFF);             //类别掩码
    ballBody->setCollisionBitmask(0xFFFFFFFF);            //允许撞我
    ballBody->setContactTestBitmask(0xFFFFFFFF);         //可接到通知
    ballBody->setGravityEnable(false);                    //不受重力影响
    
    ballBody->setTag(tagball);
    
    ballBody->setRotationEnable(false);                  //设定球不旋转

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
    int speed = 2;
    if (keyMap[a])
    {
        offsetx = -1 * speed;
    }
    if (keyMap[d])
    {
        offsetx = speed;
    }
    if (keyMap[w])
    {
        offsety = speed;
    }
    if (keyMap[s])
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