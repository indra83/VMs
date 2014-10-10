#ifndef __CHALLENGE_MENU_SCENE_H__
#define __CHALLENGE_MENU_SCENE_H__

#include "cocos2d.h"

class ChallengeMenu : public cocos2d::Layer
{
    bool _fromChallenge;

public:
    ChallengeMenu() : _fromChallenge(false) {}

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(ChallengeMenu);

	// challenge select action function
	void touchDownAction(cocos2d::Ref *sender);

	// back button exit game
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
};

#endif //__CHALLENGE_MENU_SCENE_H__
