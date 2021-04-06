#pragma once
#include <string>
#include <list>

namespace interview
{
	using namespace std;

	class Order
	{
	public:
		string id;//Order ID( string )     // e.g  "ID1" (unique id for order)
		string security;//Security ID( string )  // e.g. "US9128473801"
		string side;//Side( string )         // e.g. “Buy” or “Sell” 
		int  quantity;//Quantity( int )        // e.g. 120000
		string user;//User ID( string )      // e.g. “John Smith” 
		string company;//Company Name( string ) // e.g. “Hedge Fund A” (company of User ID)

		static list<Order> Examples();
	};

	bool operator < ( const Order& first, const Order& second );
}
