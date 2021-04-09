#pragma once
#include "Solution.h"
#include "Instance.h"
#include "ConstraintSet.h"

namespace validator {

	class ConstraintsRouting {
	public:
		static bool checkRoutingConstraints(Solution& solution, const ConstraintSet& cSet, Instance& instance);

		static bool checkCost(const Solution& solution, const Instance& instance);
		static bool checkTimeWindows(const Solution& solution, const Instance& instance);
		static bool checkTimeWindowCustomer(const unsigned int customer_id, const unsigned int customer_last_id, double& time, const Instance& instance);
		static bool checkCapacities(const Solution& solution, const bool load_capacity, const Instance& instance);
		static bool checkDispatchedCustomers(const Solution& solution, const Instance& instance);
		static bool checkDispatchedItems(const Solution& solution, const Instance& instance);
		static bool checkUsedVehicles(const Solution& solution, const Instance& instance);
		static double calcEuclideanDistance(const float ax, const float ay, const float bx, const float by);
	};
}