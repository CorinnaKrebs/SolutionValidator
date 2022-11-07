#pragma once
#include <vector>

#include "Tour.h"
namespace validator {

	/**
	* The type Solution.
	*/
	class Solution {
	public:
		Solution() : total_travel_distance(0), feasible(false), total_iterations(0), calculation_time(0) {}

		/**
		* Instantiates a new Solution.
		*
		* @param tours the list of Tours
		*/
		Solution(std::vector<Tour>& tours) : tours(tours), total_travel_distance(0), total_travel_time(0),unloading_effort(0), 
			feasible(false), total_iterations(0), calculation_time(0) {}

		/** the list of Tours */
		std::vector<Tour> tours;

		/** Total Travel Distance */
		double       total_travel_distance;

		/** Total Travel Time */
		double		 total_travel_time;

		/** Total Unloading Effort */
		double		unloading_effort;

		/** Feasibility of the Solution (1: feasible) */
		bool         feasible;

		/** Total Number of conducted Iterations */
		unsigned int total_iterations;

		/** Total Calculation Time */
		double       calculation_time;
	};

}