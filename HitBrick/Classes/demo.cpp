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
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /*
    //添加背景
    //addBackGround();
    label = Label::create();
    label->setString(" ");
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
       300));
    this->addChild(label,5);
    */

    ifstart = 0;   //未发球
    startF = 0;    //

    //添加地图
    addmap();
    //添加板
    addBoard();
    //添加球
    addball();
    //添加Wall
    addWall();
    //添加砖块
    addBricks();
    add2xbricks(); //添加2倍砖块
    addslowbricks(); //添加减速砖块
    addcrazybricks(); //添加疯狂砖块
    addstaticbricks(); //添加固定砖块
    //添加暂停按钮
    addbutton();
    //添加分数
    addscore();
    //添加最高分
    addhscore();
    //添加键盘监听器
    addKeyListener();
    //添加碰撞监听器
    addcontactListener();

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

    board->setTag(tagboard);           //供键盘监听器定位

    
    //添加板的刚体属性
    auto BoardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));
  
    BoardBody->setCategoryBitmask(0xFFFFFFFF);           //类别掩码
    BoardBody->setCollisionBitmask(0xFFFFFFFF);           //允许撞我  碰撞掩码
    BoardBody->setContactTestBitmask(0xFFFFFFFF);        //可接到通知  接触掩码
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
    
    //ballBody->setTag(tagball);
    
    ballBody->setRotationEnable(false);                  //设定球不旋转

    ball->setPhysicsBody(ballBody);                
    ball->setTag(tagball);
    addChild(ball);
}

void HitBrick::addmap()
{
    //选择地图  
    UserDefault::getInstance()->flush();
    Gamechoice = UserDefault::getInstance()->getIntegerForKey("Gamechoice",0);
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

void HitBrick::addWall()
{
    //边界
    int bottom = 0;
    int over = 282;
    int right = 453;
    int left = 95;
    //边界Size
    Size WallSize = Size(right-left,over-bottom);   
    
    auto WallBody = PhysicsBody::createEdgeBox(WallSize, PhysicsMaterial(0.0f, 1.0f, 0.0f),3);  //Wall的刚体属性  无密度 完全弹性碰撞 无摩擦力  第三个参数是边框的宽度

    Wall = Sprite::create();
    Wall->setAnchorPoint(Vec2(0.5,0));
    Wall->setPosition(visibleSize.width / 2 + origin.x+28,145);
    Wall->setPhysicsBody(WallBody);
    addChild(Wall);

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
//暂停按钮
void HitBrick::addbutton() {
    auto pause = MenuItemImage::create("progress/pause.png","progress/pause.png",CC_CALLBACK_1(HitBrick::pauseClickCallBack, this));
    auto menu = Menu::create(pause, NULL);
    menu->setPosition(Vec2(visibleSize.width-30, visibleSize.height-20));
    addChild(menu);
}
//暂停目录
void HitBrick::pauseClickCallBack(Ref* pSender)               
{
    Director::getInstance()->stopAnimation();

    pausebg = Sprite::create("progress/bg.png");
    pausebg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausebg,10);                                                                                 

    MenuItemFont::setFontSize(20);

    auto ur1 = FileUtils::getInstance()->getStringFromFile("progress/continue.txt");
    auto con = MenuItemFont::create(ur1, CC_CALLBACK_1(HitBrick::pausemenuClickCallBack, this));

    auto ur2 = FileUtils::getInstance()->getStringFromFile("progress/back.txt");
    auto back = MenuItemFont::create(ur2, CC_CALLBACK_1(HitBrick::pausemenuClickCallBack, this));

    auto ur3 = FileUtils::getInstance()->getStringFromFile("progress/restart.txt");
    auto restart = MenuItemFont::create(ur3, CC_CALLBACK_1(HitBrick::pausemenuClickCallBack, this));

    //MainmenuTag
    con->setTag(1);
    back->setTag(2);
    restart->setTag(3);
    pausemenu = Menu::create(con, back, restart, NULL);
    pausemenu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(pausemenu,11);
    pausemenu->alignItemsVertically();
}

void HitBrick::pausemenuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();

    if (NULL != node)
    {
        auto Gamemenuscene = Gamemenu::createScene();
        auto Gamescene = HitBrick::createScene();
        switch (tag)
        {
        case 1:             
            //继续
            removeChild(pausebg);
            removeChild(pausemenu);
            Director::getInstance()->startAnimation();
            break;
        case 2:
            //返回
            Director::getInstance()->startAnimation();
            Director::getInstance()->replaceScene(Gamemenuscene);
            break;
        case 3:
            //重玩
            Director::getInstance()->startAnimation();
            Director::getInstance()->replaceScene(Gamescene);
            break;
        default:
            break;
        }
    }
}

