#ifndef INTERFACE_HEADER
#define INTERFACE_HEADER
#pragma once

#include <string>
#include <memory>
#include <set>

#include "../device.h"

using namespace std;

namespace example
{
	namespace engine
	{
		class Interface
		{
		public:
			using Pointer = unique_ptr<Interface>;

			static Pointer Get( const string & endpoint, const string & login, const string & password );

			virtual ~Interface() = default;

			virtual void getDevices( Device::Map & result, set<Device::Type> types = {} ) = 0;

			struct RefreshResult
			{
				bool refreshed = false;
				bool changes = false;
			};
			virtual RefreshResult refreshStates( Device::Map & devices ) = 0;

			class Exception
				: public std::runtime_error
			{
			public:
				using runtime_error::runtime_error;
			};

			class NotAuthenticated
				: public Exception
			{
			public:
				NotAuthenticated() : Exception( "Not authenticated" ) {}
			};
		};
	}
}


#endif