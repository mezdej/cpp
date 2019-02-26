#ifndef INTERFACE_MOCK_HEADER
#define INTERFACE_MOCK_HEADER
#pragma once

#include "interface.h"

namespace example
{
	namespace engine
	{
		class Mock
			: public Interface
		{
		public:
			Mock( const string & endpoint, const string & login, const string & password );

			virtual void getDevices( Device::Map & result, set<Device::Type> types = {} ) override;
			virtual RefreshResult refreshStates( Device::Map & devices ) override;

		private:
			string endpoint;
			string login;
			string password;
		};
	}
}

#endif