void HitBrick::addscore()
{
    auto label = Label::createWithTTF("score", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(50, visibleSize.height - 20));
    this->addChild(label);

    strscore = StringUtils::format("%d",score);
    scorelabel = Label::createWithTTF(strscore, "fonts/Marker Felt.ttf", 24);
    scorelabel->setPosition(Vec2(150, visibleSize.height - 20));
    this->addChild(scorelabel);

}
//添加最高分
void HitBrick::addhscore()
{
    auto url = FileUtils::getInstance()->getStringFromFile("progress/hscore.txt");
    auto label = Label::create();
    label->setString(url);
    label->setPosition(Vec2(200, visibleSize.height - 20));
    this->addChild(label);

    //取得最高分
    std::string hstrscore;
    switch (Gamechoice) {
    case 1:
        hstrscore = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("y1hscore", 0));
        break;
    case 2:
        hstrscore = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("y2hscore", 0));
        break;
    case 3:
        hstrscore = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("y3hscore", 0));
        break;
    default:
        break;
    }
    auto hscorelabel = Label::createWithTTF(hstrscore, "fonts/Marker Felt.ttf", 24);
    hscorelabel->setPosition(Vec2(250, visibleSize.height - 20));
    this->addChild(hscorelabel);

}
//砖块碰撞监测器
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
    //Node* ball  = this->getChildByTag(tagball);                    ???为什么ball 和 board 同时动会报错？？？
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

    //SPACE  ball蓄力
    if (ifstart==1 && startF<=100) {
        startF++;
    }

    return;
}

void HitBrick::addBricks()
{
    TMXObjectGroup* objectGroup = map->getObjectGroup("bricks");
    ValueVector values = objectGroup->getObjects();
    
    for (Value value : values)//遍历所有对象
    {
        ValueMap valueMap = value.asValueMap();//获得属性值：Value转换成ValueMap
       float x = valueMap["x"].asFloat();//获取对象的属性:(as一类的方法 （转换类型）
        float y = valueMap["y"].asFloat();
         //name/type/width/height
        //int hp = valueMap["hp"].asInt();
        int type = valueMap["type"].asInt();
        //添加砖块精灵
        Sprite* it = Sprite::create("bluebrick.png"); //enemy = type == 4 ? Sprite::create("CloseNormal.png") : Sprite::create("CloseSelected.png");//根据位置属性创建精灵
        it->setAnchorPoint(Vec2(0,0));                       
        it->setPosition(60+x,y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //类别掩码
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //允许撞我
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //可接到通知
        BrickBody->setGravityEnable(false);                    //不受重力影响

        //BrickBody->setTag(tagbrick);

        BrickBody->setRotationEnable(false);                  //设定不旋转
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);

        this->addChild(it); 
        it->setTag(tagbrick);

        bricksnum++;
        Brickpath.push_back(Vec2(x, y));//将路径点保存到路径中
    }
}

void HitBrick::add2xbricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("2xbrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//遍历所有对象
    {
        ValueMap valueMap = value.asValueMap();//获得属性值：Value转换成ValueMap
        float x = valueMap["x"].asFloat();//获取对象的属性:(as一类的方法 （转换类型）
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //添加砖块精灵
        Sprite* it = Sprite::create("brick2x.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //类别掩码
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //允许撞我
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //可接到通知
        BrickBody->setGravityEnable(false);                    //不受重力影响


        BrickBody->setRotationEnable(false);                  //设定不旋转
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);


        addChild(it);
        it->setTag(tag2xbrick);

        bricksnum++;
        // Brickpath.push_back(Vec2(x, y));//将路径点保存到路径中
    }
}

void HitBrick::addslowbricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("slowbrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//遍历所有对象
    {
        ValueMap valueMap = value.asValueMap();//获得属性值：Value转换成ValueMap
        float x = valueMap["x"].asFloat();//获取对象的属性:(as一类的方法 （转换类型）
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //添加砖块精灵
        Sprite* it = Sprite::create("orangebrick.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //类别掩码
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //允许撞我
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //可接到通知
        BrickBody->setGravityEnable(false);                    //不受重力影响


        BrickBody->setRotationEnable(false);                  //设定不旋转
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);


        addChild(it);
        it->setTag(tagslowbrick);

        bricksnum++;
        // Brickpath.push_back(Vec2(x, y));//将路径点保存到路径中
    }
}

