// Interview.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "cache.h"
#include "test.h"

int main()
{
	using namespace interview;

	auto examples = Order::Examples();

	Cache cache( Order::Examples() );

	for( auto& entry : examples )
		cache.Add( entry );

	cout << "Match for \"US5422358DA3\": " << cache.Match( "US5422358DA3" ) << endl;

	Test::Run();
	
	system( "pause" );

}
