#ifndef DEVICE_HEADER
#define DEVICE_HEADER
#pragma once

#include <string>
#include <map>

using namespace std;

namespace example
{
	class Device
	{
	public:
		using Id = unsigned int;
		using Type = string;
		using Map = map< Id, Device >;

		Device( Id id );
		
		Device( const Device & ) = delete;
		Device & operator = ( const Device & ) = delete;

		operator string() const;
		
	public:
		const Id id;
		Type type;

		string name;
		string value;
	};
}

#endif // ! HOME_HEADER
