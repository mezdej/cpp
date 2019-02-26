#include "http.h"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/format.hpp>

namespace network
{
	HttpClient::Request HttpClient::Request::Basic()
	{
		Request request;
		request["Connection"] = "close";
		request["Accept"] = "text/html";
		request["Accept-Encoding"] = "text/html";
		request["Accept-Language"] = "pl-PL,pl;q=0.8,en-US;q=0.6,en;q=0.4";
		request["User-Agent"] = "HTTP-Client/1.0";
		return request;
	}

	HttpClient::Request HttpClient::Request::Default = HttpClient::Request::Basic();

	HttpClient::Response::Response()
		: version(), status( 0 )
	{}

	HttpClient::Response::Response( Response && response )
		: Header( std::move( response ) )
		, data( std::move( response.data ) )
		, version( response.version ), status( response.status )
	{}

	HttpClient::Response::Response( const Response & response )
		: Header( response )
		, data( response.data )
		, version( response.version ), status( response.status )
	{}

	HttpClient::Response & HttpClient::Response::operator = ( Response && move )
	{
		*(Header*) this = move;
		data = std::move( move.data );
		return *this;
	}

	HttpClient::Response HttpClient::Get( const Url & endpoint )
	{
		return Get( endpoint, Request::Default, boost::none );
	}

	HttpClient::Response HttpClient::Post( const Url & endpoint )
	{
		return Post( endpoint, Request::Default, boost::none );
	}

	HttpClient::Response HttpClient::Get( const Url & endpoint, const Header & header, optional<const Data &> data )
	{
		return Execute( Method::Get, endpoint, header, data );
	}

	HttpClient::Response HttpClient::Post( const Url & endpoint, const Header & header, optional<const Data &> data )
	{
		return Execute( Method::Post, endpoint, header, data );
	}

	const string & MethodString( HttpClient::Method method )
	{
		using Method = HttpClient::Method;
		static std::map<HttpClient::Method, string> names =
		{
			{ Method::Get,     "GET" },
			{ Method::Head,    "HEAD" },
			{ Method::Post,    "POST" },
			{ Method::Put,     "PUT" },
			{ Method::Delete,  "DELETE" },
			{ Method::Options, "OPTIONS" },
			{ Method::Trace,   "TRACE" },
			{ Method::Connect, "CONNECT" }
		};

		return names[method];
	}

	HttpClient::Response HttpClient::Execute( Method method, Url endpoint, const Header & header, optional<const Data &> data )
	{
		namespace asio = boost::asio;
		using tcp = boost::asio::ip::tcp;
		using error_code = boost::system::error_code;
		using format = boost::format;

		if( endpoint.domain.empty() )
			return Response();

		if( endpoint.port.empty() )
		{
			if( endpoint.protocol == "https" )
				endpoint.port = "443";
			else
				endpoint.port = "80";
		}
		if( endpoint.path.empty() )
			endpoint.path = "/";

		asio::io_context context;

		tcp::resolver resolver{ context };
		tcp::socket socket{ context };

		auto const resolved = resolver.resolve( endpoint.domain, endpoint.port );

		error_code error = asio::error::host_not_found;
		auto result = asio::connect( socket, resolved.begin(), resolved.end() );

		if( result == resolved.end() )
			throw std::runtime_error( "Failed to connect!" );

		auto first = format( "%1% %2% HTTP/1.0\r\n" ) % MethodString( method ) % endpoint.absolutePath();
		auto host = format( "Host: %1%\r\n" ) % endpoint.domain;

		auto sendall = [&socket] ( const string & data )
		{
			asio::write( socket, asio::buffer( data, data.length() ), asio::transfer_all() );
		};
		
		sendall( first.str() );
		sendall( host.str() );

		for( auto & entry : header )
		{
			sendall( entry.first );
			sendall( ": " );
			sendall( entry.second );
			sendall( "\r\n" );
		}

		if( data )
		{
			if( header.find( "Content-Length" ) == header.end() )
			{
				auto content = format( "Content-Length: %1%\r\n" ) % data->size();
				sendall( content.str() );
			}
		}
		sendall( "\r\n" );

		if( data && data->empty() == false )
		{
			asio::write( socket, asio::buffer( data.value() ), asio::transfer_all() );
		}

		Response response;
		asio::streambuf received;
		std::string line;

		do
		{
			int readed = asio::read_until( socket, received, "\r\n" );

			if( !response.status )
			{
				std::istream response_stream( &received );

				response_stream >> response.version;
				response_stream >> response.status;
				std::getline( response_stream, response.message );
				response.message.pop_back();
			}
			else
			{
				std::istream response_stream( &received );
				std::getline( response_stream, line );
				line.pop_back();//skip \r

				if( line.empty() )
					break;

				auto found = line.find( ':' );

				if( found != string::npos )
					response[line.substr( 0, found )] = line.substr( found + 1 );
			}
		} while( true );


		bool has_length = false;
		int content = 0;

		auto length = response.find( "Content-Length" );

		if( length != response.end() )
		{
			content = std::stoi( length->second );
			if( content > 0 )
			{
				response.data.resize( content + 1 );
				response.data[content] = 0;
			}
		}

		if( content > 0 )
			asio::read( socket, received, asio::transfer_exactly( content ), error );
		else
		{
			while( asio::read( socket, received, asio::transfer_at_least( 1 ), error ) );
		}

		if( error != asio::error::eof )
			throw boost::system::system_error( error );


		std::istream rest( &received );
		if( content > 0 )
			rest.read(  &response.data[0], content );
		else
			response.data = string( std::istreambuf_iterator<char>( rest ), {} );

		socket.shutdown( socket.shutdown_both );

		if( response.status == 401 )
			throw NotAuthenticated();

		return response;
	}

}
