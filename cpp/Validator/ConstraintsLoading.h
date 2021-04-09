#pragma once
#include "Item.h"
#include "Vehicle.h"
#include "Solution.h"
#include "Instance.h"
#include "ConstraintSet.h"

namespace validator {

	class ConstraintsLoading {
	public:
		static bool checkLoadingConstraints(Solution& solution, const ConstraintSet& cSet, Instance& instance);

		static bool checkVehicleWalls(const Item& item, const Vehicle& vehicle, const bool& msg);
		static bool checkAxleWeights(Item& item, Tour& tour, const bool& axle_weights, const Instance& instance, const bool& msg);
		static bool checkBalancedLoading(Item& item, Tour& tour, const bool& balanced_loading, const float& balanced_part, const Instance& instance, const bool& msg);
		static bool checkOverlapping(Item& item, Tour& tour, const unsigned int& tourEndPos, const Instance& instance, const bool& msg);
		static bool checkDimensions(const Item& item, const bool& msg);
		static bool checkMaxCoordinates(const Item& item, const bool& msg);
		static bool checkStacking(Item& item, Tour& tour, const unsigned int& tourEndPos, const Stacking& stacking, Instance& instance, const bool& msg);
		static bool checkVStability(Item& item, const VerticalStability& vStability, const float& alpha, Instance& instance, const bool& msg);
		static bool checkUnloadingSequence(Item& item, Tour& tour, const unsigned int& tourEndPos, const UnloadingSequence& uSequence, Instance& instance, const bool& msg);
		static bool checkFragility(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance, const bool& msg);
		static bool checkMinimalSupport(Item& item, const float& alpha, Instance& instance, const bool& msg);
		static bool checkTopOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg);
		static bool checkMultipleOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg);
		static bool checkStaticStability(Item& item, Instance& instance, const bool& msg);
		static bool checkLBSSimple(Item& item, Instance& instance, const bool& msg);
		static bool checkLBSComplete(Item& item, Instance& instance, double load, const bool& msg);
		static bool checkReachability(Item& item, Tour& tour, const unsigned int& lambda, const bool& reachability, Instance& instance, const bool& msg);
		static bool checkLBS(Item& below, Item& above, double weight, Instance& instance, const Stacking& stacking, const bool& msg);

		static int    calcSupportArea(const Item& current, const Item& other);
		static void   calcItemSupport(Item& item, Instance& instance);


		static void getRelevantItems(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance);
		static int  getScaleFactor(const unsigned int& vehicleH);
	};
}
