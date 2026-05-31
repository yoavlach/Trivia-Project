#pragma once
#include <iostream>
#define AMOUNT_OF_ANSWERS 4

struct Question {
    int id;
    std::string question;
    std::string answers[AMOUNT_OF_ANSWERS];
    int correctAns;
};
