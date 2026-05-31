#include "StatisticsManager.h"

#define HIGH_SCORE_VECTOR_LEN 5

/**
 * Initializes the statistics manager with the provided database
 * @param database the database pointer to be used by the manager
 */
StatisticsManager::StatisticsManager(IDataBase* database) : m_database(database)
{
}

/**
 * Retrieves the top high scores from the database
 *  The number of high scores returned is limited by the maximum vector length
 * @return a vector of strings containing the top high scores
 */
std::vector<std::string> StatisticsManager::getHighScore()
{
    std::vector<std::string> highScores = m_database->getHighScores();
    return std::vector<std::string>(highScores.begin(), highScores.begin() + HIGH_SCORE_VECTOR_LEN);
}

/**
 * Retrieves the personal statistics of a specific user from the database
 * @param username the username of the user whose statistics are requested
 * @return a vector of strings containing the statistics of the user
 */
std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username)
{
    return {
        std::to_string(m_database->getPlayerAverageAnswerTime(username)),
        std::to_string(m_database->getNumOfCorrectAnswers(username)),
        std::to_string(m_database->getNumOfTotalAnswers(username)),
        std::to_string(m_database->getNumOfPlayerGames(username)),
        std::to_string(m_database->getPlayerScore(username))
    };
}