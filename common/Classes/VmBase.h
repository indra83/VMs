#pragma once
#include "ChallengeData.h"
#include "cocos2d.h"
#include <string>
#include <vector>
#include <map>

class VmBase : public cocos2d::Application
{
public :    
    class InfoListener
    {
    public :    
        InfoListener() 
        { 
            auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
            if (base)
                base->addInfoListener(this);
        }
        ~InfoListener()
        {
            auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
            if (base)
                base->removeInfoListener(this);
        }
        virtual void onInfo(const ChallengeInfo & att) {}
    };

    class AttemptListener
    {
    public :    
        AttemptListener() 
        { 
            auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
            if (base)
                base->addAttemptListener(this);
        }

        ~AttemptListener()
        {
            auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
            if (base)
                base->removeAttemptListener(this);
        }

        virtual void onAttempt(const ChallengeAttempt & att) {}
    };

private :
    std::map<std::string, ChallengeInfo> _info; 
    std::map<std::string, ChallengeAttempt> _state; 
    std::map<std::string, std::function< cocos2d::Scene * (bool) >> _gens; 
    std::function< cocos2d::Scene * () > _startGen; 
    std::vector<InfoListener*> _infoListeners;
    std::vector<AttemptListener*> _attemptListeners;
public :
    VmBase();
    void addChallengeDetails(const ChallengeInfo & challengeInfo);
    void addAttemptDetails(const ChallengeAttempt & attemptInfo);
    void addChallengeGen(const std::string &id, std::function< cocos2d::Scene * (bool)> gen);
    std::map<std::string, std::function< cocos2d::Scene * (bool) >> & getChallengeInitializers() { return _gens; }
    std::map<std::string, ChallengeInfo> & getChallengeInfo() { return _info; }
    std::map<std::string, ChallengeAttempt> & getAttemptData() { return _state; }
    void addStartingScenegen( std::function< cocos2d::Scene * ()> gen ) { _startGen = gen; }
    cocos2d::Scene * getStartingScene() { return _startGen(); }

    void addInfoListener(InfoListener* lis) { _infoListeners.push_back(lis); }
    void addAttemptListener(AttemptListener* lis) { _attemptListeners.push_back(lis); }

    void removeInfoListener(InfoListener * lis) 
    {
        auto found = std::find(_infoListeners.begin(), _infoListeners.end(), lis);
        if (found != _infoListeners.end()) 
            _infoListeners.erase(found);
    }

    void removeAttemptListener(AttemptListener * lis) 
    {
        auto found = std::find(_attemptListeners.begin(), _attemptListeners.end(), lis);
        if (found != _attemptListeners.end()) 
            _attemptListeners.erase(found);
    }

    virtual bool applicationDidFinishLaunching();

    virtual void applicationDidEnterBackground();

    virtual void applicationWillEnterForeground();
};
