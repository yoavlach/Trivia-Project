#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

/**
 * Initializes the request handler factory
 *  It sets the database pointer and initializes the login manager with the provided database
 * @param database the database pointer to be used by the factory
 */
RequestHandlerFactory::RequestHandlerFactory(IDataBase* database) : m_database(database), m_loginManager(database), m_statisticsManager(database)
{
}

/**
 * Creates a new login request handler
 *  It allocates memory for a new LoginRequestHandler object and passes a reference to this factory to it
 * @return a pointer to the newly created login request handler
 */
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

/**
 * Retrieves the login manager
 *  It returns a reference to the internal login manager object stored in the factory
 * @return a reference to the login manager
 */
LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

/**
 * Creates a new menu request handler and allocates memory for a new MenuRequestHandler object on the heap
 * @param loggedUser the user who is currently logged in
 * @return a pointer to the newly created menu request handler
 */
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& loggedUser)
{
	return new MenuRequestHandler(loggedUser, *this);
}

/**
 * Retrieves the database
 * @return a pointer to the database
 */
IDataBase* RequestHandlerFactory::getDatabase()
{
	return m_database;
}

/**
 * Retrieves the statistics manager
 * @return a reference to the statistics manager
 */
StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return m_statisticsManager;
}

/**
 * Retrieves the room manager
 * @return a reference to the room manager
 */
RoomManager& RequestHandlerFactory::getRoomManager()
{
	return m_roomManager;
}