#pragma once
#include "Communicator.h"
#include "IDataBase.h"
#include "RequestHandlerFactory.h"

class Server
{
public:
	Server(RequestHandlerFactory& handlerFactory);
	void run();
private:
	Communicator m_communicator;
	IDataBase* m_database;
	RequestHandlerFactory m_handlerFactory;
};

