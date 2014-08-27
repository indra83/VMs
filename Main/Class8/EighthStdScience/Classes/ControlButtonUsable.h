#ifndef __CONTROL_BUTTON_USABLE_H
#define __CONTROL_BUTTON_USABLE_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class ControlButtonUsable: public cocos2d::extension::ControlButton
{
public :
    static ControlButtonUsable* create(cocos2d::extension::Scale9Sprite* sprite)
    {
        ControlButtonUsable *pRet = new ControlButtonUsable();
        pRet->initWithBackgroundSprite(sprite);
        pRet->autorelease();
        return pRet;
    }

    void setTouchDownHandler(Ref* target, Handler action)
    {
        addTargetWithActionForControlEvent(target, action, Control::EventType::TOUCH_DOWN);
    }
};

#endif // __CONTROL_BUTTON_USABLE_H
