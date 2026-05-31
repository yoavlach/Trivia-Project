#pragma once

#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <deque>
#include <queue>
#include <map>
#include <mutex>
#include <condition_variable>
#include <WinSock2.h>
#include "RecvMessage.h"

class Communicator
{
public:
	Communicator(RequestHandlerFactory& handlerFactory);
	~Communicator();
	void startHandleRequests();


private:
	void bindAndListen();
	void acceptClient();
	void handleNewClient(const SOCKET client_socket);

	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
};

