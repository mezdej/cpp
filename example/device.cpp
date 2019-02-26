#include "device.h"

#include <sstream>
#include <iomanip> 

namespace example
{
	Device::Device( Id number )
		: id( number )
	{}

	Device::operator string() const
	{
		ostringstream stream;

		stream << setw( 4 ) << id << " " << setw( 25 ) << name << " : " << value;

		return stream.str();
	}
}