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
    ChallengeAttempt(const ChallengeAttempt &ch)
        : _challengeId(ch._challengeId), _totalPoints(ch._totalPoints), _subquestions(ch._subquestions), 
          _correctAttempts(ch._correctAttempts), _wrongAttempts(ch._wrongAttempts), _data(ch._data) {}
    ChallengeAttempt() 
        : _challengeId(""), _totalPoints(0), _subquestions(0), _correctAttempts(0), _wrongAttempts(0), _data("") {} 
};

struct ChallengeInfo
{
    std::string _id;
    std::string _title;
    std::string _desc;

    ChallengeInfo(const std::string & id, const std::string & title, const std::string & desc) 
        : _id(id), _title(title), _desc(desc) {}

    ChallengeInfo(const ChallengeInfo & ch)
        : _id(ch._id), _title(ch._title), _desc(ch._desc) {}
    ChallengeInfo() 
        : _id(""), _title(""), _desc("") {}
};
