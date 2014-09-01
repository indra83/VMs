#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class BackGroundLayer;
class SpriteLayer;
class MenuLayer;

template< class Derived >
class Challenge: public cocos2d::Layer
{
public:
    Challenge() :
        _spriteLayer(nullptr),
        _bgLayer(nullptr),
        _menuLayer(nullptr) {}

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    static Derived * create();

    void showInfoPopUp();
    void addPopupMenu(const std::string & title, const std::string & caption, bool loud = true, bool vibrate = false, bool replace = false );
    void done();
    virtual void forceValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void frictionValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    // back button exit
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);

protected:

    SpriteLayer * _spriteLayer;
    BackGroundLayer * _bgLayer;
    MenuLayer * _menuLayer;
};

class Challenge1: public Challenge<Challenge1>
{
public:
    Challenge1() : _friendHelpShown(false), _numMaxHits(0) {}
    virtual bool init();
    static cocos2d::Scene* createScene();
    void forceValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent) override;
private:
    bool _friendHelpShown;
    int _numMaxHits;
};

class Challenge4: public Challenge<Challenge4>
{
public:
    Challenge4(){}
    virtual bool init();
    static cocos2d::Scene* createScene();
private:
};

#endif // __CHALLENGE_H__
