#include "ChallengeBase.h"
#include "ChallengeMenuScene.h"
#include "Util.h"
#include "PopUpScene.h"
#include "NativeHelper.h"
#include "VmBase.h"

USING_NS_CC;

static const std::string CHIME("audio/notification.mp3");

bool ChallengeBase::init(bool showInfo)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(CHIME.c_str());
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
    // enable keypress cbs
    this->setKeypadEnabled(true);


    // show initial info popup
    if (showInfo)
        showInfoPopup();
    return true;
}

void ChallengeBase::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
    Director::getInstance()->popScene();
}

cocos2d::Menu * ChallengeBase::getTopMenu()
{
    static const float PADDING = 10.0;
    cocos2d::Menu * menu =  Menu::create();
    float topMenuOffsetX;
    auto addToMenu = [&](MenuItem * item, const std::function< void (Ref *)> &cb) -> void
    {
        item->setCallback(cb);
        item->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        topMenuOffsetX -= PADDING;
        item->setPosition(topMenuOffsetX, -PADDING);
        topMenuOffsetX -= item->getContentSize().width * item->getScale();
        menu->addChild(item);
    };

    // add the reset button
    auto restart_scene = MenuItemImage::create("reset_normal.png", "reset_normal.png");
    restart_scene->setScale(0.8);
    addToMenu(restart_scene,
                [=](Ref * sender) -> void
                {
                    auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
                    if (!base)
                        return;
                    auto gen = base->getChallengeInitializers()[_id];
                    Director::getInstance()->replaceScene(gen(false));
                });

    // add the info button
    auto info = MenuItemImage::create("info.png", "info.png");
    info->setScale(0.8);
    addToMenu(info,
              [this](Ref * sender) -> void
              {
                  showInfoPopup();
              });

    // add the challenges menu
    auto list = MenuItemImage::create("menu.png", "menu.png");
    list->setScale(0.8);
    addToMenu(list,
              [](Ref * sender) -> void
              {
                    Director::getInstance()->pushScene(ChallengeMenu::createScene(true, nullptr));
              });
    return menu;
}


void ChallengeBase::addPopupMenu(const std::string & title, const std::string & caption, bool loud, bool vibrate, bool replace)
{
    if (loud)
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHIME.c_str());
    if (vibrate)
        NativeHelper::vibrate(500);

    captureScreenAsSprite(
                [=](Sprite * sprite) -> void
                {
                    auto scene = PopUp::createScene(title, caption, sprite);
                    if (replace)
                        Director::getInstance()->replaceScene(scene);
                    else
                        Director::getInstance()->pushScene(scene);
                });
}

void ChallengeBase::showInfoPopup()
{
 // TODO: get data from Base and show here 
	addPopupMenu("INSTRUCTION", "Move the box to the right by applying force on it. Drag and hold the slider in position to apply a specific amount of force."
	    		" Help the box reach the house which is some distance away", false);
}

void ChallengeBase::done(bool success)
{
    // TODO: call save score from here
    if (success)
        addPopupMenu("Challenge Complete", "Congrats!!", true, true, true);
    else
        addPopupMenu("Failed!!", "Try Again", true, true, true);
}

void ChallengeBase::showScore()
{
    //TODO: show scores as a seperate widget
    
}

void ChallengeBase::onAttempt(const ChallengeAttempt & att)
{
    if ( _id == att._challengeId )
        showScore();
}


