#include "AppDelegate.h"
#include "GameLoadScene.h"
#include "Challenge.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

    Challenge1::addGenerator(this);
    Challenge2::addGenerator(this);
    Challenge3::addGenerator(this);

    addStartingScenegen([]() -> Scene * 
    { 
        return GameLoad::createScene(); 
    });
}

AppDelegate::~AppDelegate() 
{
}

