#pragma once

#include "order.h"

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <initializer_list>

#include <mutex>

namespace interview
{
	// not thread safe
	// if required I would use conditional_variable so matching threads will get notification

	class Cache
	{
	public:
		using Data = unordered_map<string, Order>;

		Cache();
		Cache( const Cache& );
		Cache( const Data& );
		Cache( const list< Order > & );

		bool Add( const Order & order );

		bool CancelOrderById( const string& id );
		bool CancelOrderByUser( const string& user );
		bool CancelOrderBySecuritiesId( const string& id );

		int Match( const string& security );

		unsigned int Size();

	protected:

	private:
		using StringReferenceMap = unordered_map<string_view, Order&>;
		using UserMap = unordered_map< string, StringReferenceMap >;
		using TypeMap = map< string, StringReferenceMap >;
		using StringTypeMap = unordered_map< string, TypeMap >;

		struct
		{
			Data byId;
			UserMap byUser;
			StringTypeMap bySecuritiesId;
		}orders;
	};
}

