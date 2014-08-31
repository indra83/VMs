#ifndef __CHALLENGE_SCENE_H__
#define __CHALLENGE_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class ChallengeMenu : public cocos2d::Layer
{
    bool _fromChallenge;
public:
    ChallengeMenu() : _fromChallenge(false) {}
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(bool fromChallenge, cocos2d::Sprite * sprite);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChallengeMenu);

    // back button previous scene
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);
    void touchDownAction(cocos2d::Ref *sender);
};

#endif // __CHALLENGE_SCENE_H__
