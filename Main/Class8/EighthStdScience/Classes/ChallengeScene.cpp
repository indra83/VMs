#include "ChallengeScene.h"

USING_NS_CC;

Scene* Challenge::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Challenge::create();

    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Challenge::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // 2. adding an empty challenge pop up

    // main layer
    auto popup_main = LayerColor::create(Color4B(255,255,255,255) , visibleSize.width , visibleSize.height/3);
    popup_main->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2));
    this->addChild(popup_main , 1);

    // translucent layer
    auto popup_tls = LayerColor::create(Color4B(255,255,255,0) , visibleSize.width , visibleSize.height);
    popup_tls->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2));
    this->addChild(popup_tls , 0);


    return true;

}
