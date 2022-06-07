#include "cocos2d.h"
#include "DESCRIPTION.h"
#include "Mainmenu.h"


USING_NS_CC;


Scene* DESCRIPTION::createScene()
{
    auto scene = Scene::create();
    auto layer = DESCRIPTION::create();
    scene->addChild(layer);
    return scene;
}

bool DESCRIPTION::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //��ȡ��Ļ����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    MenuItemFont::setFontSize(6);

    auto url = FileUtils::getInstance()->getStringFromFile("description.txt");
    auto txt = Label::create();
    txt->setString(url);
    txt->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    addChild(txt);

    MenuItemFont::setFontSize(20);
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(DESCRIPTION::menuClickCallBack, this));
    back->setTag(0);
    auto menu = Menu::create(back, NULL);
    menu->setPosition(visibleSize.width / 2 + origin.x, 30);
    menu->setColor(Color3B::BLACK);
    addChild(menu);
    addBackGround();
    scheduleUpdate();
    return true;
}
void DESCRIPTION::addBackGround()
{
    //ʵ�ֱ���ͼƬ������������
    auto bg = Sprite::create("Mainback.png");
    bg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    Size mywinsize = Director::getInstance()->getWinSize();
    float winw = mywinsize.width; //��ȡ��Ļ���
    float winh = mywinsize.height;//��ȡ��Ļ�߶�
    float spx = bg->getTextureRect().getMaxX();
    float spy = bg->getTextureRect().getMaxY();
    bg->setScaleX(winw / spx); //���þ��������ű���
    bg->setScaleY(winh / spy);
    this->addChild(bg, -3);

    return;
}
void DESCRIPTION::menuClickCallBack(Ref* sender)
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
        default:
            break;
        }
    }
}