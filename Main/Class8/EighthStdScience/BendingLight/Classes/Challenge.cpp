#include "Challenge.h"
#include "MenuLayer.h"
#include "ChallengeMenuScene.h"
#include "NativeHelper.h"
#include "PopUpScene.h"
#include "Util.h"
#include "ui/CocosGUI.h"


#include <iomanip> // setw, setfill

static const int MN_ZINDEX=2;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

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
    auto scene = Scene::createWithPhysics();
    
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

    Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // physics body 1
	sp_1 = Sprite::create("HelloWorld.png");
	sp_1->setPosition(Vec2(visibleSize.width/3, visibleSize.height/2));
	body_1 = PhysicsBody::createBox(sp_1->getContentSize(), PhysicsMaterial(0,1,0));
	sp_1->setPhysicsBody(body_1);
	this->addChild(sp_1);

	// physics body 2
	sp_2 = Sprite::create("HelloWorld.png");
	sp_2->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	body_2 = PhysicsBody::createBox(sp_2->getContentSize(), PhysicsMaterial(0,1,0));
	sp_2->setPhysicsBody(body_2);
	this->addChild(sp_2);


//	 joints
	auto joint = PhysicsJointLimit::construct(body_1, body_2, Vec2::ZERO, Vec2::ANCHOR_TOP_RIGHT);
	joint->setCollisionEnable(false);
//	joint->setDistance(50);
//	sceneWorld->addJoint(joint);	// TODO solve the problem here, it crashes
//	this->getScene()->getPhysicsWorld()->addJoint(joint);



	// listener for touch
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Challenge1::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Challenge1::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Challenge1::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


//	// ui slider
//	auto slider_surf1 = Slider::create();
//	slider_surf1->loadBarTexture("sliderTrack.png");
//	slider_surf1->loadSlidBallTextures("slider_hangle.png", "slider_handle.png", "slider_handle.png");
//	slider_surf1->loadProgressBarTexture("sliderProgress.png");
//	slider_surf1->setPosition(Vec2(visibleSize.width/2, 3 * visibleSize.height/4));
//	slider_surf1->addEventListener(CC_CALLBACK_2(HelloWorld::sliderEvent, this));
//	this->addChild(slider_surf1);


	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

   return true;
}

//void Challenge1::sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::SliderEventType type)
//{
//	if(type == Slider::EventType::ON_PERCENTAGE_CHANGED)
//	{
//		auto slider = dynamic_cast<Slider *>(pSender);
//		int percent = slider->getPercent();
//		CCLOG(percent);
//	}
//}

bool Challenge1::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	// condition for touch to the sprite
	auto pos1_point = sp_1->getPosition();
	auto pos2_point = sp_2->getPosition();

	auto pos1 = Vec2(pos1_point.x - (sp_1->getContentSize().width/2),
			(pos1_point.y - (sp_1->getContentSize().height/2)));

	auto pos2 = Vec2(pos2_point.x - (sp_2->getContentSize().width/2),
				(pos2_point.y - (sp_2->getContentSize().height/2)));

	// get the touch location in points
	Vec2 touchLoc = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

	if( (touchLoc.x > pos1.x && touchLoc.x < (pos1.x + sp_1->getContentSize().width)) &&
			(touchLoc.y > pos1.y && touchLoc.y < (pos1.y + sp_1->getContentSize().height)) )
	{
		_tag = 1;
		return true;
	}

	else if( (touchLoc.x > pos2.x && touchLoc.x < (pos2.x + sp_2->getContentSize().width))
					&& (touchLoc.y > pos2.y && touchLoc.y < (pos2.y + sp_2->getContentSize().height)) )
	{
		_tag = 2;
		return true;
	}

return false;
}

void Challenge1::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	Vec2 touchL = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
	if(_tag == 1)
		sp_1->setPosition(touchL);
	else if(_tag == 2)
		sp_2->setPosition(touchL);
}

void Challenge1::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {}

void Challenge1::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	Director::getInstance()->end();
}

