#ifndef __POPUP_SCENE_H__
#define __POPUP_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class PopUp: public cocos2d::Layer
{
    cocos2d::LabelTTF * _labelTitle;
    cocos2d::LabelTTF * _labelCaption;
public :
    static cocos2d::Scene* createScene(const std::string &title, const std::string &caption, cocos2d::Sprite * sprite);

    PopUp() : _labelTitle(nullptr), _labelCaption(nullptr) {}
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(PopUp);

    // back button previous scene
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);
 
};

#endif // __POPUP_SCENE_H__
