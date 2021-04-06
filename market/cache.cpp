#include "cache.h"

#include <set>

namespace interview
{
	Cache::Cache() = default;
	Cache::Cache( const Cache& ) = default;

	Cache::Cache( const Data & data )
	{
		for( const auto& entry : data )
			Add( entry.second );
	}

	Cache::Cache( const list< Order > & data )
	{
		for( const auto& entry : data )
			Add( entry );
	}


	bool Cache::Add( const Order & order )
	{
		auto created = orders.byId.emplace( order.id, order );
		if( created.second )
		{
			auto& place = created.first;
			auto& added = place->second;
			
			auto & user = orders.byUser[added.user];
			user.emplace( added.id, added );

			auto & security = orders.bySecuritiesId[added.security];
			security[added.side].emplace( added.id, added );

			return true;
		}
		return false;
	}

	bool Cache::CancelOrderById( const string& id )
	{
		auto found = orders.byId.find( id );

		if( found == orders.byId.end() )
			return false;

		auto& order = found->second;

		orders.byUser.find( order.user )->second.erase( order.id );

		auto security = orders.bySecuritiesId.find( order.security );
		auto & types = security->second;

		types.find( order.side )->second.erase( order.id );

		orders.byId.erase( found );

		return false;
	}

	bool Cache::CancelOrderByUser( const string& user )
	{
		auto& user_orders = orders.byUser[user];

		auto processed = move( user_orders );

		for( auto& order : processed )
			CancelOrderById( order.second.id );

		return true;
	}

	bool Cache::CancelOrderBySecuritiesId( const string& id )
	{
		auto& securities_orders = orders.bySecuritiesId[id];

		for( auto& type : securities_orders )
		{
			auto processed = move( type.second );
			
			for( auto& order : processed )
				CancelOrderById( order.second.id );
		}

		return false;
	}

	int Cache::Match( const string& security )
	{
		auto& securities_orders_by_type = orders.bySecuritiesId[security];

		int result = 0;

		map<Order*, int> used_and_left;

		if( securities_orders_by_type.size() != 2 )
			return 0;

		auto & first_type = *securities_orders_by_type.begin();
		auto & second_type = *securities_orders_by_type.rbegin();

		for( const auto& first_type_order : first_type.second )
		{
			auto& first_order = first_type_order.second;

			for( const auto& second_type_order : second_type.second )
			{
				auto& second_order = second_type_order.second;

				if( first_order.company == second_order.company )
					continue;

				auto & first_quantity = used_and_left.emplace( &first_order, first_order.quantity ).first->second;
				auto & second_quantity = used_and_left.emplace( &second_order, second_order.quantity ).first->second;

				if( first_quantity > second_quantity )
				{
					result += second_quantity;
					first_quantity -= second_quantity;
					second_quantity = 0;
				}
				else
				{
					result += first_quantity;
					second_quantity -= first_quantity;
					first_quantity = 0;
				}
			}
		}

		return result;
	}

	unsigned int Cache::Size()
	{
		return orders.byId.size();
	}
}