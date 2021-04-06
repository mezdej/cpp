#pragma once
#include "cache.h"

namespace interview
{
	// normally I would use some unittest framework
	struct Test
	{
		static bool Run();

		static void AddOne();
		static void RemoveOne();
		static void RemoveByUser();
		static void RemoveByScuritiesID();
		static void SimpleMatch();
		static void SimpleMatchSameCompany();
		static void ComplexMatch();
	};
}