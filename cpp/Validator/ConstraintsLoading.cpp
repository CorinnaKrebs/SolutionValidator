#include <iostream>
#include <algorithm>
#include "ConstraintsLoading.h"
#include "Item.h"
#include "../../../DBLFwithSpaces/DBLFwithSpaces/DBLF.h"
#include "Write.h"
using namespace std;

bool ConstraintsLoading::checkLoadingConstraints(Solution& solution, const ConstraintSet& cSet, Instance& instance) {
	const unsigned int scaleFactor = getScaleFactor(instance.vehicle.h);

	for (auto& tour : solution.tours) {
		tour.feasible = true;
		
		// Necessary for some constraints beforehand
		for (const auto item_id : tour.item_ids) {
			auto& currentItem = instance.items.at(item_id);
			getRelevantItems(currentItem, tour, tour.item_ids.size(), instance);
		}

		const unsigned int endPos = tour.item_ids.size();
		for (const auto item_id : tour.item_ids) {
			auto& currentItem = instance.items.at(item_id);
			if (!checkVehicleWalls(currentItem, instance.vehicle, true) 
				|| !checkMaxCoordinates(currentItem, true)
				|| !checkDimensions(currentItem, true)
				|| !checkOverlapping(currentItem, tour, endPos, instance, true)
				|| !checkAxleWeights(currentItem, tour, cSet.axle_weights, instance, true)
				|| !checkBalancedLoading(currentItem, tour, cSet.balanced_loading, cSet.balanced_part, instance, true)
				|| !checkUnloadingSequence(currentItem, tour, endPos, cSet.uSequence, instance, true)
				|| !checkReachability(currentItem, tour, cSet.lambda * scaleFactor, cSet.reachability, instance, true)
				|| !checkVStability(currentItem, cSet.vStability, cSet.alpha, instance, true)
				|| !checkStacking(currentItem, tour, endPos, cSet.stacking, instance, true)) {
				cerr << "Solution is NOT feasible according Loading Constraints." << endl;
				tour.feasible = false;
				return false;
			}
		}
	}

	cout << "Solution is feasible according Loading Constraints." << endl;
	return true;
}


bool ConstraintsLoading::checkVehicleWalls(const Item& item, const Vehicle& vehicle, const bool& msg) {
	if (item.max.x > vehicle.l || item.max.y > vehicle.w || item.max.z > vehicle.h) {
		if (msg) cerr << "Overlapping of item " << item.id << " with Vehicle Walls." << endl;
		return false;
	}
	return true;
}

bool ConstraintsLoading::checkDimensions(const Item& item, const bool& msg) {
	unsigned int l = item.max.x - item.min.x;
	unsigned int w = item.max.y - item.min.y;
	unsigned int h = item.max.z - item.min.z;
	if (l + w + h != item.l + item.w + item.h) {
		if (msg) cerr << "Dimensions of " << item.id << " not fitting." << endl;
		return false;
	}
	return true;
}

bool ConstraintsLoading::checkMaxCoordinates(const Item& item, const bool& msg) {
	if (item.max == Point(0, 0, 0)) {
		if (msg) cerr << "Maximal Coordinates of item " << item.id << " is Origin. " << endl;
		return false;
	}
	return true;
}

bool ConstraintsLoading::checkOverlapping(Item& item, Tour& tour, const unsigned int& tourEndPos, const Instance& instance, const bool& msg) {
	for (size_t i = 0; i < tourEndPos; ++i) {
		const auto item_id = tour.item_ids.at(i);
		if (item_id == item.id) continue;
		auto& other = instance.items.at(item_id);

		if (item.min.x < other.max.x
			&& item.min.y < other.max.y
			&& item.min.z < other.max.z
			&& other.min.x < item.max.x
			&& other.min.y < item.max.y
			&& other.min.z < item.max.z) {
			if (msg) cerr << "Overlapping of item " << item.id << " and Item " << other.id << endl;
			return false;
		}
	}
	return true;
}


