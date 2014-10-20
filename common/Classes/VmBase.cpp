#include "VmBase.h"
#include "co_in_divi_vms_CommonVMActivity.h"

USING_NS_CC;

VmBase::VmBase()
{
   for ( auto at : getCachedAttempts() ) 
       addAttemptDetails(at);
   for ( auto nf : getCachedInfos() ) 
       addChallengeDetails(nf);
   clearCachedData();
}

void VmBase::addChallengeDetails(const ChallengeInfo & challengeInfo)
{
    _info[challengeInfo._id] = challengeInfo;
    for(auto lis : _infoListeners)
        lis->onInfo(challengeInfo);
}

void VmBase::addAttemptDetails(const ChallengeAttempt & attemptInfo)
{
    _state[attemptInfo._challengeId] = attemptInfo;
    for(auto lis : _attemptListeners)
        lis->onAttempt(attemptInfo);
}

void VmBase::addChallengeGen(const std::string &id, std::function< cocos2d::Scene * (bool)> gen)
{
    _gens[id] = gen;
}

bool VmBase::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = getStartingScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void VmBase::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void VmBase::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
