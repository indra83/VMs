#pragma once
#include "ChallengeData.h"
#include <string>
#include <vector>
#include <map>
class VmBase
{
    std::vector<ChallengeInfo> _info; 
    std::map<std::string, ChallengeAttempt> _state; 
public :
    void addChallengeDetails(const ChallengeInfo & challengeInfo);
    void addAttemptDetails(const ChallengeAttempt & attemptInfo);

};
