#pragma once
#include "IDataBase.h"
#include <iostream>
#include <vector>

class StatisticsManager
{
public:
	StatisticsManager(IDataBase* database);
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(const std::string& username);
private:
	IDataBase* m_database;
};