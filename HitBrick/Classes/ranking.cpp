#include "cocos2d.h"
#include "Gamemodel.h"
#include "Gamemenu.h"
#include "Gamemenu2.h"
#include "Mainmenu.h"
#include "SpeedGame.h"
#include "store.h"
#include "ranking.h"

USING_NS_CC;
//构造函数 初始化
ranking::ranking():model(0),gamechoice(0)
{

}

Scene* ranking::createScene()
{
    auto scene = Scene::create();
    auto layer = ranking::create();
    scene->addChild(layer);
    return scene;
}

bool ranking::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    bg();
    getscore();
    gettime();
    addBackGround();
    scheduleUpdate();
    return true;
}

void ranking::addBackGround()
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
void ranking::menuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    if (NULL != node)
    {
        auto Gamemenuscene =Gamemenu2::createScene();
        auto SpeedGame = HitBrick2::createScene();
        switch (tag)
        {
        case 0: 
            Director::getInstance()->replaceScene(Gamemenuscene);
            break;
        case 1:
           
            Director::getInstance()->replaceScene(SpeedGame);
            break;
        default:
            break;
        }
    }
}


void ranking::update(float delta) {
    
}

void ranking::bg(){
    visibleSize = Director::getInstance()->getVisibleSize();                              //获取屏幕坐标
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    MenuItemFont::setFontSize(20);
    auto ur1 = FileUtils::getInstance()->getStringFromFile("ranking/start.txt");
    auto start = MenuItemFont::create(ur1, CC_CALLBACK_1(ranking::menuClickCallBack, this));
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(ranking::menuClickCallBack, this));
    //MainmenuTag
    start->setTag(1);
    back->setTag(0);
    auto menu = Menu::create(start, back, NULL);
    menu->setColor(Color3B::BLACK);
    menu->setPosition(visibleSize.width / 2 + origin.x + 100, visibleSize.height / 2);
    addChild(menu);
    menu->alignItemsVertically();


    //ranking
    MenuItemFont::setFontSize(20);
    //auto ur1 = FileUtils::getInstance()->getStringFromFile("ranking/start.txt");
    auto r1 = MenuItemImage::create("ranking/1.png", "ranking/1.png", CC_CALLBACK_1(ranking::menuClickCallBack, this));
    auto r2 = MenuItemImage::create("ranking/2.png", "ranking/2.png", CC_CALLBACK_1(ranking::menuClickCallBack, this));
    auto r3 = MenuItemImage::create("ranking/3.png", "ranking/3.png", CC_CALLBACK_1(ranking::menuClickCallBack, this));
    auto r4 = MenuItemImage::create("ranking/4.png", "ranking/4.png", CC_CALLBACK_1(ranking::menuClickCallBack, this));
    auto r5 = MenuItemImage::create("ranking/5.png", "ranking/5.png", CC_CALLBACK_1(ranking::menuClickCallBack, this));
    //MainmenuTag
    r1->setTag(11);
    r2->setTag(12);
    r3->setTag(13);
    r4->setTag(14);
    r5->setTag(15);
    auto rank = Menu::create(r1, r2, r3, r4, r5, NULL);
    rank->setPosition(visibleSize.width / 2 + origin.x - 140, visibleSize.height / 2);
    addChild(rank);
    rank->alignItemsVertically();
}

