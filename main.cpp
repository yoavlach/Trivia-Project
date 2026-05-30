#pragma comment (lib, "ws2_32.lib")
#include "Server.h"
#include "WSAInitializer.h"
#include "RequestHandlerFactory.h"
#include "SqliteDatabase.h"
#include "Helper.h"
#include <iostream>
#include <fstream>
#include <thread>

#define EXIT_COMMAND "EXIT"

void serverCmd();

int main()
{
	try
	{

		TRACE("Starting...");
		WSAInitializer wsa_init;
		SqliteDatabase* sqliteDatabase = new SqliteDatabase();
		RequestHandlerFactory requestHandlerFactory(sqliteDatabase);
		Server server(requestHandlerFactory);
		std::thread serverThread(&Server::run, &server);
		std::thread cmdThread(serverCmd);
		serverThread.detach();
		cmdThread.join();
		delete sqliteDatabase;
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception was thrown in function: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception in main !" << std::endl;
	}
}

/*
* Waits for a command input until the user enters "EXIT" and prints an error
* message in case he entered an unrecognized command
* input: none
* output: none
*/
void serverCmd()
{
	std::string command = "";
	do
	{
		std::cin >> command;
		if (command != EXIT_COMMAND)
			std::cout << "Unrecognized command. Please try again" << std::endl;
	} while (command != EXIT_COMMAND);
	std::cout << "Goodbye!" << std::endl;
}