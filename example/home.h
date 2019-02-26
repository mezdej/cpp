#ifndef HOME_HEADER
#define HOME_HEADER
#pragma once

#include "engines/interface.h"

#include <thread>
#include <boost/asio.hpp>
#include <memory>

using namespace std;

namespace asio = boost::asio;

namespace example
{
	class Home
	{
	public:
		Home( const string & endpoint, const string & login, const string & password );
		~Home();

		void getDevices();
		void refreshStates();

	protected:
		void autoRefresh();
		void displayDevices();

	protected:
		string endpoint;
		string login;
		string secret;

		Device::Map devices;

	private:
		using Interface = engine::Interface;
		Interface::Pointer connection;
		asio::io_context context;
		asio::steady_timer timer;
		unique_ptr<asio::io_context::work> work;

		thread run;
	};
}

#endif // ! HOME_HEADER
