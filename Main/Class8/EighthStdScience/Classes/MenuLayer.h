#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class SpriteLayer;
class BackGroundLayer;

class MenuLayer : public cocos2d::Layer
{
private :    
    SpriteLayer * _spriteLayer;

    cocos2d::MenuItemToggle *_sof;
    cocos2d::MenuItemImage *restart_scene;
public :  
    MenuLayer() 
        : _spriteLayer(nullptr), 
          _sof(nullptr)
    {}
    virtual bool init();  

    void restartScene(cocos2d:: Ref* pSender);
    void menuLeftCallback(cocos2d::Ref* pSender);
    void menuRightCallback(cocos2d::Ref* pSender);

    void menuCallbackShowSumOfForces(cocos2d::Ref* pSender);
    void menuCallbackShowValues(cocos2d::Ref* pSender);
    void menuCallbackShowForces(cocos2d::Ref* pSender);
    void menuCallbackShowMasses(cocos2d::Ref* pSender);
    void menuCallbackShowSpeed(cocos2d::Ref* pSender);

    void forceValueChanged(cocos2d::Ref* pSender, cocos2d::extension::Control::EventType controlEvent);
    void frictionValueChanged(cocos2d::Ref* pSender, cocos2d::extension::Control::EventType controlEvent);
    CREATE_FUNC(MenuLayer);

    void setSpriteLayer(SpriteLayer * layer);
    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
