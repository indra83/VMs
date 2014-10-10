#pragma once
#include "ChallengeData.h"
#include "cocos2d.h"
#include <string>
#include <vector>
#include <map>
class VmBase
{
    std::map<std::string, ChallengeInfo> _info; 
    std::map<std::string, ChallengeAttempt> _state; 
    std::map<std::string, std::function< cocos2d::Scene * (bool) >> _gens; 
    std::function< cocos2d::Scene * () > _startGen; 
public :
    void addChallengeDetails(const ChallengeInfo & challengeInfo);
    void addAttemptDetails(const ChallengeAttempt & attemptInfo);
    void addChallengeGen(const std::string &id, std::function< cocos2d::Scene * (bool)> gen);
    std::map<std::string, std::function< cocos2d::Scene * (bool) >> & getChallengeInitializers() { return _gens; }
    std::map<std::string, ChallengeInfo> & getChallengeInfo() { return _info; }
    void addStartingScenegen( std::function< cocos2d::Scene * ()> gen ) { _startGen = gen; }
    cocos2d::Scene * getStartingScene() { return _startGen(); }
};
