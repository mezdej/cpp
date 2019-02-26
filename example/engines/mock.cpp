#include "mock.h"

namespace example
{
	namespace engine
	{
		Mock::Mock( const string & host, const string & user, const string & secret )
			: endpoint( host ), login( user ), password( secret )
		{}

		void Mock::getDevices( Device::Map & result, set<Device::Type> types )
		{
			//TODO! mock implementation
		}

		Interface::RefreshResult Mock::refreshStates( Device::Map & devices )
		{
			//TODO! mock implementation
			return { false,false };
		}
	}
}