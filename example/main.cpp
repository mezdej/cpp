// example.cpp : Defines the entry point for the application.
//

#include "home.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <exception>

using namespace std;
namespace options = boost::program_options;

int main( int arguments_count, char* arguments[] )
{
	if( arguments_count < 3 )
	{
		cerr << "Usage: [app-name] <host> <port> login" << endl;
		return 1;
	}

	string endpoint = arguments[1];
	string login = arguments[2];
	string password;

	cout << "Enter password for " << login << " at " << endpoint << endl;
	cout << "\r";
	cin >> password;


	cout << "Possible commands:" << endl;
	cout << "- refresh" << endl;
	cout << "- quit" << endl;

	try
	{
		example::Home home( endpoint, login, password );

		string command;
		do
		{
			cout << endl << "> ";
			cin >> command;

			if( command == "quit" || command == "exit" )
				break;
			if( command == "refresh" )
				home.getDevices();

		} while( true );

	}
	catch( std::exception & error )
	{
		cerr << "Exception occurecd: " << error.what() << endl;
		return 1;
	}
	catch( ... )
	{
		cerr << "Unknown exception!" << endl;
		return 1;
	}
	return 0;
}