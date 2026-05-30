#pragma once
#include <iostream>
#include "LoginManager.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDataBase* database);
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& loggedUser);
	IDataBase* getDatabase();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
private:
	LoginManager m_loginManager;
	IDataBase* m_database;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
};