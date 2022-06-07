#include "cocos2d.h"
#include "setting.h"
#include "Mainmenu.h"


USING_NS_CC;


Scene* setting::createScene()
{
    auto scene = Scene::create();
    auto layer = setting::create();
    scene->addChild(layer);
    return scene;
}

bool setting::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    ball = Sprite::create("ball/ball.png");
    ball->setScale(4.0f, 4.0f);
    ball->setPosition(Vec2(visibleSize.width / 6 + origin.x, visibleSize.height / 2+origin.y ));
    addChild(ball);
    //ballcolor
    MenuItemFont::setFontSize(20);
    auto ballred = MenuItemFont::create("red", CC_CALLBACK_1(setting::menuClickCallBack, this));
    ballred->setColor(Color3B::RED);
    auto ballblue = MenuItemFont::create("blue", CC_CALLBACK_1(setting::menuClickCallBack, this));
    ballblue->setColor(Color3B::BLUE);
    auto ballgreen = MenuItemFont::create("green", CC_CALLBACK_1(setting::menuClickCallBack, this));
    ballgreen->setColor(Color3B::GREEN);
    auto ballwhite = MenuItemFont::create("white", CC_CALLBACK_1(setting::menuClickCallBack, this));
    ballwhite->setColor(Color3B::WHITE);
    //MainmenuTag
    ballred->setTag(1);
    ballblue->setTag(2);
    ballgreen->setTag(3);
    ballwhite->setTag(4);
    auto menu = Menu::create(ballred, ballblue, ballgreen,ballwhite, NULL);
    menu->setPosition(visibleSize.width / 3 + origin.x, visibleSize.height / 2);

    addChild(menu);
    menu->alignItemsVertically();

    //board
    board = Sprite::create("board/board.png");
    board->setScale(1.0f, 1.0f);
    board->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(board);
    MenuItemFont::setFontSize(20);
    auto boardred = MenuItemFont::create("red", CC_CALLBACK_1(setting::menuClickCallBack2, this));
    boardred->setColor(Color3B::RED);
    auto boardblue = MenuItemFont::create("blue", CC_CALLBACK_1(setting::menuClickCallBack2, this));
    boardblue->setColor(Color3B::BLUE);
    auto boardgreen = MenuItemFont::create("green", CC_CALLBACK_1(setting::menuClickCallBack2, this));
    boardgreen->setColor(Color3B::GREEN);
    auto boardwhite = MenuItemFont::create("white", CC_CALLBACK_1(setting::menuClickCallBack2, this));
    boardwhite->setColor(Color3B::WHITE);
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(setting::menuClickCallBack2, this));
    //MainmenuTag
    boardred->setTag(5);
    boardblue->setTag(6);
    boardgreen->setTag(7);
    boardwhite->setTag(8);
    back->setTag(0);
 
    auto menu2 = Menu::create(boardred, boardblue, boardgreen, boardwhite, back, NULL);
    menu2->setPosition(visibleSize.width * 2 / 3 + origin.x, visibleSize.height / 2);

    addChild(menu2);
    menu2->alignItemsVertically();


    addBackGround();

    scheduleUpdate();
    return true;
}

void setting::addBackGround()
{
    //实现背景图片铺满整个窗口
    auto bg = Sprite::create("Mainback.png");
    bg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    Size mywinsize = Director::getInstance()->getWinSize();
    float winw = mywinsize.width; //获取屏幕宽度
    float winh = mywinsize.height;//获取屏幕高度
    float spx = bg->getTextureRect().getMaxX();
    float spy = bg->getTextureRect().getMaxY();
    bg->setScaleX(winw / spx); //设置精灵宽度缩放比例
    bg->setScaleY(winh / spy);
    this->addChild(bg, -3);

    return;
}
void setting::menuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    if (NULL != node)
    {
        auto Gamemodelscene = Mainmenu::createScene();
        switch (tag)
        {
        case 0:
            Director::getInstance()->replaceScene(Gamemodelscene);
            break;
        case 1:
            ball->setColor(Color3B::RED);
            UserDefault::getInstance()->setIntegerForKey("ballcolor", 1);
            break;
        case 2:
            ball->setColor(Color3B::BLUE);
            UserDefault::getInstance()->setIntegerForKey("ballcolor", 2);
            break;
        case 3:
            ball->setColor(Color3B::GREEN);
            UserDefault::getInstance()->setIntegerForKey("ballcolor", 3);
            break;
        case 4:
            ball->setColor(Color3B::WHITE);
            UserDefault::getInstance()->setIntegerForKey("ballcolor", 4);
            break;
        default:
            break;
        }
    }
}

void setting::menuClickCallBack2(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    if (NULL != node)
    {
        auto Gamemodelscene = Mainmenu::createScene();
        switch (tag)
        {
        case 0:
            Director::getInstance()->replaceScene(Gamemodelscene);
            break;
        case 5:
            board->setColor(Color3B::RED);
            UserDefault::getInstance()->setIntegerForKey("boardcolor", 1);
            break;
        case 6:
            board->setColor(Color3B::BLUE);
            UserDefault::getInstance()->setIntegerForKey("boardcolor", 2);
            break;
        case 7:
            board->setColor(Color3B::GREEN);
            UserDefault::getInstance()->setIntegerForKey("boardcolor", 3);
            break;
        case 8:
            board->setColor(Color3B::WHITE);
            UserDefault::getInstance()->setIntegerForKey("boardcolor", 4);
            break;
        default:
            break;
        }
    }
}
//监测Gamechoice是否改变
void setting::update(float delta) {
    
}