void ranking::getscore()
{
    gamechoice = UserDefault::getInstance()->getIntegerForKey("Gamechoice");

    std::string strscore1;
    std::string strscore2;
    std::string strscore3;
    std::string strscore4; 
    std::string strscore5;

    Label* score1; 
    Label* score2;
    Label* score3;
    Label* score4;
    Label* score5;

    switch (gamechoice) {

    case 1:
        strscore1= StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s1hscore1"));
        score1 = Label::createWithTTF(strscore1, "fonts/Marker Felt.ttf", 20);
        score1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 70));
        addChild(score1);

        strscore2 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s1hscore2"));
        score2 = Label::createWithTTF(strscore2, "fonts/Marker Felt.ttf", 20);
        score2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 32));
        addChild(score2);

        strscore3 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s1hscore3"));
        score3 = Label::createWithTTF(strscore3, "fonts/Marker Felt.ttf", 20);
        score3->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 -2));
        addChild(score3);

        strscore4 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s1hscore4"));
        score4 = Label::createWithTTF(strscore4, "fonts/Marker Felt.ttf", 20);
        score4->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 34));
        addChild(score4);

        strscore5 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s1hscore5"));
        score5 = Label::createWithTTF(strscore5, "fonts/Marker Felt.ttf", 20);
        score5->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 -72));
        addChild(score5);

        break;
    case 2:
        strscore1 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s2hscore1"));
        score1 = Label::createWithTTF(strscore1, "fonts/Marker Felt.ttf", 20);
        score1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 70));
        addChild(score1);

        strscore2 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s2hscore2"));
        score2 = Label::createWithTTF(strscore2, "fonts/Marker Felt.ttf", 20);
        score2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 32));
        addChild(score2);

        strscore3 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s2hscore3"));
        score3 = Label::createWithTTF(strscore3, "fonts/Marker Felt.ttf", 20);
        score3->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 2));
        addChild(score3);

        strscore4 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s2hscore4"));
        score4 = Label::createWithTTF(strscore4, "fonts/Marker Felt.ttf", 20);
        score4->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 34));
        addChild(score4);

        strscore5 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s2hscore5"));
        score5 = Label::createWithTTF(strscore5, "fonts/Marker Felt.ttf", 20);
        score5->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 72));
        addChild(score5);
        
        break;

    case 3:
        strscore1 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s3hscore1"));
        score1 = Label::createWithTTF(strscore1, "fonts/Marker Felt.ttf", 20);
        score1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 70));
        addChild(score1);

        strscore2 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s3hscore2"));
        score2 = Label::createWithTTF(strscore2, "fonts/Marker Felt.ttf", 20);
        score2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 32));
        addChild(score2);

        strscore3 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s3hscore3"));
        score3 = Label::createWithTTF(strscore3, "fonts/Marker Felt.ttf", 20);
        score3->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 2));
        addChild(score3);

        strscore4 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s3hscore4"));
        score4 = Label::createWithTTF(strscore4, "fonts/Marker Felt.ttf", 20);
        score4->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 34));
        addChild(score4);

        strscore5 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s3hscore5"));
        score5 = Label::createWithTTF(strscore5, "fonts/Marker Felt.ttf", 20);
        score5->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 72));
        addChild(score5);
        break;
    case 4:
        strscore1 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s4hscore1"));
        score1 = Label::createWithTTF(strscore1, "fonts/Marker Felt.ttf", 20);
        score1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 70));
        addChild(score1);

        strscore2 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s4hscore2"));
        score2 = Label::createWithTTF(strscore2, "fonts/Marker Felt.ttf", 20);
        score2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 + 32));
        addChild(score2);

        strscore3 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s4hscore3"));
        score3 = Label::createWithTTF(strscore3, "fonts/Marker Felt.ttf", 20);
        score3->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 2));
        addChild(score3);

        strscore4 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s4hscore4"));
        score4 = Label::createWithTTF(strscore4, "fonts/Marker Felt.ttf", 20);
        score4->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 34));
        addChild(score4);

        strscore5 = StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("s4hscore5"));
        score5 = Label::createWithTTF(strscore5, "fonts/Marker Felt.ttf", 20);
        score5->setPosition(Vec2(visibleSize.width / 2 + origin.x - 60, visibleSize.height / 2 - 72));
        addChild(score5);

        break;
    }
}

