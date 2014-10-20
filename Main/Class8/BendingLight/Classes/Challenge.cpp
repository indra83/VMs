#include "Challenge.h"
#include "MenuLayer.h"
#include "ChallengeMenuScene.h"
#include "NativeHelper.h"
#include "PopUpScene.h"
#include "Util.h"

#include <iomanip> // setw, setfill

static const int MN_ZINDEX=2;

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

template< class Derived >
Derived * Challenge<Derived>::create(bool showInfo)
{
    auto *pRet = new Derived();
    if (pRet && pRet->init(showInfo))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

template< class Derived >
Scene* Challenge<Derived>::createScene(bool showInfo)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Challenge<Derived>::create(showInfo);
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
template< class Derived >
bool Challenge<Derived>::init(bool showInfo)
{
    _id = Derived::getId();

    //////////////////////////////
    // 1. super init first
    if ( !ChallengeBase::init(showInfo) )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
    // /////////////////////////////
    // // 2. add the menu layer
    _menuLayer = MenuLayer::create();

    // // add common top layer
    _menuLayer->setTopMenu(getTopMenu());

     this->addChild(_menuLayer, MN_ZINDEX);

   return true;
}

//////////////////////////////
// challenge1
//////////////////////////////

Scene* Challenge1::createScene(bool showInfo)
{
    return Challenge<Challenge1>::createScene(showInfo);
}

// on "init" you need to initialize your instance
bool Challenge1::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }

   return true;
}

