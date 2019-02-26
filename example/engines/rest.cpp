#include "rest.h"
#include "../network/http.h"

#include <boost/beast/core/detail/base64.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>


namespace example
{
	namespace engine
	{
		using network::HttpClient;

		Rest::Rest( const string & host, const string & user, const string & secret )
			: endpoint( host ), login( user ), password( secret )
			, last( 0 )
		{}

		void Rest::getDevices( Device::Map & result, set<Device::Type> types )
		{
			Url target = endpoint;
			target.path = Devices;

			ptree data;
			if( Execute( target, data ) )
			{
				for( auto & entry : data )
				{
					assert( entry.first.empty() );

					try
					{
						auto & current = entry.second;
						
						string type = current.get<string>( "type" );
						if( types.empty() || types.count( type ) )
						{
							Device::Id id{ current.get<unsigned int>( "id" ) };

							auto & device = result.emplace( id, id ).first->second;
							device.type = std::move( type );
							device.name = current.get<string>( "name" );
							device.value = current.get<string>( "properties.value" );
						}
					}
					catch( const std::exception & error )
					{
						//log error
					}
				}
			}
		}

		Interface::RefreshResult Rest::refreshStates( Device::Map & devices )
		{
			Url target = endpoint;
			target.path = States;

			if( last )
				target.query = "?last=" + std::to_string( last );

			ptree data;
			if( Execute( target, data ) )
			{
				last = data.get<unsigned int>( "last" );

				auto changes = data.get_child_optional( "changes" );
				if( !changes )
					return { true, false };

				bool updated = false;
				
				for( auto & change : changes.value() )
				{
					try
					{
						auto & current = change.second;

						Device::Id id{ current.get<unsigned int>( "id" ) };

						auto found = devices.find( id );

						if( found != devices.end() )
							found->second.value.swap( current.get<string>( "value" ) ), updated = true;
					}
					catch( const std::exception & error )
					{
						//log error
					}
				}
				return { true, updated };
			}
			return { false,false };
		}

		bool Rest::Execute( const Url & target, ptree & result )
		{
			try
			{
				HttpClient client;

				namespace utils = boost::beast::detail;

				HttpClient::Request header = HttpClient::Request::Basic();
				header["Authorization"] = "Basic " + utils::base64_encode( login + ":" + password );

				auto response = client.Get( target, header );

				std::stringstream stream;
				stream << response.data;

				read_json( stream, result );

				return true;
			}
			catch( const HttpClient::NotAuthenticated & error )
			{
				throw NotAuthenticated();
			}
			catch( const std::exception & error )
			{
				cerr << error.what() << endl;
			}
			return false;
		}
	}
}
