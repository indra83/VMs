#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class BackGroundLayer;
class SpriteLayer;
class MenuLayer;


template< class Derived >
class Challenge: public cocos2d::Layer
{
public:
    Challenge() :
        _spriteLayer(nullptr),
        _bgLayer(nullptr),
        _menuLayer(nullptr) {}

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(bool showInfo);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(bool showInfo);

    // implement the "static create()" method manually
    static Derived * create(bool showInfo);

    virtual void showInfoPopup() = 0;
    void addPopupMenu(const std::string & title, const std::string & caption, bool loud = true, bool vibrate = false, bool replace = false );
    void done();
    virtual void forceValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void frictionValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    cocos2d::Menu* selectSurfaceFriction(const std::string &surface_no , const cocos2d::Vec2 &label_pos , const cocos2d::Vec2 &menu_pos);
    void radioSelectSurface(cocos2d::Object *pSender);
    // back button exit
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event);

protected:

    SpriteLayer * _spriteLayer;
    BackGroundLayer * _bgLayer;
    MenuLayer * _menuLayer;
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
    std::list<Node *> _trollies;
public:
    Challenge2(): _trollies(){}
    virtual bool init(bool showInfo) override;
    static cocos2d::Scene* createScene(bool showInfo);
    void showInfoPopup() override;
};

class Challenge3: public Challenge<Challenge3>
{
public:
	Challenge3(){}
	virtual bool init(bool showInfo) override;
	static cocos2d::Scene * createScene(bool showInfo);
	void showInfoPopup() override;
};

class Challenge4: public Challenge<Challenge4>
{
public:
	Challenge4():
		_timeLimit(60.0f),
		_timeLeft(0.0f) {}

	virtual bool init(bool showInfo) override;
	static cocos2d::Scene * createScene(bool showInfo);
	void showInfoPopup() override;
	void enableChal4Logic();
	void countDownTimer(float dt);
	void getSurfaceLabel(cocos2d::Menu * surf_labels /*, const cocos2d::Vec2 &surf_label_pos*/);
	cocos2d::LabelTTF * _timeLabel;
	cocos2d::LabelTTF * _secLabel;
	cocos2d::Menu * _surfSelectionMenu1;
	cocos2d::Menu * _surfSelectionMenu2;
	cocos2d::MenuItemImage * _play;
	std::string getTimeString();
private:
	float _timeLimit;
	float _timeLeft;
};

#endif // __CHALLENGE_H__
