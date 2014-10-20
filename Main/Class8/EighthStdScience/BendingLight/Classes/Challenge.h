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
};

#endif // __CHALLENGE_H__
