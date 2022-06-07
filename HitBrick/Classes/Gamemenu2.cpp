#include "cocos2d.h"
#include "Gamemenu2.h"
#include "Gamemodel.h"
#include "Mainmenu.h"
#include "SpeedGame.h"
#include "store.h"]
#include "ranking.h"

USING_NS_CC;
//构造函数 初始化
Gamemenu2::Gamemenu2() :Gamechoice(0), visibleSize(Size(0, 0)), origin(Vec2(0, 0))
{

}

Scene* Gamemenu2::createScene()
{
    auto scene = Scene::create();
    auto layer = Gamemenu2::create();
    scene->addChild(layer);
    return scene;
}

bool Gamemenu2::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    MenuItemFont::setFontSize(20);
    auto ur1 = FileUtils::getInstance()->getStringFromFile("Gamemenu/Gamemenu1.txt");
    auto game1 = MenuItemFont::create(ur1, CC_CALLBACK_1(Gamemenu2::menuClickCallBack, this));
    auto ur2 = FileUtils::getInstance()->getStringFromFile("Gamemenu/Gamemenu2.txt");
    auto game2 = MenuItemFont::create(ur2, CC_CALLBACK_1(Gamemenu2::menuClickCallBack, this));
    auto ur3 = FileUtils::getInstance()->getStringFromFile("Gamemenu/Gamemenu3.txt");
    auto game3 = MenuItemFont::create(ur3, CC_CALLBACK_1(Gamemenu2::menuClickCallBack, this));
    auto ur4 = FileUtils::getInstance()->getStringFromFile("Gamemenu/Gamemenu4.txt");
    auto game4 = MenuItemFont::create(ur4, CC_CALLBACK_1(Gamemenu2::menuClickCallBack, this));
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(Gamemenu2::menuClickCallBack, this));

    //MainmenuTag
    game1->setTag(1);
    game2->setTag(2);
    game3->setTag(3);
    game4->setTag(4);
    back->setTag(0);
    auto menu = Menu::create(game1, game2, game3, game4, back, NULL);
    menu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    menu->setColor(Color3B::BLACK);
    addChild(menu);
    menu->alignItemsVertically();

    addBackGround();

    scheduleUpdate();
    return true;
}

void Gamemenu2::addBackGround()
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
void Gamemenu2::menuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    if (NULL != node)
    {
        auto Gamemodelscene = Gamemodel::createScene();
        switch (tag)
        {
        case 0:
            Director::getInstance()->replaceScene(Gamemodelscene);
            break;
        case 1:
            Gamechoice = 1;
            break;
        case 2:
            Gamechoice = 2;
            break;
        case 3:
            Gamechoice = 3;
            break;
        case 4:
            Gamechoice = 4;
            break;
        default:
            break;
        }
    }
}

//监测Gamechoice是否改变
void Gamemenu2::update(float delta) {
    if (Gamechoice != 0) {
        UserDefault::getInstance()->setIntegerForKey("Gamechoice", Gamechoice);
        UserDefault::getInstance()->flush();
        Game();
    }
}

//改变场景至具体游戏
void Gamemenu2::Game()
{
    auto Gamescene = ranking::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}