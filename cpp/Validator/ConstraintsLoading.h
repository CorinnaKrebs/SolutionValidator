#pragma once
#include "Item.h"
#include "Vehicle.h"
#include "Solution.h"
#include "Instance.h"
#include "ConstraintSet.h"

namespace validator {
	/**
	 * Class with all Loading Constraints
	 */
	class ConstraintsLoading {
	public:
		/**
		* Check solution concerning the feasibility of all loading constraints defined in the constraint set.
		*
		* @param solution the solution
		* @param cSet     the constraint set
		* @param instance the instance
		* @return the feasibility of the solution w.r.t. loading constraints
		*/
		static bool checkLoadingConstraints(Solution& solution, const ConstraintSet& cSet, Instance& instance, const bool& msg);

		/**
		* Check if an item overlaps with the vehicle walls.
		*
		* @param item    the item
		* @param vehicle the vehicle
		* @param msg     optional output messages
		* @return feasibility of item position.
		*/
		static bool checkVehicleWalls(const Item& item, const Vehicle& vehicle, const bool& msg);

		/**
		* Check if an item overlaps with any already placed item.
		*
		* @param item       the item
		* @param tour       the tour
		* @param tourEndPos the current position of the packing process
		* @param instance   the instance
		* @param msg        optional output message
		* @return feasibility of item position.
		*/
		static bool checkOverlapping(Item& item, Tour& tour, const unsigned int& tourEndPos, const Instance& instance, const bool& msg);

		/**
		* Check if the dimension of an item are calculated correctly.
		*
		* @param item the item
		* @param msg  optional output message
		* @return correctness of item dimensions
		*/
		static bool checkDimensions(const Item& item, const bool& msg);

		/**
		* Check if the maximum corner point is not zero.
		*
		* @param item the item
		* @param msg  optional output message
		* @return correctness of maximum corner point
		*/
		static bool checkMaxCoordinates(const Item& item, const bool& msg);

