// Interview.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "cache.h"
#include "test.h"

int main()
{
	using namespace interview;

	Cache cache( Order::Examples() );

	cout << "Match for \"US5422358DA3\": " << cache.Match( "US5422358DA3" ) << endl;

	Test::Run();
	
	system( "pause" );

}