void ranking::gettime()
{
    gamechoice = UserDefault::getInstance()->getIntegerForKey("Gamechoice");

    std::string strtime1;
    std::string strtime2;
    std::string strtime3;
    std::string strtime4;
    std::string strtime5;

    Label* time1;
    Label* time2;
    Label* time3;
    Label* time4;
    Label* time5;

    switch (gamechoice) {

    case 1:
        strtime1 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s1htime1"));
        time1 = Label::createWithTTF(strtime1, "fonts/Marker Felt.ttf", 20);
        time1->setPosition(Vec2(visibleSize.width / 2 + origin.x+20, visibleSize.height / 2 + 70));
        addChild(time1);

        strtime2 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s1htime2"));
        time2 = Label::createWithTTF(strtime2, "fonts/Marker Felt.ttf", 20);
        time2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 + 32));
        addChild(time2);

        strtime3 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s1htime3"));
        time3 = Label::createWithTTF(strtime3, "fonts/Marker Felt.ttf", 20);
        time3->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 2));
        addChild(time3);

        strtime4 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s1htime4"));
        time4 = Label::createWithTTF(strtime4, "fonts/Marker Felt.ttf", 20);
        time4->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 34));
        addChild(time4);

        strtime5 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s1htime5"));
        time5 = Label::createWithTTF(strtime5, "fonts/Marker Felt.ttf", 20);
        time5->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 72));
        addChild(time5);

        break;
    case 2:
        strtime1 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s2htime1"));
        time1 = Label::createWithTTF(strtime1, "fonts/Marker Felt.ttf", 20);
        time1->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 + 70));
        addChild(time1);

        strtime2 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s2htime2"));
        time2 = Label::createWithTTF(strtime2, "fonts/Marker Felt.ttf", 20);
        time2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 + 32));
        addChild(time2);

        strtime3 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s2htime3"));
        time3 = Label::createWithTTF(strtime3, "fonts/Marker Felt.ttf", 20);
        time3->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 2));
        addChild(time3);

        strtime4 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s2htime4"));
        time4 = Label::createWithTTF(strtime4, "fonts/Marker Felt.ttf", 20);
        time4->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 34));
        addChild(time4);

        strtime5 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s2htime5"));
        time5 = Label::createWithTTF(strtime5, "fonts/Marker Felt.ttf", 20);
        time5->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 72));
        addChild(time5);

        break;

    case 3:
        strtime1 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s3htime1"));
        time1 = Label::createWithTTF(strtime1, "fonts/Marker Felt.ttf", 20);
        time1->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 + 70));
        addChild(time1);

        strtime2 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s3htime2"));
        time2 = Label::createWithTTF(strtime2, "fonts/Marker Felt.ttf", 20);
        time2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 + 32));
        addChild(time2);

        strtime3 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s3htime3"));
        time3 = Label::createWithTTF(strtime3, "fonts/Marker Felt.ttf", 20);
        time3->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 2));
        addChild(time3);

        strtime4 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s3htime4"));
        time4 = Label::createWithTTF(strtime4, "fonts/Marker Felt.ttf", 20);
        time4->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 34));
        addChild(time4);

        strtime5 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s3htime5"));
        time5 = Label::createWithTTF(strtime5, "fonts/Marker Felt.ttf", 20);
        time5->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 72));
        addChild(time5);

        break;
    case 4:
        strtime1 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s4htime1"));
        time1 = Label::createWithTTF(strtime1, "fonts/Marker Felt.ttf", 20);
        time1->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 + 70));
        addChild(time1);

        strtime2 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s4htime2"));
        time2 = Label::createWithTTF(strtime2, "fonts/Marker Felt.ttf", 20);
        time2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 + 32));
        addChild(time2);

        strtime3 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s4htime3"));
        time3 = Label::createWithTTF(strtime3, "fonts/Marker Felt.ttf", 20);
        time3->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 2));
        addChild(time3);

        strtime4 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s4htime4"));
        time4 = Label::createWithTTF(strtime4, "fonts/Marker Felt.ttf", 20);
        time4->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 34));
        addChild(time4);

        strtime5 = StringUtils::format("%d s", UserDefault::getInstance()->getIntegerForKey("s4htime5"));
        time5 = Label::createWithTTF(strtime5, "fonts/Marker Felt.ttf", 20);
        time5->setPosition(Vec2(visibleSize.width / 2 + origin.x + 20, visibleSize.height / 2 - 72));
        addChild(time5);

        break;
    }
}