		/**
		* Check item position w.r.t. Unloading Sequence Constraints.
		*
		* @param item       the item
		* @param tour       the tour
		* @param tourEndPos the current position of the packing process
		* @param uSequence  the considered Unloading Sequence constraint
		* @param instance   the instance
		* @param msg        optional output message
		* @return feasibility of item position.
		*/
		static bool checkUnloadingSequence(Item& item, Tour& tour, const unsigned int& tourEndPos, const UnloadingSequence& uSequence, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Stacking Constraints.
		*
		* @param item       the item
		* @param tour       the tour
		* @param tourEndPos the current position of the packing process
		* @param stacking   the considered stacking constraint
		* @param instance   the instance
		* @param msg        optional output message
		* @return feasibility of item position.
		*/
		static bool checkStacking(Item& item, Tour& tour, const unsigned int& tourEndPos, const Stacking& stacking, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Fragility Constraint.
		* Non-Fragile Items cannot be stacked on fragile items.
		* Fragile Item can be stacked on top of fragile items.
		*
		* @param item       the item
		* @param tour       the tour
		* @param tourEndPos the current position of the packing process
		* @param instance   the instance
		* @param msg        optional output message
		* @return feasibility of item position.
		*/
		static bool checkFragility(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. LBS (Complete or Simple) Constraint.
		*
		* @param below    the item below
		* @param above    the item above
		* @param weight   the weight to be distributed
		* @param instance the instance
		* @param stacking the stacking constraint (LBS)
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkLBS(Item& below, Item& above, double weight, Instance& instance, const Stacking& stacking, const bool& msg);

		/**
		* Check item position w.r.t. LBS Simple Constraint.
		* The Load of an item is distributed to all indirect and direct
		* supporting items underneath the item base area.
		*
		* @param item     the item
		* @param instance the instance
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkLBSSimple(Item& item, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. LBS Complete Constraint.
		* The load of an item is recursively distributed to the direct underlying items.
		*
		* @param item     the item
		* @param instance the instance
		* @param load     the load
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkLBSComplete(Item& item, Instance& instance, double load, const bool& msg);

		/**
		* Check item position w.r.t. Vertical Stability Constraints.
		*
		* @param item       the item
		* @param vStability the considered vertical stability constraint
		* @param alpha      support parameter (part of base area)
		* @param instance   the instance
		* @param msg        optional output message
		* @return feasibility of item position.
		*/
		static bool checkVStability(Item& item, const VerticalStability& vStability, const float& alpha, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. at least one directly underlying item.
		*
		* @param item     the item
		* @param instance the instance
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkFlyingItem(Item& item, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Minimal Supporting Area Constraint.
		* Each item has a supporting area of at least a percentage alpha of its base area;
		*
		* @param item     the item
		* @param alpha    support parameter (part of base area)
		* @param instance the instance
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkMinimalSupport(Item& item, const float& alpha, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Multiple Overhanging Constraint.
		* Each item has a supporting area of at least a percentage alpha of its base area at any height.
		* Therefore check the Minimal Supporting Area at each level.
		*
		* @param item     the item
		* @param alpha    support parameter (part of base area)
		* @param instance the instance
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkMultipleOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Top Overhanging Constraint.
		* All Items of a stack must be fully supported except the topmost item,
		* which is allowed to overhanging respecting the Minimal Supporting Area Constraint.
		*
		* @param item     the item
		* @param alpha    support parameter (part of base area)
		* @param instance the instance
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkTopOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Static Stability Constraint.
		* Each item has a center of gravity, which must be within an item at each level.
		*
		* @param item     the item
		* @param instance the instance
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkStaticStability(Item& item, Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Static Stability Constraint proposed by Mack et al. (2004).
		* The center of gravity of each item must be supported by underlying items and
		* the base area of each item must be supported by the items on the ground.
		* @param item     the item
		* @param alpha    support parameter (part of base area)
		* @param instance the instance
		* @param msg      optional output message
		* @return feasibility of item position.
		*/
		static bool checkStaticStabiltyMack(Item& item, const float& alpha, Instance& instance, const bool& msg);

		/**
		* Check if item position does not lead to an exceedence of the axle weights.
		*
		* @param item         the item
		* @param tour         the tour
		* @param axle_weights the consideration of axle weights
		* @param instance     the instance
		* @param msg          optional output messages
		* @return feasibility of item position.
		*/
		static bool checkAxleWeights(Item& item, Tour& tour, const bool& axle_weights, const Instance& instance, const bool& msg);

		/**
		* Check if item position does not lead to an exceedence of the permissible load per vehicle half.
		* The load of one vehicle half does not exceed a certain percentage of the vehicle capacity.
		*
		* @param item             the item
		* @param tour             the tour
		* @param balanced_loading the consideration of balanced loading
		* @param balanced_part    the part of the vehicle capacity
		* @param instance         the instance
		* @param msg              optional output messages
		* @return feasibility of item position.
		*/
		static bool checkBalancedLoading(Item& item, Tour& tour, const bool& balanced_loading, const float& balanced_part, const Instance& instance, const bool& msg);

		/**
		* Check item position w.r.t. Reachability Constraint.
		*
		* @param item         the item
		* @param tour         the tour
		* @param lambda       the minimum dimension units
		* @param reachability the consideration of reachability constraint
		* @param instance     the instance
		* @param msg          optional output message
		* @return feasibility of item position.
		*/
		static bool checkReachability(Item& item, Tour& tour, const unsigned int& lambda, const bool& reachability, Instance& instance, const bool& msg);

		/**
		* Calculate the total support of an item.
		*
		* @param item     the item
		* @param instance the instance
		*/
		static void   calcItemSupport(Item& item, Instance& instance);

		/**
		* Calculate the support area units between two items.
		*
		* @param current the current item
		* @param other   the other item
		* @return the supporting area units
		*/
		static int    calcSupportArea(const Item& current, const Item& other);
		
		/**
		* Fills relevant sets of items.
		* For each item, add all indirect supporting items,
		* add all items above,
		* add all items in front and
		* add all items behind.
		*
		* @param tour       the tour
		* @param tourEndPos the current position of the packing process
		* @param instance   the instance
		*/
		static void getRelevantItems(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance);

		/**
		* Gets the scale factor for Reachability Constraint
		* to rescale the lambda factor.
		*
		* @param vehicleH the vehicle height
		* @return the scale factor
		*/
		static int  getScaleFactor(const unsigned int& vehicleH);
	};
}
