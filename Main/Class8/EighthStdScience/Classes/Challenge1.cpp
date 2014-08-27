#include "Challenge1.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"

#define BG_ZINDEX 0
#define SP_ZINDEX 1

USING_NS_CC;

Scene* Challenge1::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Challenge1::create();
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Challenge1::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add the background layer
    auto bgLayer = BackGroundLayer::create();
    this->addChild(bgLayer, BG_ZINDEX);

    // crate
    auto crate = Sprite::create("crate.png");
    crate->setAnchorPoint(Point(0.0, 0.0));
    crate->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/3));
    this->addChild(crate, SP_ZINDEX);

    this->setKeypadEnabled(true);

    return true;
}

void Challenge1::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
	Director::getInstance()->replaceScene(Challenge::createScene());
}
