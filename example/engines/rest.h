#ifndef REST_HEADER
#define REST_HEADER
#pragma once

#include "interface.h"
#include "../network/url.h"

#include <boost/property_tree/ptree.hpp>

namespace example
{
	namespace engine
	{
		using network::Url;
		using namespace boost::property_tree;

		class Rest
			: public Interface
		{
			const string Devices = "/api/devices";
			const string States = "/api/refreshStates";

		public:
			Rest( const string & endpoint, const string & login, const string & password );

			virtual void getDevices( Device::Map & result, set<Device::Type> types = {} ) override;
			virtual RefreshResult refreshStates( Device::Map & devices ) override;

		private:
			bool Execute( const Url & target, ptree & result );

		private:
			Url endpoint;
			string login;
			string password;

			unsigned int last;
		};
	}
}

#endif