bool ConstraintsLoading::checkUnloadingSequence(Item& item, Tour& tour, const unsigned int& tourEndPos, const UnloadingSequence& uSequence, Instance& instance, const bool& msg) {
	if (uSequence != UnloadingSequence::none) {
		for (size_t i = 0; i < tourEndPos; ++i) {
			const auto item_id = tour.item_ids.at(i);
			auto& other = instance.items.at(item_id);
			if (item.customer_id == other.customer_id) {
				continue;
			}
			// Get Positions of Customers in Tour
			auto itCurrentCustomer = find(tour.customer_ids.begin(), tour.customer_ids.end(), item.customer_id);
			auto itOtherCustomer = find(tour.customer_ids.begin(), tour.customer_ids.end(), other.customer_id);
			int posCurrentCustomer = std::distance(tour.customer_ids.begin(), itCurrentCustomer);
			int posOtherCustomer = std::distance(tour.customer_ids.begin(), itOtherCustomer);

			// Other Customer is Delivered After Current 
			if (posOtherCustomer > posCurrentCustomer) {
				if (item.isBehind(other)) {
					if (msg) cerr << "Unloading Sequence for Item " << item.id << " not respected due to Item " << other.id << endl;
					return false;
				}
				if ((uSequence == UnloadingSequence::LIFO && item.isBelow(other, false)) ||
					(uSequence == UnloadingSequence::MLIFO && item.isBelow(other, true))) {
					if (msg) cerr << "Unloading Sequence for Item " << item.id << " not respected due to Item " << other.id << endl;
					return false;
				}
			}	
		}
	}
	return true;
}

bool ConstraintsLoading::checkReachability(Item& item, Tour& tour, const unsigned int& lambda, const bool& reachability, Instance& instance, const bool& msg) {
	if (reachability) {
		// Determine max X
		unsigned int maxX = item.max.x;
		for (auto& item_id : tour.item_ids) {
			auto& other = instance.items.at(item_id);

			// Get Positions of Customers in Tour
			auto itCurrentCustomer = find(tour.customer_ids.begin(), tour.customer_ids.end(), item.customer_id);
			auto itOtherCustomer = find(tour.customer_ids.begin(), tour.customer_ids.end(), other.customer_id);
			int posCurrentCustomer = std::distance(tour.customer_ids.begin(), itCurrentCustomer);
			int posOtherCustomer = std::distance(tour.customer_ids.begin(), itOtherCustomer);
			
			if (posOtherCustomer > posCurrentCustomer && item.max.y > other.min.y && item.min.y < other.max.y) {
				maxX = max(maxX, other.max.x);
			}
		}
		// Check Reachability
		if (maxX - item.max.x > lambda) {
			if (msg) cerr << "Not reachable Item " << item.id << endl;
			return false;
		}
	}
	return true;
}

bool ConstraintsLoading::checkStacking(Item& item, Tour& tour, const unsigned int& tourEndPos, const Stacking& stacking, Instance& instance, const bool& msg) {
	if (stacking == Stacking::LBSComplete || stacking == Stacking::LBSSimple) {
		unsigned int area = item.l * item.w;
		item.load.reserve(area);
		fill(item.load.begin(), item.load.end(), 0);
	}

	switch (stacking) {
	case Stacking::Fragility:	return checkFragility(item, tour, tourEndPos, instance, msg);
	case Stacking::LBSSimple:   return checkLBSSimple(item, instance, msg);
	case Stacking::LBSComplete:	return checkLBSComplete(item, instance, item.mass, msg);
	case Stacking::none:		return true;
	}
	return true;
}

bool ConstraintsLoading::checkFragility(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance, const bool& msg) {
	for (size_t i = 0; i < tourEndPos; ++i) {
		const auto& item_id = tour.item_ids.at(i);
		auto& other = instance.items.at(item_id);
		if (other.isBelow(item, true) && other.fragility && !item.fragility) {
			if (msg) cerr << "Item " << item.id << " is placed on top of fragile item " << other.id << endl;
			return false;
		}
	}
	return true;
}

