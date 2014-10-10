#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "cocos2d.h"

USING_NS_CC;

// forward declaration of classes
//class BackGroundLayer;
//class SpriteLayer;
//class MenuLayer;

template < class Derived >
class Challenge : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(bool showInfo);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(bool showInfo);

	// implement the "static create()" method manually
	// manual implementation of CREATE_FUNC
	static Derived * create(bool showInfo);

    virtual void showInfoPopup() = 0;

    // challenge instruction dialog function call
    void addPopupMenu(const std::string & title, const std::string & caption, bool loud = true, bool replace = false );

    // challenge pass/fail dialog function call
    void done(bool);

protected:
//    SpriteLayer * _spriteLayer;
//    BackGroundLayer * _bgLayer;
//    MenuLayer * _menuLayer;
};

class Challenge1: public Challenge<Challenge1>
{
public:
    virtual bool init(bool showInfo) override;
    static cocos2d::Scene* createScene(bool showInfo);
    void showInfoPopup() override;
};

class Challenge2: public Challenge<Challenge2>
{
public:
    virtual bool init(bool showInfo) override;
    static cocos2d::Scene* createScene(bool showInfo);
    void showInfoPopup() override;
};

class Challenge3: public Challenge<Challenge3>
{
public:
	virtual bool init(bool showInfo) override;
	static cocos2d::Scene * createScene(bool showInfo);
	void showInfoPopup() override;
};

#endif // __CHALLENGE_H__

