#ifndef URL_HEADER
#define URL_HEADER
#pragma once

#include <string>

namespace network
{
	using std::string;

	class Url
	{
	public:
		Url( const string & );

		string absolutePath() const;

	public:
		string protocol;
		string domain;
		string port;
		string path;
		string query;
	};
}

#endif