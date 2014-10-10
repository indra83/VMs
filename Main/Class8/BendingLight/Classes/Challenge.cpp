#include "Challenge.h"

#include "SimpleAudioEngine.h"
#include "PopUpScene.h"		// for pop up
#include "Util.h"			// for sound files and screenshot

USING_NS_CC;
using namespace CocosDenshion;

// create method for derived class using CRTP, as static methods cannot have polymorphism
// create method of a class creates the object of the class and add it to the autorelease pool.
template< class Derived >
Derived * Challenge<Derived>::create(bool showInfo)
{
	auto *pRet = new Derived();
	if(pRet && pRet->init(showInfo))
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
Scene * Challenge<Derived>::createScene(bool showInfo)
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

template< class Derived >
bool Challenge<Derived>::init(bool showInfo)
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// TODO: write code which will be initialized to all the challenges

	return true;
}

template< class Derived >
void Challenge<Derived>::addPopupMenu(const std::string & title, const std::string & caption, bool loud, bool replace)
{
    if (loud)
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHIME.c_str());

    captureScreenAsSprite(
                [=](Sprite * sprite) -> void
                {
                    auto scene = PopUp::createScene(title, caption, sprite);
                    if (replace)
                        Director::getInstance()->replaceScene(scene);
                    else
                        Director::getInstance()->pushScene(scene);
                });
}

template< class Derived >
void Challenge<Derived>::done(bool success)
{
	//TODO: write necessary code to do before showing results.
    if (success)
        addPopupMenu("Challenge Complete", "Congrats!!", true, true);
    else
        addPopupMenu("Failed!!", "Try Again", true, true);
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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//TODO: code here

    return true;
}

void Challenge1::showInfoPopup()
{
	addPopupMenu("INSTRUCTION", "DUMMY TEXT", false);
}

//////////////////////////////
// challenge2
//////////////////////////////

Scene* Challenge2::createScene(bool showInfo)
{
    return Challenge<Challenge2>::createScene(showInfo);
}

bool Challenge2::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//TODO: code here


    return true;
}

void Challenge2::showInfoPopup()
{
    addPopupMenu("INSTRUCTION", "DUMMY TEXT", false);
}

//////////////////////////////
// challenge3
//////////////////////////////

Scene* Challenge3::createScene(bool showInfo)
{
    return Challenge<Challenge3>::createScene(showInfo);
}

bool Challenge3::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //TODO: code here


    return true;
}

void Challenge3::showInfoPopup()
{
	addPopupMenu("INSTRUCTION", "DUMMY TEXT", false);
}
