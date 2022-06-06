#include "cocos2d.h"
#include "Gamemodel.h"

USING_NS_CC;


Scene* Gamemodel::createScene()
{
    auto scene = Scene::create();
    auto layer = Gamemodel::create();
    scene->addChild(layer);
    return scene;
}

bool Gamemodel::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /*
    auto computer = Sprite::create("mainmenu/computer.png");
    computer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2));
    addChild(computer,4);
    */
    MenuItemFont::setFontSize(20);
    auto ur1 = FileUtils::getInstance()->getStringFromFile("Gamemodel/Gamemodel1.txt");
    auto game1 = MenuItemFont::create(ur1, CC_CALLBACK_1(Gamemodel::menuClickCallBack, this));
    auto ur2 = FileUtils::getInstance()->getStringFromFile("Gamemodel/Gamemodel2.txt");
    auto game2 = MenuItemFont::create(ur2, CC_CALLBACK_1(Gamemodel::menuClickCallBack, this));
    auto ur3 = FileUtils::getInstance()->getStringFromFile("Gamemodel/Gamemodel3.txt");
    auto game3 = MenuItemFont::create(ur3, CC_CALLBACK_1(Gamemodel::menuClickCallBack, this));
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(Gamemodel::menuClickCallBack, this));
    //MainmenuTag
    game1->setTag(1);
    game2->setTag(2);
    game3->setTag(3);
    back->setTag(0);
    auto menu = Menu::create(game1, game2, game3 , back, NULL);
    menu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(menu);
    menu->setColor(Color3B::BLACK);
    menu->alignItemsVertically();

    addBackGround();


    scheduleUpdate();
    return true;
}

void Gamemodel::addBackGround()
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
void Gamemodel::menuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    if (NULL != node)
    {
        auto Mainscene = Mainmenu::createScene();
        switch (tag)
        {
        case 0:
            Director::getInstance()->replaceScene(Mainscene);
            break;
        case 1:
            model = 1;
            break;
        case 2:
           model = 2;
            break;
        case 3:
            model = 3;
            break;
        default:
            break;
        }
    }
}

//监测Gamemodel是否改变
void Gamemodel::update(float delta) {
    if (model == 1) {
        Game1();
    }
    else if (model == 2)
        Game2();
    else if(model==3)
        Game3();
}

//改变场景至关卡选择
void Gamemodel::Game1()
{
    auto Gamescene = Gamemenu::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}

void Gamemodel::Game2()
{
    auto Gamescene2 = Gamemenu2::createScene();
    Director::getInstance()->replaceScene(Gamescene2);
}

void Gamemodel::Game3()
{
    auto Gamescene3 = Matching::createScene();
    Director::getInstance()->replaceScene(Gamescene3);
}