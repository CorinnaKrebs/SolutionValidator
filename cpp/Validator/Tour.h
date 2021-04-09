#pragma once
#include <vector>

namespace validator {

	class Tour {

	public:
		Tour(unsigned int id, std::vector<unsigned int> customer_ids, std::vector<unsigned int> item_ids)
			: id(id), customer_ids(customer_ids), item_ids(item_ids), mass_R(0), mass_L(0), sum_F(0), sum_T(0), feasible(false) {}

		unsigned int id;
		std::vector<unsigned int> customer_ids; // Sequence of Customer Ids in Visiting Order
		std::vector<unsigned int> item_ids;		// Sequence of Item Ids in Loading Order

		double				sum_T;				// Current sum of torques for rear axle
		double				sum_F;				// Current sum of force
		double				mass_R;				// Current load of the right vehicle side
		double				mass_L;				// Current load of the left vehicle side
		bool				feasible;			// Feasibility of Tour (0: infeasible, 1: feasible)
	};
}
