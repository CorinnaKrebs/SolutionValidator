#pragma once
#include <vector>

namespace validator {
	/**
	* The type Tour.
	*/
	class Tour {

	public:
		/**
		* Instantiates a new Tour.
		*
		* @param id           the Unique id
		* @param customer_ids the Sequence of Customer Ids in Visiting Order
		* @param item_ids     the Sequence of Item Ids
		*/
		Tour(unsigned int id, std::vector<unsigned int> customer_ids, std::vector<unsigned int> item_ids)
			: id(id), customer_ids(customer_ids), item_ids(item_ids), mass_R(0), mass_L(0), sum_F(0), sum_T(0), feasible(false) {}

		/** Unique id */
		unsigned int id;

		/** Sequence of Customer Ids in Visiting Order */
		std::vector<unsigned int> customer_ids;

		/** Sequence of Item Ids */
		std::vector<unsigned int> item_ids;

		/** Current sum of torques */
		double				sum_T;

		/** Current sum of forces */
		double				sum_F;

		/** Current load of the right vehicle side */
		double				mass_R;

		/** Current load of the left vehicle side */
		double				mass_L;

		/** Feasibility of Tour (0: infeasible, 1: feasible) */
		bool				feasible;
	};
}
