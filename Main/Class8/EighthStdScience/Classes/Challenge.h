#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include "MenuLayer.h"

class BackGroundLayer;
class SpriteLayer;


template< class Derived >
class Challenge: public cocos2d::Layer
{
public:
    Challenge() :
        _spriteLayer(nullptr),
        _bgLayer(nullptr),
        _menuLayer(nullptr),
        _timeLabel(nullptr),
        _timeLeft(0.0),
        _warningTime(0.0) {}

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(bool showInfo);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(bool showInfo);

    // implement the "static create()" method manually
    static Derived * create(bool showInfo);

    virtual void showInfoPopup() = 0;
    void addPopupMenu(const std::string & title, const std::string & caption, bool loud = true, bool vibrate = false, bool replace = false );
    void addBaseSurface(MenuLayer::SurfaceType surf);
    void done(bool);
    virtual void forceValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void frictionValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void countDownTimer(float dt);
    void setupTimer(float howLong, float warning);
    std::string getTimeString(); 
    // back button exit
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);

protected:

    SpriteLayer * _spriteLayer;
    BackGroundLayer * _bgLayer;
    MenuLayer * _menuLayer;
private :
    float _timeLeft;
    cocos2d::LabelTTF * _timeLabel;
    float _warningTime;
};

class Challenge1: public Challenge<Challenge1>
{
public:
    Challenge1() : _friendHelpShown(false), _numMaxHits(0) {}
    virtual bool init(bool showInfo) override;
    static cocos2d::Scene* createScene(bool showInfo);
    void showInfoPopup() override;
    void forceValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent) override;
private:
    bool _friendHelpShown;
    int _numMaxHits;
};

class Challenge2: public Challenge<Challenge2>
{
    struct TrolleyInfo
    {
        float time;
        cocos2d::Action *animationPlaying;
        cocos2d::EventListener * eventListener;
        int direction;
    };

    std::map<Node *, TrolleyInfo> _trollies;
public:
    Challenge2(): _trollies(){}
    virtual bool init(bool showInfo) override;
    static cocos2d::Scene* createScene(bool showInfo);
    void showInfoPopup() override;
};

class Challenge3: public Challenge<Challenge3>
{
public:
	Challenge3() {}

	virtual bool init(bool showInfo) override;
	static cocos2d::Scene * createScene(bool showInfo);
	void showInfoPopup() override;
};

#endif // __CHALLENGE_H__
