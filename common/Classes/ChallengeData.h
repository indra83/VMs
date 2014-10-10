#pragma once

#include <string>
struct ChallengeAttempt
{
    std::string _challengeId;
    int _totalPoints;
    int _subquestions;
    int _correctAttempts;
    int _wrongAttempts;
    std::string _data;
    ChallengeAttempt(const std::string & challengeId, int totalPoints,int subquestions,int correctAttempts,int wrongAttempts, const std::string & data) 
        : _challengeId(challengeId), _totalPoints(totalPoints), _subquestions(subquestions), 
          _correctAttempts(correctAttempts), _wrongAttempts(wrongAttempts), _data(data) {}
};

struct ChallengeInfo
{
    std::string _id;
    std::string _title;
    std::string _desc;

    ChallengeInfo(const std::string & id, const std::string & title, const std::string & desc) 
        : _id(id), _title(title), _desc(desc) {}
};
