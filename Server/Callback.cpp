#include "Callback.h"
#include <iostream>
#include <vector>
#include <string>
#include "Question.h"

#define ID_INDEX 0
#define QUESTION_INDEX 1
#define ANSWER_START_INDEX 2
#define ANSWER_END_INDEX 5
#define CORRECT_ANS_INDEX 6
#define QUESTIONS_IN_DB 10
enum ANSWER_INDEXES { ANSWER_1, ANSWER_2, ANSWER_3, ANSWER_4 };

/**
 * Processes a row of questions from the database and adds the question to the vector of questions
 * @param data the pointer to the vector of Question objects
 * @param argc the number of columns in the result
 * @param argv the array of strings representing the row data
 * @param colName the array of strings representing column names
 * @return 0 to indicate success
 */
int Callback::questionsCallback(void* data, int argc, char** argv, char** colName)
{
    std::vector<Question>* questions = nullptr;
    if (data)
    {
        questions = (std::vector<Question>*) data;
        Question question;
        for (int i = 0; i < argc; i++)
        {
            if (!strcmp(colName[i], "question"))
                question.question = argv[i];
            else if (!strcmp(colName[i], "correct_ans"))
                question.correctAns = std::stoi(argv[i]);

            else if (!strcmp(colName[i], "answer1"))
                question.answers[ANSWER_1] = argv[i];
            else if (!strcmp(colName[i], "answer2"))
                question.answers[ANSWER_2] = argv[i];
            else if (!strcmp(colName[i], "answer3"))
                question.answers[ANSWER_3] = argv[i];
            else if (!strcmp(colName[i], "answer4"))
                question.answers[ANSWER_4] = argv[i];

        }
    }
    return 0;
}

/**
 * Processes a single row from the questions table and populates a Question object
 * @param data the pointer to the Question object to be populated
 * @param argc the number of columns in the result
 * @param argv the array of strings representing the row data
 * @param colName the array of strings representing column names
 * @return 0 to indicate success
 */
int Callback::questionCallback(void* data, int argc, char** argv, char** colName)
{
    Question* q = static_cast<Question*>(data);
    q->id = std::stoi(argv[ID_INDEX]);
    q->question = argv[QUESTION_INDEX];
    for (int i = ANSWER_START_INDEX; i <= ANSWER_END_INDEX; i++)
        q->answers[i - ANSWER_START_INDEX] = argv[i];
    q->correctAns = std::stoi(argv[CORRECT_ANS_INDEX]);
    return 0;
}

/**
 * Retrieves a single integer result from a query and stores it in the given pointer
 *  It converts the first column of the first row into an integer
 * @param data the pointer to the integer where the result will be stored
 * @param argc the number of columns in the result
 * @param argv the array of strings representing the row data
 * @param colName the array of strings representing column names
 * @return 0 to indicate success
 */
int Callback::intCallback(void* data, int argc, char** argv, char** colName)
{
    int* result = static_cast<int*>(data);
    if (argc > 0 && argv[0])
        *result = std::stoi(argv[0]);
    return 0;
}

/**
 * Processes a high score row from the database and adds the username and score to a vector
 * @param data the pointer to the vector of strings storing the high scores
 * @param argc the number of columns in the result
 * @param argv the array of strings representing the row data
 * @param colName the array of strings representing column names
 * @return 0 to indicate success
 */
int Callback::highScoreCallback(void* data, int argc, char** argv, char** colName)
{
    std::vector<std::string>* highScores = nullptr;
    std::string username = "", highScore = "";
    if (data)
    {
        highScores = (std::vector<std::string>*) data;
        for (int i = 0; i < argc; i++)
        {
            if (!strcmp(colName[i], "username"))
                username = argv[i];
            else if (!strcmp(colName[i], "highScore"))
                highScore = argv[i];
        }
        highScores->push_back(username + ":" + highScore);
    }
    return 0;
}

/**
 * Retrieves a single float result from a query and stores it in the given pointer
 *  It converts the first column of the first row into an float
 * @param data the pointer to the float where the result will be stored
 * @param argc the number of columns in the result
 * @param argv the array of strings representing the row data
 * @param colName the array of strings representing column names
 * @return 0 to indicate success
 */
int Callback::floatCallback(void* data, int argc, char** argv, char** colName)
{
    float* result = static_cast<float*>(data);
    if (argc > 0 && argv[0])
        *result = std::stof(argv[0]);
    return 0;
}
