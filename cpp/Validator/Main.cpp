// This code deals with the checking of several loading constraints.
// The focus is on readability and comprehensibility, not on performance.

#include <string>
#include <iostream>
#include "Read.h"
#include "ConstraintsLoading.h"
#include "ConstraintsRouting.h" 
using namespace std;


int main(char* argv[]) {
	Instance	  instance		= Read::readInstanceFile  ("../../Input/Instances/Krebs_Ehmke_Koch_2020/001_n020_m200_bt3.txt");
	ConstraintSet constraintSet	= Read::readConstraintFile("../../Input/Constraint_Sets/P1.txt");
	Solution	  solution		= Read::readSolutionFile("../../Input/PackPlan/001_n020_m200_bt3_P1_1.txt", instance);

	if (ConstraintsRouting::checkRoutingConstraints(solution, constraintSet, instance)
		&& ConstraintsLoading::checkLoadingConstraints(solution, constraintSet, instance)) {
		cout << "All Constraints checked. Solution is feasible." << endl;
		return 1;
	}
	cerr << "Solution is not feasible. Please check error hints above." << endl;
	return 0;
}
