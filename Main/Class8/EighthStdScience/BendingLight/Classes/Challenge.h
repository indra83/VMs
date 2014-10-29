#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include "MenuLayer.h"
#include "ChallengeBase.h"

class BackGroundLayer;
class SpriteLayer;


template< class Derived >
class Challenge: public ChallengeBase
{
public:
    Challenge() :
        _menuLayer(nullptr) {}

    static void addGenerator( VmBase * base )
    {
        base->addChallengeGen( Derived::getId(), [](bool showInfo) -> cocos2d::Scene *
        {
            return Derived::createScene(showInfo);
        });
    }

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(bool showInfo);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(bool showInfo) override;

    // implement the "static create()" method manually
    static Derived * create(bool showInfo);

protected:
    MenuLayer * _menuLayer;
};

class Challenge1: public Challenge<Challenge1>
{
public:
    static std::string getId() { return "challenge1"; }
    Challenge1() {}
    virtual bool init(bool showInfo) override;
    static cocos2d::Scene* createScene(bool showInfo);


    // slider event call
//    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::SliderEventType type);
	// back button exit
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

//	cocos2d::PhysicsWorld *sceneWorld;
//	void setPhysicsWorld(cocos2d::PhysicsWorld *world){sceneWorld = world;}

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	cocos2d::Sprite *sp_1;
	cocos2d::Sprite *sp_2;
	cocos2d::PhysicsBody *body_1;
	cocos2d::PhysicsBody *body_2;
private:
	int _tag;
};

#endif // __CHALLENGE_H__
