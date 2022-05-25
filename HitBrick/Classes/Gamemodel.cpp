#include "cocos2d.h"
#include "Gamemodel.h"
#include "Gamemenu.h"
#include "Mainmenu.h"
#include "demo.h"
#include "store.h"

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
    //auto ur2 = FileUtils::getInstance()->getStringFromFile("Gamemodel/Gamemodel2.txt");
    auto game2 = MenuItemFont::create("2", CC_CALLBACK_1(Gamemodel::menuClickCallBack, this));
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(Gamemodel::menuClickCallBack, this));
    //MainmenuTag
    game1->setTag(1);
    game2->setTag(2);
    back->setTag(0);
    auto menu = Menu::create(game1, game2, back, NULL);
    menu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(menu);
    menu->alignItemsVertically();

    scheduleUpdate();
    return true;
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
}

//改变场景至关卡选择
void Gamemodel::Game1()
{
    auto Gamescene = Gamemenu::createScene();
    Director::getInstance()->replaceScene(Gamescene);
}