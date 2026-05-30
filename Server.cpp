#include "Server.h"
#include "SqliteDatabase.h"

/**
 * Initializes the server object and its member variables
 *  It allocates a new SqliteDatabase instance and sets the communicator and handler factory
 * @param handlerFactory the factory responsible for creating request handlers
 */
Server::Server(RequestHandlerFactory& handlerFactory)
	: m_communicator(handlerFactory), m_database(handlerFactory.getDatabase()), m_handlerFactory(handlerFactory)
{
}

/**
 * Starts the server to begin listening and handling client requests
 *  It delegates the network listening process by calling the startHandleRequests method on the communicator
 */
void Server::run()
{
	m_communicator.startHandleRequests();
}