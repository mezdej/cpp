#include "home.h"
#include "engines/interface.h"

#include <iostream>
using namespace std;

namespace example
{
	Home::Home( const string & host, const string & user, const string & password )
		: endpoint( host ), login( user ), secret( password ), timer( context, 5s )
	{
		connection = Interface::Get( host, login, password );
		work = make_unique< asio::io_context::work >( context );

		getDevices();
		refreshStates();

		run = thread( [this] ()
		{
			context.run();
		} );
	}

	Home::~Home() try
	{
		work.reset();
		context.stop();
		run.join();
	}
	catch( ... )
	{
		cout << "Critical error: " __FUNCTION__ << endl;
	}

	void Home::getDevices()
	{
		context.post( [this] ()
		{
			try
			{
				cout << "Getting devices..." << endl;
				connection->getDevices( devices, { "com.example.temperatureSensor" } );

				displayDevices();
			}
			catch( const exception & error )
			{
				cerr << error.what() << endl;
			}
		} );
	}

	void Home::refreshStates()
	{
		context.post( [this] ()
		{
			try
			{
				if( connection->refreshStates( devices ).changes )
					displayDevices();
				refreshStates();
			}
			catch( const exception & error )
			{
				cerr << error.what() << endl;
			}
		} );
	}
	
	void Home::displayDevices()
	{
		cout << "Devices:" << endl;
		for( auto & device : devices )
			cout << (string) device.second << endl;

		cout << "Total devices:" << devices.size() << endl;
	}
}