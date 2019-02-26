#include "interface.h"

#include "../network/url.h"

#include "mock.h"
#include "rest.h"

#include <boost/beast/http/parser.hpp>

namespace example
{
	namespace engine
	{
		Interface::Pointer Interface::Get( const string & endpoint, const string & login, const string & password )
		{
			network::Url url( endpoint );
			if( url.protocol.empty() == false )
			{
				//todo: return interface by protocol
				if( url.protocol == "mock" )
					return Interface::Pointer( new Mock( endpoint, login, password ) );
			}
			return Interface::Pointer( new Rest( endpoint, login, password ) );
		}
	}
}