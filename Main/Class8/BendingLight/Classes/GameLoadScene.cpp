#include "GameLoadScene.h"

#include "BackGroundLayer.h"
#include "SpriteLayer.h"
#include "MenuLayer.h"

USING_NS_CC;

// z-index of game layers
#define BG_ZINDEX 1
#define SP_ZINDEX 2
#define MN_ZINDEX 3


Scene* GameLoad::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLoad::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLoad::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
	// 2. BackGroundLayer
    auto bgLayer = BackGroundLayer::create();
    this->addChild(bgLayer, BG_ZINDEX);

    //////////////////////////////
	// 3. SpriteLayer
    auto spLayer = SpriteLayer::create();
    this->addChild(spLayer, SP_ZINDEX);

    //////////////////////////////
	// 4. MenuLayer
    auto mnLayer = MenuLayer::create();
    this->addChild(mnLayer, MN_ZINDEX);

    // trigger to active key events
    this->setKeypadEnabled(true);

    return true;
}

void GameLoad::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	Director::getInstance()->end();
}
