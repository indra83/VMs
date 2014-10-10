#include "VmBase.h"

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

