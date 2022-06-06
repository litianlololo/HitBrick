#include "cocos2d.h"
#include "Mainmenu.h"
#include "Gamemenu.h"
#include "Gamemodel.h"
#include "DESCRIPTION.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* Mainmenu::createScene()
{
    auto scene = Scene::create();
    auto layer =Mainmenu::create();
    scene->addChild(layer);
    return scene;
}

bool Mainmenu::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    MenuItemFont::setFontSize(20);
    auto GAME = MenuItemFont::create("GAME", CC_CALLBACK_1(Mainmenu::menuClickCallBack, this));
    auto description = MenuItemFont::create("DESCRIPTION", CC_CALLBACK_1(Mainmenu::menuClickCallBack, this));
    //MainmenuTag
    GAME->setTag(1);
    description->setTag(2);

    auto menu = Menu::create(GAME, description, NULL);
    menu->setPosition(visibleSize.width/2+origin.x,visibleSize.height/2);
    menu->setColor(Color3B::BLACK);
    addChild(menu);
    menu->alignItemsVertically();

    addBackGround();

    return true;
}
void Mainmenu::addBackGround()
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
    this->addChild(bg,-3);

    return;
}
void Mainmenu::menuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    if (NULL != node)
    {
        auto scene = Gamemodel::createScene();
        auto Dscene = DESCRIPTION::createScene();
        switch (tag)
        {
        case 1:
           
            Director::getInstance()->replaceScene(scene);
            break;
        case 2:
            Director::getInstance()->replaceScene(Dscene);
            break;
        default:
            break;
        }
    }
}