#pragma once

#include "cocos2d.h"
#include "VmBase.h"

class ChallengeBase: public cocos2d::Layer, public VmBase::AttemptListener 
{
protected :    
    std::string _id;

public :
    // back button exit
    virtual bool init(bool showInfo);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);
    void showInfoPopup();
    void addPopupMenu(const std::string & title, const std::string & caption, bool loud = true, bool vibrate = false, bool replace = false );
    cocos2d::Menu * getTopMenu();
    void done(bool success);
    void onAttempt(const ChallengeAttempt & att) override;
    void showScore();
};
