#pragma once
#include "IDataBase.h"
#include "sqlite3.h"
#include "Question.h"
#include <string>


class SqliteDatabase : public IDataBase
{
public:
    SqliteDatabase();
    ~SqliteDatabase();
    bool open() override;
    bool close() override;
    int doesUserExist(const std::string& username) override;
    int doesPasswordMatch(const std::string& username, const std::string& password) override;
    int addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
    float getPlayerAverageAnswerTime(const std::string& username) override;
    int getNumOfCorrectAnswers(const std::string& username) override;
    int getNumOfTotalAnswers(const std::string& username) override;
    int getNumOfPlayerGames(const std::string& username) override;
    float getPlayerScore(const std::string& username) override;
    std::vector<std::string> getHighScores() override;
    Question getQuestion(int id) override;
    std::vector<Question> getQuestions(int amountOfQuestions) override;
    void executeSQLCommand(const std::string& command);
    void executeSQLCommand(const std::string& command, int(*callbackFunction)(void*, int, char**, char**));
    void executeSQLCommand(const std::string& command, int(*callbackFunction)(void*, int, char**, char**), void* data);

private:
    sqlite3* _db;
    std::string _dbName;
};