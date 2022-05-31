#include "cocos2d.h"
#include "Mainmenu.h"
#include "Gamemenu.h"
#include "Gamemodel.h"
#include "DESCRIPTION.h"
#include "ui/CocosGUI.h"
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
    visibleSize = Director::getInstance()->getVisibleSize();                              //»ñÈ¡ÆÁÄ»×ø±ê
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /*
    auto computer = Sprite::create("mainmenu/computer.png");
    computer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2));
    addChild(computer,4);
    */
    MenuItemFont::setFontSize(20);
    auto GAME = MenuItemFont::create("GAME", CC_CALLBACK_1(Mainmenu::menuClickCallBack, this));
    auto description = MenuItemFont::create("DESCRIPTION", CC_CALLBACK_1(Mainmenu::menuClickCallBack, this));
    //MainmenuTag
    GAME->setTag(1);
    description->setTag(2);

    auto menu = Menu::create(GAME, description, NULL);
    menu->setPosition(visibleSize.width/2+origin.x,visibleSize.height/2);
    addChild(menu);
    menu->alignItemsVertically();


    return true;
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