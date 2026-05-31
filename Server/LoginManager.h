#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "IDataBase.h"
#include "LoggedUser.h"
#include "Responses.h"
#include "Requests.h"

class LoginManager
{
public:
	LoginManager(IDataBase* database);
	SuccessfulLoginInfo signup(const std::string& username, const std::string& password, const std::string& email);
	SuccessfulLoginInfo login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
	bool isLoggedIn(const std::string& username);
private:
	IDataBase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
};

