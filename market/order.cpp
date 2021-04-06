#include "order.h"


namespace interview
{
	list<Order> Order::Examples()
	{
		return {
			{"OrdId1", "US9128473801", "Buy", 1000, "User1", "CompanyA"},
			{"OrdId2", "US5422358DA3", "Sell", 3000, "User2", "CompanyB"},
			{"OrdId3", "US9128473801", "Sell", 500, "User3", "CompanyA"},
			{"OrdId4", "US5422358DA3", "Buy", 600, "User4", "CompanyC"},
			{"OrdId5", "US5422358DA3", "Buy", 100, "User5", "CompanyB"},
			{"OrdId6", "US19635GY645", "Buy", 1000, "User6", "CompanyD"},
			{"OrdId7", "US5422358DA3", "Buy", 2000, "User7", "CompanyE"},
			{"OrdId8", "US5422358DA3", "Sell", 5000, "User8", "CompanyE"}
		};
	}

	bool operator < ( const Order& first, const Order& second )
	{
		return first.id < second.id;
	}
}