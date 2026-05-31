#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Question.h"

class IDataBase
{
public:
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual int doesUserExist(const std::string& username) = 0;
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;
	virtual float getPlayerAverageAnswerTime(const std::string& username) = 0;
	virtual int getNumOfCorrectAnswers(const std::string& username) = 0;
	virtual int getNumOfTotalAnswers(const std::string& username) = 0;
	virtual int getNumOfPlayerGames(const std::string& username) = 0;
	virtual float getPlayerScore(const std::string& username) = 0;
	virtual std::vector<std::string> getHighScores() = 0;
	virtual Question getQuestion(int id) = 0;
	virtual std::vector<Question> getQuestions(int amountOfQuestions) = 0;
};