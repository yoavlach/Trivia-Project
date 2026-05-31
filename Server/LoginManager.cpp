#include "LoginManager.h"
#include "messageCodes.h"

/**
 * Initializes the login manager with a database pointer
 *  Sets the database member variable to interact with user data
 * @param database the pointer to the database interface
 */
LoginManager::LoginManager(IDataBase* database) : m_database(database)
{
}

/**
 * Signs up a new user to the system and logs them in
 *  Checks if the user already exists in the database before creating a new entry
 * @param username the requested username for the new account
 * @param password the requested password for the new account
 * @param email the email address of the new user
 * @return a struct containing the success or error status code and the username
 */
SuccessfulLoginInfo LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    unsigned int response = SUCCESS;
    if (m_database->doesUserExist(username))
        response = USER_ALREADY_EXISTS;
    else
    {
        m_database->addNewUser(username, password, email);
        m_loggedUsers.push_back(LoggedUser{ username });
    }
    return SuccessfulLoginInfo{ response, username };
}

/**
 * Logs an existing user into the system
 *  Verifies that the user exists and that the provided password matches the database record
 * @param username the username of the user trying to log in
 * @param password the password provided for authentication
 * @return a struct containing the success or error status code and the username
 */
SuccessfulLoginInfo LoginManager::login(const std::string& username, const std::string& password)
{
    unsigned int response = SUCCESS;
    if (isLoggedIn(username))
        response = USER_ALREADY_LOGGED_IN;
    else if (m_database->doesUserExist(username))
    {
        if (m_database->doesPasswordMatch(username, password))
            m_loggedUsers.push_back(LoggedUser{ username });
        else
            response = INCORRECT_PASSWORD;
    }
    else
        response = USER_DOES_NOT_EXIST;
    return SuccessfulLoginInfo{ response, username };
}

/**
 * Logs a user out of the system by removing them from the active users list
 *  Iterates through the list of currently logged in users to find and erase the specific user
 * @param username the username of the user to log out
 */
void LoginManager::logout(const std::string& username)
{
    bool foundUser = false;
    for (int i = 0; i < m_loggedUsers.size() && !foundUser; i++)
    {
        foundUser = m_loggedUsers[i].getUsername() == username;
        if (foundUser)
            m_loggedUsers.erase(m_loggedUsers.begin() + i);
    }
}

bool LoginManager::isLoggedIn(const std::string& username)
{
    bool found = false;
    for (int i = 0; i < m_loggedUsers.size() && !found; i++)
        found = m_loggedUsers[i].getUsername() == username;
    return found;
}
