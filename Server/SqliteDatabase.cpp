#include <iostream>
#include <string>
#include "SqliteDatabase.h"
#include "Callback.h"
#include "json.hpp"


#define ID_INDEX 0
#define QUESTION_INDEX 1
#define ANSWER_START_INDEX 2
#define ANSWER_END_INDEX 5
#define CORRECT_ANS_INDEX 6
#define QUESTIONS_IN_DB 10

/**
 * Initializes the database object and opens the connection
 *  Throws an exception if the database file fails to open
 */
SqliteDatabase::SqliteDatabase()
    : _db(nullptr), _dbName("trivia.db")
{
    if (!open())
        throw std::exception("Unable to open .db file");
}

/**
 * Safely closes the database connection
 */
SqliteDatabase::~SqliteDatabase()
{
    close();
}

/**
 * Opens the connection to the SQLite database and creates the users questions and statistics tables if they do not exist
 * @return true if the database was opened and the tables were created successfully false otherwise
 */
bool SqliteDatabase::open()
{
    int res = sqlite3_open(_dbName.c_str(), &_db);
    if (res != SQLITE_OK)
    {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(_db) << std::endl;
        return false;
    }

    const char* createTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY NOT NULL,"
        "password TEXT NOT NULL,"
        "email    TEXT NOT NULL"
        ");";
    executeSQLCommand(createTable);

    const char* createQuestions =
        "CREATE TABLE IF NOT EXISTS questions ("
        "id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "question   TEXT NOT NULL,"
        "answer1    TEXT NOT NULL,"
        "answer2    TEXT NOT NULL,"
        "answer3    TEXT NOT NULL,"
        "answer4    TEXT NOT NULL,"
        "correct_ans INTEGER NOT NULL"
        ");";
    executeSQLCommand(createQuestions);

    int count = 0;
    executeSQLCommand("SELECT COUNT(*) FROM questions;", Callback::intCallback, &count);

    if (count == 0)
    {
        const char* insertQuestions =
            "INSERT INTO questions (question, answer1, answer2, answer3, answer4, correct_ans) VALUES "
            "('What is the capital of France?', 'Paris', 'London', 'Berlin', 'Madrid', 1),"
            "('Who invented the telephone?', 'Edison', 'Bell', 'Tesla', 'Marconi', 2),"
            "('How many legs does a spider have?', '6', '4', '8', '10', 3),"
            "('What planet is closest to the sun?', 'Venus', 'Earth', 'Mars', 'Mercury', 4),"
            "('What is the largest ocean?', 'Atlantic', 'Indian', 'Arctic', 'Pacific', 4),"
            "('How many sides does a hexagon have?', '5', '7', '6', '8', 3),"
            "('Who painted the Mona Lisa?', 'Van Gogh', 'Da Vinci', 'Picasso', 'Rembrandt', 2),"
            "('What is the fastest land animal?', 'Lion', 'Horse', 'Cheetah', 'Tiger', 3),"
            "('How many continents are there?', '5', '6', '8', '7', 4),"
            "('What gas do plants absorb?', 'Oxygen', 'Nitrogen', 'CO2', 'Hydrogen', 3);";
        char* errMsg3 = nullptr;
        sqlite3_exec(_db, insertQuestions, nullptr, nullptr, &errMsg3);
    }


    const char* createStats =
        "CREATE TABLE IF NOT EXISTS statistics ("
        "username TEXT NOT NULL,"
        "avgAnswerTime REAL,"
        "correctAnswers INTEGER,"
        "totalAnswers INTEGER,"
        "numGames INTEGER,"
        "highScore REAL,"
        "FOREIGN KEY(username) REFERENCES users(username)"
        ");";
    executeSQLCommand(createStats);
    return true;
}

/**
 * Closes the active connection to the SQLite database if it is currently open
 * @return true after successfully closing the connection
 */
bool SqliteDatabase::close()
{
    if (_db)
    {
        sqlite3_close(_db);
        _db = nullptr;
    }
    return true;
}

/**
 * Checks if a specific user exists in the database by searching for their username
 * @param username the username to search for in the database
 * @return 1 if the user exists 0 if the user does not exist or -1 if an error occurred
 */
int SqliteDatabase::doesUserExist(const std::string& username)
{
    int result = 0;
    std::string query =
        "SELECT COUNT(*) FROM users WHERE username = '" + username + "';";
    executeSQLCommand(query.c_str(), Callback::intCallback, &result);
    return result;
}

/**
 * Checks if the provided password matches the password stored in the database for the given user
 * @param username the username of the user
 * @param password the password to verify
 * @return 1 if the password matches 0 if it does not match or -1 if an error occurred
 */
int SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
    int result = 0;
    std::string query =
        "SELECT COUNT(*) FROM users WHERE username = '" + username +
        "' AND password = '" + password + "';";
    executeSQLCommand(query.c_str(), Callback::intCallback, &result);
    return result;
}

/**
 * Adds a new user to the database with their chosen username password and email and initializes their statistics
 * @param username the username of the new user
 * @param password the password of the new user
 * @param email the email address of the new user
 * @return 1 if the user was added successfully 0 if an error occurred
 */