bool ConstraintsLoading::checkVStability(Item& item, const VerticalStability& vStability, const float& alpha, Instance& instance, const bool& msg) {
	if (item.min.z == 0) {
		item.support = 1.0;
		return true;
	}

	switch (vStability) {
	case VerticalStability::MinimalSupport:
		return checkMinimalSupport(item, alpha, instance, msg);
	case VerticalStability::MultipleOverhanging:
		return checkMultipleOverhanging(item, alpha, instance, msg);
	case VerticalStability::TopOverhanging:
		return checkTopOverhanging(item, alpha, instance, msg);
	case VerticalStability::none:
		return checkMinimalSupport(item, 0.1f, instance, msg);
	}
	return true;
}

bool ConstraintsLoading::checkMinimalSupport(Item& item, const float& alpha, Instance& instance, const bool& msg) {
	item.support = calcSupport(item, instance);

	// Check Support
	if (item.support < alpha * 0.99) {
		if (msg) cerr << "Item " << item.id << " not enough supported." << endl;
		return false;
	}
	return true;
}

bool ConstraintsLoading::checkMultipleOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg) {
	double min_support = 1.0;
	// Item a is one level
	for (const auto& a_id : item.itemsBelow) {
		Item& aBelow = instance.items.at(a_id);
		double supportArea = calcSupportArea(item, aBelow);

		// Search for Items b, which in the same level of item a
		for (auto b_id : item.itemsBelow) {
			if (a_id == b_id) continue;
			Item& bBelow = instance.items.at(b_id);

			// If item b lies in the same level of item a
			if (bBelow.max.z >= aBelow.max.z && bBelow.min.z < aBelow.max.z)
				supportArea += calcSupportArea(bBelow, item);
			}
			double support = supportArea / ((double) item.l * item.w);
			min_support = min(support, min_support);
	}
	if (min_support < alpha * 0.99) {
		if (msg) cerr << "Item " << item.id << " not enough supported." << endl;
		return false;
	}
	return true;
}

bool ConstraintsLoading::checkTopOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg) {
	if (item.support == 0) {
		item.support = calcSupport(item, instance);
	}

	// Last Item of Stack
	if (item.itemsAbove.size() == 0 && item.support < alpha * 0.99) {
		if (msg) cerr << "Item " << item.id << " of Customer " << item.customer_id << " not enough supported." << endl;
		return false;
	}
	
	// Other item can hang over current item
	for (const auto& otherId : item.itemsAbove) {
		auto& other = instance.items.at(otherId);		
		if (other.isAbove(item, 1) && item.support < 0.99) {
			if (msg) cerr << "Item " << item.id << " of Customer " << item.customer_id << " not enough supported." << endl;
			return false;
		}
	}

	return true;
}


double ConstraintsLoading::calcSupport(Item& item, Instance& instance) {
	// On Bottom
	if (item.min.z == 0) {
		item.support = 1.0;
	}
	else {
		// Calculate Support
		double supportArea = 0;
		for (auto& item_id : item.itemsBelow) {
			auto& other = instance.items.at(item_id);
			if (other.isBelow(item, 1)) {
				supportArea += calcSupportArea(item, other);
			}
		}
		item.support = supportArea / ((double)item.l * item.w);
	}
	return item.support;
}


int ConstraintsLoading::calcSupportArea(const Item& current, const Item& other) {
	unsigned int minX = max(current.min.x, other.min.x);
	unsigned int minY = max(current.min.y, other.min.y);
	unsigned int maxX = min(current.max.x, other.max.x);
	unsigned int maxY = min(current.max.y, other.max.y);
	int area = (maxX - minX) * (maxY - minY);

	return max(area, 0);
}

