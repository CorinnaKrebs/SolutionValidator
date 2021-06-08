// This code deals with the checking of several loading constraints.
// The focus is on readability and comprehensibility, not on performance.

#include <iostream>
#include "Read.h"
#include "ConstraintsLoading.h"
#include "ConstraintsRouting.h" 

int main(char* argv[]) {
	validator::Instance	  instance			= validator::Read::readInstanceFile  ("../../Input/Instances/Krebs_Ehmke_Koch_2020/005_n020_m200_bt3.txt");
	validator::ConstraintSet constraintSet	= validator::Read::readConstraintFile("../../Input/Constraint_Sets/P14.txt");
	validator::Solution	  solution			= validator::Read::readSolutionFile("../../Input/PackPlan/005_n020_m200_bt3_P14_1.txt", instance);

	if (validator::ConstraintsRouting::checkRoutingConstraints(solution, constraintSet, instance, true)
		&& validator::ConstraintsLoading::checkLoadingConstraints(solution, constraintSet, instance, true)) {
		std::cout << "All Constraints checked. Solution is feasible." << std::endl;
		return 1;
	}
	std::cerr << "Solution is not feasible. Please check error hints above." << std::endl;
	return 0;
}