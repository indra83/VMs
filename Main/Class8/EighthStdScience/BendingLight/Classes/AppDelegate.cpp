#include "AppDelegate.h"
#include "ChallengeMenuScene.h"
#include "Challenge.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
    Challenge1::addGenerator(this);

    addStartingScenegen([]() -> Scene * 
    { 
        return ChallengeMenu::createScene(true, nullptr);
    });
}

AppDelegate::~AppDelegate() 
{
}

