#ifndef __GAMELOAD_SCENE_H__
#define __GAMELOAD_SCENE_H__

#include "cocos2d.h"

class GameLoad : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLoad);

    // back button exit
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);
private:
    bool onContactPreSolve(cocos2d::PhysicsContact &contact, cocos2d::PhysicsContactPreSolve &solve);
    void onContactPostSolve(cocos2d::PhysicsContact &contact, const cocos2d::PhysicsContactPostSolve &solve);

    float _current_res;
    ssize_t _vibration_length;
};

#endif // __GAMELOAD_SCENE_H__
