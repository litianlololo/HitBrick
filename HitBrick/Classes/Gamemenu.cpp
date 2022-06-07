#include "cocos2d.h"
#include "Gamemenu.h"
#include "Gamemodel.h"
#include "Mainmenu.h"
#include "demo.h"
#include "store.h"

USING_NS_CC;
Gamemenu::Gamemenu() :Gamechoice(0),visibleSize(Size(0,0)),origin(Vec2(0,0))
{

}
Scene* Gamemenu::createScene()
{
    auto scene = Scene::create();
    auto layer = Gamemenu::create();
    scene->addChild(layer);
    return scene;
}

bool Gamemenu::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /*
    auto computer = Sprite::create("mainmenu/computer.png");
    computer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2));
    addChild(computer,4);
    */
    MenuItemFont::setFontSize(20);
    auto ur1 = FileUtils::getInstance()->getStringFromFile("Gamemenu/Gamemenu1.txt");
    auto game1 = MenuItemFont::create(ur1, CC_CALLBACK_1(Gamemenu::menuClickCallBack, this));
    auto ur2 = FileUtils::getInstance()->getStringFromFile("Gamemenu/Gamemenu2.txt");
    auto game2 = MenuItemFont::create(ur2, CC_CALLBACK_1(Gamemenu::menuClickCallBack, this));
    auto ur3 = FileUtils::getInstance()->getStringFromFile("Gamemenu/Gamemenu3.txt");
    auto game3 = MenuItemFont::create(ur3, CC_CALLBACK_1(Gamemenu::menuClickCallBack, this));
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(Gamemenu::menuClickCallBack, this));
    //MainmenuTag
    game1->setTag(1);
    game2->setTag(2);
    game3->setTag(3);
    back->setTag(0);
    auto menu = Menu::create(game1, game2,game3, back,NULL);
    menu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    menu->setColor(Color3B::BLACK);
    addChild(menu);
    menu->alignItemsVertically();

    addBackGround();

    scheduleUpdate();
    return true;
}

void Gamemenu::addBackGround()
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
void Gamemenu::menuClickCallBack(Ref* sender)
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
        default:
            break;
        }
    }
}

//监测Gamechoice是否改变
void Gamemenu::update(float delta) {
    if (Gamechoice != 0) {
        UserDefault::getInstance()->setIntegerForKey("Gamechoice", Gamechoice);
        UserDefault::getInstance()->flush();
        Game();
    }
}

//改变场景至具体游戏
void Gamemenu::Game()
{
    auto Gamescene = HitBrick::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}