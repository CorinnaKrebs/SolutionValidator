#pragma once
#include "Instance.h"
#include "Solution.h"

namespace validator {

    /**
	* The Write Class.
	*/
	class Write	{
	public:
		
        /**
	    * Write a packing plan to file.
	    *
	    * @param path         the path of the output file.
	    * @param solution     the solution
	    * @param instance     the instance
	    * @param constraintNo the constraint number (e.g. P1)
	    * @param seed         the seed, the test number
	    */
		static void writePackingPlan(const std::string path, Solution& solution, Instance& instance, const int constraintNo, const int seed);
	};

}

