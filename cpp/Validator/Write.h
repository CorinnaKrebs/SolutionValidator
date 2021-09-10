#pragma once
#include "Instance.h"
#include "Solution.h"

namespace validator {
	class ConstraintSet;

	/**
	* The Write Class.
	*/
	class Write	{
	public:

		/**
		* Write an instance to file.
		*
		* @param path         the path of the output file.
		* @param instance     the instance
		*/
		static void instanceFile(const std::string path, Instance& instance);

		/**
		* Write a constraint set to file.
		*
		* @param path			   the path of the output file.
		* @param constraintSet     the constraintSet
		* @param constraintNumber  the number of the constraint set
		*/
		static void constraintFile(const std::string path, ConstraintSet& constraintSet, const int constraintNumber);
		
        /**
	    * Write a packing plan (aka solution) to file.
	    *
	    * @param path         the path of the output file.
	    * @param solution     the solution
	    * @param instance     the instance
	    * @param constraintNo the constraint number (e.g. P1)
	    * @param seed         the seed, the test number
	    */
		static void solutionFile(const std::string path, Solution& solution, Instance& instance, const int constraintNo, const int seed);


	};

}