int SqliteDatabase::addNewUser(const std::string& username,
    const std::string& password,
    const std::string& email)
{
    std::vector<std::string> queries = {
        "INSERT INTO users (username, password, email) VALUES ('"
        + username + "', '" + password + "', '" + email + "');",
        "INSERT INTO statistics(username, avgAnswerTime, correctAnswers, totalAnswers, numGames, highScore) VALUES('"
        + username + "', 0, 0, 0, 0, 0);"
    };
    for (const std::string& query : queries)
        executeSQLCommand(query.c_str());
    return 1;
}

/**
 * Retrieves the average answer time of a specific player from the statistics table
 * @param username the username of the player
 * @return the average answer time of the player
 */
float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
    std::string query =
        "SELECT avgAnswerTime FROM statistics WHERE username = '" + username + "';";
    int result = 0;
    char* errMsg = nullptr;
    executeSQLCommand(query.c_str(), Callback::intCallback, &result);
    return static_cast<float>(result);
}

/**
 * Retrieves the total number of correct answers for a specific player from the statistics table
 * @param username the username of the player
 * @return the total number of correct answers
 */
int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
    std::string query =
        "SELECT correctAnswers FROM statistics WHERE username = '" + username + "';";
    int result = 0;
    char* errMsg = nullptr;
    executeSQLCommand(query.c_str(), Callback::intCallback, &result);
    return result;
}

/**
 * Retrieves the total number of answers submitted by a specific player from the statistics table
 * @param username the username of the player
 * @return the total number of answers
 */
int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
    std::string query =
        "SELECT totalAnswers FROM statistics WHERE username = '" + username + "';";
    int result = 0;
    char* errMsg = nullptr;
    executeSQLCommand(query.c_str(), Callback::intCallback, &result);
    return result;
}

/**
 * Retrieves the total number of games played by a specific player from the statistics table
 * @param username the username of the player
 * @return the total number of games played
 */
int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
    std::string query =
        "SELECT numGames FROM statistics WHERE username = '" + username + "';";
    int result = 0;
    char* errMsg = nullptr;
    executeSQLCommand(query.c_str(), Callback::intCallback, &result);
    return result;
}

/**
 * Retrieves the high score of a specific player from the statistics table
 * @param username the username of the player
 * @return the high score of the player
 */
float SqliteDatabase::getPlayerScore(const std::string& username)
{
    float playerScore = 0;
    char* errMsg = nullptr;
    std::string query = "SELECT highScore FROM statistics WHERE username = '" + username + "';";
    executeSQLCommand(query.c_str(), Callback::floatCallback, &playerScore);
    return playerScore;
}

/**
 * Retrieves the top 5 high scores from the statistics table
 * @return a vector of strings containing the usernames and their high scores
 */
std::vector<std::string> SqliteDatabase::getHighScores()
{
    std::vector<std::string> highScores;
    std::string query = "SELECT username, highScore FROM statistics ORDER BY highScore DESC LIMIT 5";
    executeSQLCommand(query.c_str(), Callback::highScoreCallback, &highScores);
    return highScores;
}

/**
 * Retrieves a specific question from the database by its ID
 * @param id the ID of the question to fetch
 * @return the populated question object
 */
Question SqliteDatabase::getQuestion(int id)
{
    Question q;
    std::string query = "SELECT * FROM questions WHERE id = " + std::to_string(id) + ";";
    executeSQLCommand(query.c_str(), Callback::questionCallback, &q);
    return q;
}

/**
 * Retrieves a specified amount of questions from the database
 *  The amount of questions is capped at 10
 * @param amountOfQuestions the amount of questions to fetch
 * @return a vector containing the requested amount of question objects
 */
std::vector<Question> SqliteDatabase::getQuestions(int amountOfQuestions)
{
    if (amountOfQuestions > QUESTIONS_IN_DB)
        amountOfQuestions = QUESTIONS_IN_DB;
    std::vector<Question> questions;
    std::string query = "SELECT * FROM questions LIMIT" + std::to_string(amountOfQuestions) + ";";
    executeSQLCommand(query.c_str(), Callback::questionsCallback, &questions);
    return questions;
}


/**
* Executes a basic SQL command without any callback function by delegating to the overloaded executeSQLCommand function.
* @param command the command to execute
*/
void SqliteDatabase::executeSQLCommand(const std::string& command)
{
    executeSQLCommand(command, nullptr, nullptr);
}

/**
 * Executes an SQL command with a specified callback function by delegating to the primary executeSQLCommand function with null data.
 * @param command the command to execute
 * @param callbackFunction the callback function to invoke for each row in the result
 */
void SqliteDatabase::executeSQLCommand(const std::string& command, int(*callbackFunction)(void*, int, char**, char**))
{
    executeSQLCommand(command, callbackFunction, nullptr);
}

/**
 * Executes an SQL command using the SQLite library handling any potential errors and throwing an exception if the execution fails.
 *  Throws a MyException containing the SQLite error message if the query fails.
 * @param command the command to execute
 * @param callbackFunction the callback function to invoke for each row in the result
 * @param data the pointer to the data that will be passed into the callback function
 */
void SqliteDatabase::executeSQLCommand(const std::string& command, int(*callbackFunction)(void*, int, char**, char**), void* data)
{
    char* errMsgBuffer = nullptr;
    std::string errMsg = "";
    if (sqlite3_exec(_db, command.c_str(), callbackFunction, data, &errMsgBuffer) != SQLITE_OK)
    {
        errMsg = errMsgBuffer;
        sqlite3_free(errMsgBuffer);
        throw std::runtime_error(errMsg);
    }
}