bool ConstraintsLoading::checkLBS(Item& below, Item& above, double weight, Instance& instance, const Stacking& stacking, const bool& msg) {
	if (above.support == 0) {
		above.support = calcSupport(above, instance);
	}

	// Get Support Area between above Item and below Item
	int minY = max(above.min.y, below.min.y) - below.min.y;
	int minX = max(above.min.x, below.min.x) - below.min.x;
	int maxY = min(above.max.y, below.max.y) - below.min.y;
	int maxX = min(above.max.x, below.max.x) - below.min.x;
	int supportArea = (maxY - minY) * (maxX - minX);
	
	// Calculate part of support by above Item
	double support_below = (double) supportArea / ((double)above.l * above.w);
	support_below /= above.support;

	// Calculate part of load
	double load_part   = weight * support_below;
	double loadPerArea = load_part / (double) supportArea;

	// Check if Load per Area Unit exceeds in general the limit
	if (loadPerArea > below.lbs) return false;

	// Save total load
	below.sumLoad += load_part;

	// Check Load per Area Unit
	for (int a = minX; a < maxX; ++a) { // Row
		for (int b = minY; b < maxY; ++b) { // Column
			// Determine Square
			int square = b + (below.max.y - below.min.y) * a;

			// Save Load for the Square
			below.load[square] += loadPerArea;

			// Check Load
			if (below.load[square] > below.lbs)	
				return false;
		}
	}

	// Recursiv Call
	if (stacking == Stacking::LBSComplete) {
		return checkLBSComplete(below, instance, load_part, msg);
	}
	return true;
}

bool ConstraintsLoading::checkLBSComplete(Item& item, Instance& instance, double load, const bool& msg) {
	for (auto& below_id : item.itemsBelow) {
		Item& below = instance.items.at(below_id);

		// Consider items with direct support
		if (below.max.z != item.min.z) continue;

		// Call CalcLBS with adapted load
		if (!checkLBS(below, item, load, instance, Stacking::LBSComplete, msg)) {
			if (msg) cerr << "Load exceeded for item " << below.id << endl;
			return false;
		}
	}
	return true;
}

bool ConstraintsLoading::checkLBSSimple(Item& item, Instance& instance, const bool& msg) {
	for (const auto& below_id : item.itemsBelow) {
		Item& below = instance.items.at(below_id);

		// Call Calc_LBS with mass of above item
		if (!checkLBS(below, item, item.mass, instance, Stacking::LBSSimple, msg)) {
			if (msg) cerr << "Load exceeded for item " << below.id << endl;
			return false;
		}
	}

	return true;
}


