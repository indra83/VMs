#include "GameLoadScene.h"
#include "NativeHelper.h"
#include "Util.h"

USING_NS_CC;

Scene* GameLoad::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    //scene->getPhysicsWorld()->setGravity(Vect(0, -400));
    
    // 'layer' is an autorelease object
    auto layer = GameLoad::create();
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
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

    this->setKeypadEnabled(true);

    return true;
}

void GameLoad::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
	Director::getInstance()->end();
}