void HitBrick::addcrazybricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("crazybrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//遍历所有对象
    {
        ValueMap valueMap = value.asValueMap();//获得属性值：Value转换成ValueMap
        float x = valueMap["x"].asFloat();//获取对象的属性:(as一类的方法 （转换类型）
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //添加砖块精灵
        Sprite* it = Sprite::create("crazybrick.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //类别掩码
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //允许撞我
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //可接到通知
        BrickBody->setGravityEnable(false);                    //不受重力影响


        BrickBody->setRotationEnable(false);                  //设定不旋转
        //BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);

        addChild(it);
        it->setTag(tagcrazybrick);
    }
}

void HitBrick::addstaticbricks() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("staticbrick");
    ValueVector values = objectGroup->getObjects();

    for (Value value : values)//遍历所有对象
    {
        ValueMap valueMap = value.asValueMap();//获得属性值：Value转换成ValueMap
        float x = valueMap["x"].asFloat();//获取对象的属性:(as一类的方法 （转换类型）
        float y = valueMap["y"].asFloat();

        int type = valueMap["type"].asInt();
        //添加砖块精灵
        Sprite* it = Sprite::create("staticbrick.png");
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


        auto BrickBody = PhysicsBody::createCircle(it->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));

        BrickBody->setCategoryBitmask(0xFFFFFFFF);             //类别掩码
        BrickBody->setCollisionBitmask(0xFFFFFFFF);            //允许撞我
        BrickBody->setContactTestBitmask(0xFFFFFFFF);         //可接到通知
        BrickBody->setGravityEnable(false);                    //不受重力影响


        BrickBody->setRotationEnable(false);                  //设定不旋转
        BrickBody->setDynamic(false);
        it->setPhysicsBody(BrickBody);

        addChild(it);
        it->setTag(tagstaticbrick);
    }
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
    //运动方向
    float angle = ball->getPhysicsBody()->getRotation() * 2 / 360 * 3.1415f;
    Vec2 dir = dir = Vec2(sin(angle), cos(angle));
    if (nodeA->getTag() == tagbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速

        //更新score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速
        //更新score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    //2xbrick
    else if (nodeA->getTag() == tag2xbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        perscore = 2;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速

        //更新score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tag2xbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        perscore = 2;
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速
        //更新score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    //slowbrick
    else if (nodeA->getTag() == tagslowbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->setVelocity(120 * dir);   //减速

        //更新score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagslowbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;

        ball->getPhysicsBody()->setVelocity(120 * dir);   //减速
        //更新score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    //crazybrick
    else if (nodeA->getTag() == tagcrazybrick && nodeB->getTag() == tagball) {
        score += perscore;

        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速

        //更新score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagcrazybrick && nodeA->getTag() == tagball) {
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速
        //更新score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    else if (nodeA->getTag() == tagcrazybrick && nodeB->getTag() == tagbrick) {
        removeChild(nodeB);
        bricksnum--;
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速

        //更新score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagcrazybrick && nodeA->getTag() == tagbrick) {
        removeChild(nodeA);
        bricksnum--;
        score += perscore;
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速
        //更新score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
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
    //pos是地图上的坐标，当前屏幕的坐标+地图的偏移量
    Point pos =Vec2(location, tileMap->getPosition());
    pos.x = (int)(pos.x / tileMap->getTileSize().width);
    //（地图总块数*每块的像素 - 现在的y坐标）/ 每块的像素
    //getMapSize().height是地图高度的瓷砖数
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
    //停用所有监控器
    _eventDispatcher->removeAllEventListeners();
    ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
    board->getPhysicsBody()->setVelocity(Vec2(0, 0));
    if (bricksnum==0) {
        //输出success
        auto label1 = Label::createWithTTF("Success!~", "fonts/Marker Felt.ttf", 60);
        label1->setColor(Color3B(0, 0, 0));
        label1->setPosition(visibleSize.width / 2+ origin.x+28, visibleSize.height / 2);
        this->addChild(label1,1);
        
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

        //延时两秒返回Gamemenu
        schedule(SEL_SCHEDULE(&HitBrick::backGamemenu), 2.0f);
    }
}

void HitBrick::backGamemenu(float dt) {
    auto Gamescene = Gamemenu::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}

// 继续或重玩按钮响应函数
void HitBrick::replayCallback(Ref* pSender) {
    Director::getInstance()->replaceScene(HitBrick::createScene());
}

// 退出
void HitBrick::exitCallback(Ref* pSender) {
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
