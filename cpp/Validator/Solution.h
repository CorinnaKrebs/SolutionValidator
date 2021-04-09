#pragma once
#include <vector>

#include "Tour.h"
namespace validator {

	class Solution {
	public:
		Solution() : total_travel_distance(0), feasible(false), total_iterations(0), calculation_time(0) {}
		Solution(std::vector<Tour>& tours) : tours(tours), total_travel_distance(0), feasible(false), total_iterations(0), calculation_time(0) {}
		std::vector<Tour> tours;
		double       total_travel_distance;
		bool         feasible;
		unsigned int total_iterations;
		double       calculation_time;
	};

}