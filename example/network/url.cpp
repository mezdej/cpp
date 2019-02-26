#include "url.h"

#include <regex>

namespace network
{
	using std::string;

	Url::Url( const string & url )
	{
		static const std::regex format( R"((?:(\w+):\/\/)?([\w_\-.]+)(?::(\d+)?)?((?:\/\w*)*)(\??[\w=#]+)?)",
			std::regex_constants::ECMAScript | std::regex_constants::icase );

		std::smatch match;
		if( std::regex_match( url, match, format ) )
		{
			protocol = match[1];
			domain = match[2];
			port = match[3];
			path = match[4];
			query = match[5];
		}
	}

	string Url::absolutePath() const
	{
		return path + query;
	}
}