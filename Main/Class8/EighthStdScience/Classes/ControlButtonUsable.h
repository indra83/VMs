#ifndef __CONTROL_BUTTON_USABLE_H
#define __CONTROL_BUTTON_USABLE_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class ControlButtonUsable: public cocos2d::extension::ControlButton
{
public :
    static ControlButtonUsable* create(const std::string &labelText, cocos2d::extension::Scale9Sprite* sprite)
    {
        ControlButtonUsable *pRet = new ControlButtonUsable();
        cocos2d::Label *label = cocos2d::Label::createWithSystemFont(labelText, "Arial", 30);//
        pRet->initWithLabelAndBackgroundSprite(label, sprite);
        pRet->autorelease();
        return pRet;
    }

    void setTouchDownHandler(Ref* target, Handler action)
    {
        addTargetWithActionForControlEvent(target, action, Control::EventType::TOUCH_DOWN);
    }
};

#endif // __CONTROL_BUTTON_USABLE_H
