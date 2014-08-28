#ifndef __CHALLENGE1_SCENE_H__
#define __CHALLENGE1_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class BackGroundLayer;
class SpriteLayer;
class MenuLayer;

class Challenge1: public cocos2d::Layer
{
public:
    Challenge1() :
        _spriteLayer(nullptr),
        _bgLayer(nullptr),
        _menuLayer(nullptr) {}

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(Challenge1);

    void restartScene(cocos2d::Ref* pSender);
    void forceValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    // back button exit
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);
private:

    SpriteLayer * _spriteLayer;
    BackGroundLayer * _bgLayer;
    MenuLayer * _menuLayer;
};

#endif // __CHALLENGE1_SCENE_H__
