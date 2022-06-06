#include "cocos2d.h"
#include "SpeedGame.h"
#include "store.h"
#include "Gamemenu2.h"
#include "AudioEngine.h"
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
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
    //添加2x砖块
    add2xbricks();
    //添加slow砖块
    addslowbricks();
    //添加crazy砖块
    addcrazybricks();
    //添加固定砖块
    addstaticbricks();
    //添加暂停按钮
    addbutton();
    //添加分数
    addscore();
    //添加最高分
    addhscore();
    //添加时间
    addtime();
    //添加键盘监听器
    addKeyListener();
    //添加碰撞监听器
    addcontactListener();

    scheduleUpdate();
    
    schedule(SEL_SCHEDULE(&HitBrick2::timer), 1.0f);
    return true;
}

void HitBrick2::addBackGround()
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

void HitBrick2::addBoard()
{
    board = Sprite::create("board/board.png");
    board->setAnchorPoint(Vec2(0.5f, 0.5f));
    board->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + 50));

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

void HitBrick2::addball()
{
    ball = Sprite::create("ball/ball.png");
    ball->setScale(2.0f, 2.0f);
    ball->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + 48 + board->getContentSize().height));

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

void HitBrick2::addmap()
{
    //选择地图  
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

void HitBrick2::addtime()
{
    strtime = StringUtils::format("%d s", Gametime);
    timelabel = Label::createWithTTF(strtime, "fonts/Marker Felt.ttf", 24);
    timelabel->setPosition(Vec2(350, visibleSize.height - 20));
    this->addChild(timelabel);
}

void HitBrick2::addWall()
{
    //边界
    float bottom = -50;
    float over = 282;
    float right = 453;
    float left = 95;
    //边界Size
    Size WallSize = Size(right - left, over - bottom);

    auto WallBody = PhysicsBody::createEdgeBox(WallSize, PhysicsMaterial(0.0f, 1.0f, 0.0f), 0.1f);  //Wall的刚体属性  无密度 完全弹性碰撞 无摩擦力  第三个参数是边框的宽度
    
    Wall = Sprite::create();
    Wall->setAnchorPoint(Vec2(0.5, 0));
    Wall->setPosition(visibleSize.width / 2 + origin.x + 28, 115);
    Wall->setPhysicsBody(WallBody);
    addChild(Wall);

}
//键盘监视器
void HitBrick2::addKeyListener()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(HitBrick2::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(HitBrick2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdate();

    return;
}
//暂停按钮
void HitBrick2::addbutton() {
    auto pause = MenuItemImage::create("progress/pause.png", "progress/pause.png", CC_CALLBACK_1(HitBrick2::pauseClickCallBack, this));
    auto menu = Menu::create(pause, NULL);
    menu->setPosition(Vec2(visibleSize.width - 30, visibleSize.height - 20));
    addChild(menu);
}
//暂停目录
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
//添加最高分
void HitBrick2::addhscore()
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
//砖块碰撞监测器
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
    auto p = EventKeyboard::KeyCode::KEY_P;
    float offsetx = 0;
    float offsety = 0;

    //限定board移动范围
    bool ifw = 1;            //能否wasd移动
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

    //SPACE  ball //取消蓄力
    if (ifstart == 1 && startF <=30 ) {
        //不同关卡小球初始速度不同
        switch (Gamechoice) {
        case 1:
            startF=50;
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

void HitBrick2::addBricks()
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
        it->setAnchorPoint(Vec2(0, 0));
        it->setPosition(60 + x, y);


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

void HitBrick2::add2xbricks() {
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

void HitBrick2::addslowbricks() {
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

void HitBrick2::addcrazybricks() {
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

void HitBrick2::addstaticbricks() {
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

void HitBrick2::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    //键盘按下
    keyMap[keycode] = true;
    //第一次按空格开始蓄力
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode && ifstart == 0) {
        ifstart = 1;    //在update函数中蓄力
    }
}

void HitBrick2::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //键盘释放
    keyMap[keycode] = false;

    //空格release 停止蓄力释放小球
    if (EventKeyboard::KeyCode::KEY_SPACE == keycode) {
        if (ifstart == 1) {
            ifstart = -1;          //小球不再蓄力
            HitBrick_world->removeJoint(joint);
            ball->getPhysicsBody()->setVelocity(Vec2(0, startF * 4));
            AudioEngine::play2d("biu.mp3", false, 1.0);
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
    //运动方向
    float angle = ball->getPhysicsBody()->getRotation() * 2 / 360 * 3.1415f;
    Vec2 dir = dir = Vec2(sin(angle), cos(angle));
    if (nodeA->getTag() == tagbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        score+=perscore;
       
        ball->getPhysicsBody()->applyImpulse(ballspeedup * dir);   //加速

        //更新score
        strscore = StringUtils::format("%d", score);
        scorelabel->setString(strscore);
    }
    else if (nodeB->getTag() == tagbrick && nodeA->getTag() == tagball) {
        removeChild(nodeB);
        bricksnum--;
        score+=perscore;
       
        ball->getPhysicsBody()->applyImpulse(ballspeedup*dir);   //加速
        //更新score
        strscore = StringUtils::format("%d", score);

        scorelabel->setString(strscore);
    }
    //2xbrick
    else if (nodeA->getTag() == tag2xbrick && nodeB->getTag() == tagball) {
        removeChild(nodeA);
        bricksnum--;
        perscore = 2;
        score+=perscore;

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

        ball->getPhysicsBody()->setVelocity(120*dir);   //减速
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

void HitBrick2::setJoint()
{
    joint = PhysicsJointFixed::construct(ball->getPhysicsBody(), board->getPhysicsBody(), board->getAnchorPoint());
    HitBrick_world->addJoint(joint);
}

void HitBrick2::Gameover()
{
    //停用所有监控器
    _eventDispatcher->removeAllEventListeners();
    ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
    board->getPhysicsBody()->setVelocity(Vec2(0, 0));
    if (bricksnum == 0) {
        //输出success
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

    //延时两秒返回Gamemenu
    schedule(SEL_SCHEDULE(&HitBrick2::backGamemenu), 5.0f);
}
//排行榜结算
void HitBrick2::settlement()
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

void HitBrick2::backGamemenu(float dt) {
    AudioEngine::stopAll();
    AudioEngine::play2d("Gamemusic.mp3", true, 1.0);
    auto Gamescene = Gamemenu2::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}

void HitBrick2::timer(float dt) {
    Gametime += 1;
    strtime = StringUtils::format("%d s", Gametime);
    timelabel->setString(strtime);
}