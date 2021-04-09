#pragma once
#include "Instance.h"
#include "Solution.h"

namespace validator {
	class Write
	{
	public:
		static void writePackingPlan(const std::string path, Solution& solution, Instance& instance, const int constraintNo, const int seed);
	};

}

