#include "VmBase.h"

void VmBase::addChallengeDetails(const ChallengeInfo & challengeInfo)
{
    //TODO: trigger all the listeners
    _info[challengeInfo._id] = challengeInfo;
}

void VmBase::addAttemptDetails(const ChallengeAttempt & attemptInfo)
{
    //TODO: trigger all the listeners
    _state[attemptInfo._challengeId] = attemptInfo;
}

void VmBase::addChallengeGen(const std::string &id, std::function< cocos2d::Scene * (bool)> gen)
{
    _gens[id] = gen;
}

