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

    //添加背景
    //addBackGround();
    label = Label::create();
    label->setString("awd");
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
       300));
    this->addChild(label,5);
    
    ifstart = 0;   //未发球
    startF = 0;    //
    

    //添加板
    addBoard();
    //添加球
    addball();
    //添加键盘监听器
    addKeyListener();

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
    board->setAnchorPoint(Vec2(0.5f, 0.5f));
    board->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y + 50));

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
    ball->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y + 50 + board->getContentSize().height));

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

//键盘监视器
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

void HitBrick::update(float delta) {
    Node* sprite = this->getChildByTag(0);
    auto w = EventKeyboard::KeyCode::KEY_W;
    auto s = EventKeyboard::KeyCode::KEY_S;
    auto a = EventKeyboard::KeyCode::KEY_A;
    auto d = EventKeyboard::KeyCode::KEY_D;
    int offsetx = 0;
    int offsety = 0;

    //限定board移动范围
    bool ifw = 1;            //能否wasd移动
    bool ifs = 1;
    bool ifa = 1;
    bool ifd = 1;

    int x = 0;
    int y = 0;

    x = sprite->getPositionX();
    y = sprite->getPositionY();
    
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
    auto moveto = MoveTo::create(0.2f, Vec2(sprite->getPositionX() + offsetx, sprite->getPositionY() + offsety));
    if(ifstart!=0)        //初始board不能移动
        sprite->runAction(moveto);

    //SPACE  ball蓄力
    if (ifstart==1 && startF<=200) {
        startF++;
        std::string str = StringUtils::format("%d", startF);
        label->setString(str);
    }

    return;
}


void HitBrick::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)                 
{
    //键盘按下
    keyMap[keycode] = true;
    //第一次按空格开始蓄力
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode && ifstart==0) {                      
        ifstart = 1;    //在update函数中蓄力
    }
}

void HitBrick::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //键盘释放
    keyMap[keycode] = false;   

    //空格release 停止蓄力释放小球
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode) {
        if (ifstart == 1) {
            ifstart = -1;          //小球不再蓄力
            ball->getPhysicsBody()->setVelocity(Vec2(0, startF*2));
        }
    }
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