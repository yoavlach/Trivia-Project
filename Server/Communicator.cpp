#include "IRequestHandler.h"
#include "Communicator.h"
#include "Helper.h"
#include "SocketException.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "BinaryConverter.h"
#include "Responses.h"
#include "Requests.h"
#include <exception>
#include <iostream>
#include <string>
#include <numeric>
#include <ctime>
#include <sstream>

#define MSG_CODE_SIZE 8
#define DATA_LEN_PART_SIZE 32

// using static const instead of macros 
static const unsigned short PORT = 6767;
static const unsigned int IFACE = 0;

using std::string;
using std::mutex;
using std::unique_lock;
using std::vector;

/**
 * Initializes the Communicator object and creates a TCP listening socket
 *  Throws an exception if the socket creation fails
 * @param handlerFactory the factory used to create request handlers
 */
Communicator::Communicator(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
{
	m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

/**
 * Destroys the Communicator object and closes the listening socket
 *  Deletes all dynamically allocated request handlers for the clients
 */
Communicator::~Communicator()
{
	RecvMessage* currRecvMessage = nullptr;
	TRACE(__FUNCTION__ " closing accepting socket");
	// why is this try necessarily ?
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(m_serverSocket);

		for (auto i : m_clients)
			delete i.second;
	}
	catch (...) {}
}

/**
 * Starts the main server loop to continuously accept new client connections
 *  Calls bindAndListen before entering the infinite loop
 */
void Communicator::startHandleRequests()
{
	bindAndListen();

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		TRACE("accepting client...");
		acceptClient();
	}
}

/**
 * Binds the server socket to the specified port and sets it to listen for incoming connections
 *  Throws an exception if binding or listening fails
 */
void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;
	// again stepping out to the global namespace
	if (::bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	TRACE("binded");

	if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	TRACE("listening...");
}

/**
 * Accepts a new client connection and spawns a dedicated thread to handle it
 *  Creates a new login request handler for the client and detaches the thread
 */
void Communicator::acceptClient()
{
	SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	TRACE("Client accepted !");
	m_clients[client_socket] = m_handlerFactory.createLoginRequestHandler();
	// create new thread for client	and detach from it
	std::thread tr(&Communicator::handleNewClient, this, client_socket);
	tr.detach();

}

/**
 * Manages the communication loop for a specific connected client
 *  Continuously receives messages processes them through handlers and sends back the appropriate response
 * @param client_socket the socket descriptor associated with the connected client
 */
void Communicator::handleNewClient(const SOCKET client_socket)
{
	IRequestHandler* currHandler = m_handlerFactory.createLoginRequestHandler();
	RequestResult currRequestResult;
	Buffer respMessage;
	RequestInfo currRequestInfo;
	std::string connectedUsername = "";
	bool clientAuthenticated = false;
	ErrorResponse errResp;
	Buffer errRespBuffer;

	m_clients.find(client_socket)->second = currHandler;
	if (client_socket == INVALID_SOCKET)
		throw SocketException();
	try
	{
		while (true)
		{
			std::cout << "waiting" << std::endl;
			currRequestInfo.buffer = Helper::getMessage(client_socket);
			std::cout << "recieved" << std::endl;
			std::cout << currRequestInfo.buffer.size() << std::endl;
			for (char i : currRequestInfo.buffer)
				std::cout << i;
			std::cout << std::endl;
			currRequestInfo.receivalTime = time(nullptr);
			currRequestInfo.id = Helper::extractMessageCode(currRequestInfo.buffer);

			if (currHandler->isRequestRelevant(currRequestInfo))
			{
				try
				{
					currRequestResult = currHandler->handleRequest(currRequestInfo);
				}
				catch (const std::exception& e)
				{
					errResp.message = "Invalid message format";
					errRespBuffer = JsonResponsePacketSerializer::serializeResponse(errResp);
					Helper::sendData(client_socket, std::string(errRespBuffer.buffer.begin(), errRespBuffer.buffer.end()));
				}
				if (!clientAuthenticated)
				{
					clientAuthenticated = currRequestResult.successfulLoginInfo.success == SUCCESS;
					if (clientAuthenticated)
						connectedUsername = currRequestResult.successfulLoginInfo.username;
				}

				if (currRequestResult.newHandler)
				{
					delete m_clients.find(client_socket)->second;
					m_clients.find(client_socket)->second = currRequestResult.newHandler;
					currHandler = currRequestResult.newHandler;
				}

				respMessage = currRequestResult.response;
				Helper::sendData(client_socket, std::string(respMessage.buffer.begin(), respMessage.buffer.end()));
			}
			else
			{
				errResp.message = "Irrelevant message";
				errRespBuffer = JsonResponsePacketSerializer::serializeResponse(errResp);
				Helper::sendData(client_socket, std::string(errRespBuffer.buffer.begin(), errRespBuffer.buffer.end()));
			}
		}

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		auto clientPair = m_clients.find(client_socket);
		delete clientPair->second;
		m_clients.erase(clientPair);
		closesocket(client_socket);
		if (clientAuthenticated)
		{
			m_handlerFactory.getLoginManager().logout(connectedUsername);
			m_handlerFactory.createMenuRequestHandler(LoggedUser{ connectedUsername })->handleRequest(RequestInfo{ LOG_OUT });
		}
	}
}