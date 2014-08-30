#include "GameLayerScene.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"

#define BG_ZINDEX 0
#define SP_ZINDEX 1
#define MN_ZINDEX 2

USING_NS_CC;

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -100));
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


///////////////////////////////////////////////////////////////////////////
// created GameLayerScene (scene), add others as (layers) to this scene
//////////////////////////////////////////////////////////////////////////

    /////////////////////////////
    // 2. add the background layer
    auto bgLayer = BackGroundLayer::create();
    this->addChild(bgLayer, BG_ZINDEX);

    /////////////////////////////
    // 3. add the sprite layer
    auto spriteLayer = SpriteLayer::create();
    //spriteLayer->setBackGroundLayer(bgLayer);
    this->addChild(spriteLayer, SP_ZINDEX);

    /////////////////////////////
    // 4. add the menu layer
    auto menuLayer = MenuLayer::create();
    //menuLayer->setSpriteLayer(spriteLayer);
    this->addChild(menuLayer, MN_ZINDEX);

    // trigger to back button exit
    setKeypadEnabled(true);

    return true;
}


void GameLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
	Director::getInstance()->end();
}
