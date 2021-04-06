#include "test.h"

#include <iostream>
#include <stdexcept>

#define DO_CHECK( x ) if( !(x) ) throw std::runtime_error( #x )

namespace interview
{
	// normally I would use some unittest framework
	bool Test::Run()
	{
		try
		{
			AddOne();
			RemoveOne();
			SimpleMatch();
			SimpleMatchSameCompany();
			ComplexMatch();
			return true;
		}
		catch( const exception& error )
		{
			cout << error.what();
		}
		return false;
	}

	void Test::AddOne()
	{
		Cache cache;
		DO_CHECK( cache.Size() == 0 );
		cache.Add( { "OrdId9", "US5422358DA3", "Buy", 400, "User9", "CompanyE" } );
		DO_CHECK( cache.Size() == 1 );
	}

	void Test::RemoveOne()
	{
		Cache cache( Order::Examples() );

		cache.CancelOrderById( "OrdId5" );
		DO_CHECK( cache.Size() == 7 );

		cache.CancelOrderById( "OrdId1" );
		DO_CHECK( cache.Size() == 6 );

		cache.CancelOrderById( "OrdId8" );
		DO_CHECK( cache.Size() == 5 );
	}

	void Test::RemoveByUser()
	{
		Cache cache( Order::Examples() );

		cache.CancelOrderByUser( "User5" );
		DO_CHECK( cache.Size() == 7 );

		cache.CancelOrderByUser( "User1" );
		DO_CHECK( cache.Size() == 6 );

		cache.CancelOrderByUser( "User8" );
		DO_CHECK( cache.Size() == 5 );
	}

	void Test::RemoveByScuritiesID()
	{
		Cache cache( Order::Examples() );

		cache.CancelOrderBySecuritiesId( "US5422358DA3" );
		DO_CHECK( cache.Size() == 3 );

		cache.CancelOrderBySecuritiesId( "US9128473801" );
		DO_CHECK( cache.Size() == 1 );

		cache.CancelOrderBySecuritiesId( "US19635GY645" );
		DO_CHECK( cache.Size() == 0 );
	}

	void Test::SimpleMatch()
	{
		Cache cache;

		cache.Add( { "OrdId1", "SimpleMatch", "Buy", 1000, "User1", "CompanyA" } );
		cache.Add( { "OrdId2", "SimpleMatch", "Sell", 1000, "User2", "CompanyB" } );

		DO_CHECK( cache.Match("SimpleMatch") == 1000 );

	}

	void Test::SimpleMatchSameCompany()
	{
		Cache cache;

		cache.Add( { "OrdId1", "SimpleMatchSameCompany", "Buy", 1000, "User1", "CompanyA" } );
		cache.Add( { "OrdId2", "SimpleMatchSameCompany", "Sell", 1000, "User2", "CompanyA" } );

		DO_CHECK( cache.Match( "SimpleMatchSameCompany" ) == 0 );
	}

	void Test::ComplexMatch()
	{
		Cache cache( Order::Examples() );
		DO_CHECK( cache.Match( "US5422358DA3" ) == 2700 );
	}
};