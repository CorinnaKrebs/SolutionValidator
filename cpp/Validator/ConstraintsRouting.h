#pragma once
#include "Solution.h"
#include "Instance.h"
#include "ConstraintSet.h"

namespace validator {
	/**
	 * Class with all Routing Constraints.
	 */
	class ConstraintsRouting {
	public:
		/**
		* Check solution concerning the feasibility of all routing constraints defined in the constraint set.
		*
		* @param solution the solution
		* @param cSet     the constraint set
		* @param instance the instance
		* @param msg     optional output messages
		* @return the feasibility of the solution w.r.t. routing constraints
		*/
		static bool checkRoutingConstraints(Solution& solution, const ConstraintSet& cSet, Instance& instance, const bool& msg);

		/**
		* Check the calculation of the total travel distance.
		*
		* @param solution the solution
		* @param instance the instance
		* @param msg      optional output messages
		* @return feasibility of the calculation.
		*/
		static bool checkCost(const Solution& solution, const Instance& instance, const bool& msg);

		/**
		* Check if all time windows are met.
		*
		* @param solution the solution
		* @param instance the instance
		* @param msg      optional output messages
		* @return feasibility of time windows.
		*/
		static bool checkTimeWindows(const Solution& solution, const Instance& instance, const bool& msg);

		/**
		* Calculate the current time after each customer.
		*
		* @param customer_id      the customer id
		* @param customer_last_id the customer last id
		* @param time             the current time before visiting the customer.
		* @param instance         the instance
		* @param msg			  optional output messages
		* @return the current time after visiting the customer.
		*/
		static bool checkTimeWindowCustomer(const unsigned int customer_id, const unsigned int customer_last_id, double& time, const Instance& instance, const bool& msg);

		/**
		* Check if the vehicle dimensions are met.
		*
		* @param solution      the solution
		* @param load_capacity the consideration of the load capacity
		* @param instance      the instance
		* @param msg		   optional output messages
		* @return feasibility of vehicle dimensions (Mass and Volume).
		*/
		static bool checkCapacities(const Solution& solution, const bool load_capacity, const Instance& instance, const bool& msg);

		/**
		* Check if all customers are dispatched and visited only once.
		*
		* @param solution the solution
		* @param instance the instance
		* @param msg      optional output messages
		* @return true if all customers are visited once.
		*/
		static bool checkDispatchedCustomers(const Solution& solution, const Instance& instance, const bool& msg);

		/**
		* Check if all items of all customers are dispatched.
		*
		* @param solution the solution
		* @param instance the instance
		* @param msg      optional output messages
		* @return true if all items of all customers are dispatched.
		*/
		static bool checkDispatchedItems(const Solution& solution, const Instance& instance, const bool& msg);

		/**
		* Check if the solution exceeds the number of used vehicles.
		* Lazy-Constraint since solution can exceed the number of used vehicles.
		*
		* @param solution the solution
		* @param instance the instance
		* @param msg      optional output messages
		* @return true
		*/
		static bool checkUsedVehicles(const Solution& solution, const Instance& instance, const bool& msg);

		/**
		* Calculate the euclidean distance between Point a and Point b.
		*
		* @param ax the x-coordinate of Point a.
		* @param ay the y-coordinate of Point a.
		* @param bx the x-coordinate of Point b.
		* @param by the y-coordinate of Point a.
		* @return the euclidean distance.
		*/
		static double calcEuclideanDistance(const float ax, const float ay, const float bx, const float by);
	};
}