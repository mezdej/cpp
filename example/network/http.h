#ifndef HTTP_HEADER
#define HTTP_HEADER
#pragma once

#include "url.h"

#include <exception>
#include <map>
#include <string>
#include <vector>

#include <boost/optional.hpp>

namespace network
{
	using std::string;
	using std::map;
	using std::vector;
	using boost::optional;

	class HttpClient
	{
	public:
		enum class Method
		{
			Get, Head, Post, Put, Delete, Options, Trace,Connect
		};

		using Data = vector<unsigned char>;
		using Header = map<string, string>;

		class Request
			: public Header
		{
		public:
			static Request Basic();
			static Request Default;
		};

		class Response
			: public Header
		{
		public:
			Response();
			Response( Response && move );
			Response( const Response & copy );

			Response & operator = ( Response && move );
		public:
			string version;
			unsigned short status = 0;
			string message;
			string data;
		};

		class NotAuthenticated
			: public std::runtime_error
		{
		public:
			NotAuthenticated() : std::runtime_error( "Not authenticated" ) {}
		};

		Response Get( const Url &  endpoint );
		Response Post( const Url &  endpoint );

		Response Get( const Url &  endpoint, const Header & header, optional<const Data &> data = boost::none );
		Response Post( const Url &  endpoint, const Header & header, optional<const Data &> data = boost::none );

	private:
		Response Execute( Method method, Url endpoint, const Header & header, optional<const Data &> data );
	};
}

#endif