bool ConstraintsLoading::checkAxleWeights(Item& item, Tour& tour, const bool& axle_weights, const Instance& instance, const bool& msg) {
	if (axle_weights) {
		const double g = 9.81;
		long item_T = 0;

		// Calculate Item's Force
		double item_F = item.mass * g;
		const double length = !item.rotated ? item.l : item.w;
		double s = 0;

		long F_RA = 0, F_FA = 0, F_TA = 0;

		if (!instance.vehicle.axle_trailer) {

			// Calculate Item's Torque
			s = (double) instance.vehicle.l_FA_cargo + item.min.x + 0.5 * length;
			item_T = s * item_F;

			int l_truck_FA = instance.vehicle.wheelbase - instance.vehicle.l_tractor_RA;

			// Calculate Axle Weights
			F_RA = (tour.sum_T + item_T + instance.vehicle.mass_tractor * g * l_truck_FA) / instance.vehicle.wheelbase;
			
			F_FA = (tour.sum_F + item_F + instance.vehicle.mass_tractor * g) - F_RA;
		}
		
		else {
			// Calculate Item's Torque
			s = (double) instance.vehicle.l_cargo_TA - item.min.x - 0.5 * length;
			item_T = s * item_F;
			
			// Calculate Axle Weights
			long F_K = (1.0 / instance.vehicle.l_kingpin_TA) * (tour.sum_T + item_T + instance.vehicle.mass_trailer * g * instance.vehicle.l_trailer_TA);

			F_TA = tour.sum_F + item_F + instance.vehicle.mass_trailer * g - F_K;

			F_FA = (1.0 / instance.vehicle.wheelbase) * (F_K * instance.vehicle.l_kingpin_RA + instance.vehicle.mass_tractor * g * instance.vehicle.l_tractor_RA);

			F_RA = F_K + instance.vehicle.mass_tractor * g - F_FA;
		}

		tour.sum_F += item_F;
		tour.sum_T += item_T;

		// Check Axle Weight Limits
		if (F_RA > instance.vehicle.lim_rearAxle * g) {
			if (msg) cerr << "Overloaded Rear Axle of Tour " << tour.id << endl;
			return false;
		}

		if (F_FA > instance.vehicle.lim_frontAxle * g) {
			if (msg) cerr << "Overloaded Front Axle of Tour " << tour.id << endl;
			return false;
		}

		if (F_TA > instance.vehicle.lim_trailerAxle * g) {
			if (msg) cerr << "Overloaded Trailer Axle of Tour " << tour.id << endl;
			return false;
		}
	}
  	
	
	return true;
}


bool ConstraintsLoading::checkBalancedLoading(Item& item, Tour& tour, const bool& balanced_loading, const float& balanced_part, const Instance& instance, const bool& msg) {
	if (balanced_loading) {
		const double limit = instance.vehicle.D * (double) balanced_part;
		const double W_half = instance.vehicle.w * 0.5;

		// In Left Half
		if (item.max.y <= W_half) {
			tour.mass_L += item.mass;
		}
		
		// In Right Half
		else if (item.min.y > W_half) {
			tour.mass_R += item.mass;
		}
		// Distribute Mass
		else {
			const double width = !item.rotated ? item.w : item.l;
			const double left_area = (W_half - item.min.y) * width;
			const double part = left_area / ((double) item.w * item.l);
			const double mass_left = part * item.mass;
			tour.mass_L += mass_left;
			tour.mass_R += item.mass - mass_left;
		}

		// Check the Limits
		if (tour.mass_L > limit) {
			if (msg) cerr << "Overloaded Left Vehicle Half of Tour" << tour.id << endl;
			return false;
		}

		if (tour.mass_R > limit) {
			if (msg) cerr << "Overloaded Right Vehicle Half of Tour " << tour.id << endl;
			return false;
		}
	}
	return true;
}


void ConstraintsLoading::getRelevantItems(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance) {
	for (size_t i = 0; i < tourEndPos; ++i) {
		const auto other_id = tour.item_ids.at(i);
		if (other_id == item.id) continue;
		auto& other = instance.items.at(other_id);
		// Get direct supporting items
		if (other.isBelow(item, 1)) {
			item.itemsBelow.insert(other.id);
			other.itemsAbove.insert(item.id);
		}
		if (other.isBehind(item)) {
			item.itemsBehind.insert(other.id);
			other.itemsFront.insert(item.id);
		}
	}
	// Add indirect supporting items
	for (auto& id : tour.item_ids) {
		auto& item = instance.items.at(id);
		for (auto& direct_id : item.itemsBelow) {
			auto& directItem = instance.items.at(direct_id);
			for (auto& indirect_id : directItem.itemsBelow) {
				auto& indirectItem = instance.items.at(indirect_id);
				if (indirectItem.isBelow(item, 0)) {
					item.itemsBelow.insert(indirect_id);
					indirectItem.itemsAbove.insert(item.id);
				}
			}
		}
	}
	
}

int ConstraintsLoading::getScaleFactor(const unsigned int& vehicleH) {
	// Scale Lambda
	unsigned int digits = log10(vehicleH) + 1;
	return pow(10, digits - 2);	
}