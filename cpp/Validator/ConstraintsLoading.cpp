#include <iostream>
#include <algorithm>
#include "ConstraintsLoading.h"
#include "Item.h"
#include "Write.h"

bool validator::ConstraintsLoading::checkLoadingConstraints(Solution& solution, const ConstraintSet& cSet, Instance& instance) {
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
				std::cerr << "Solution is NOT feasible according Loading Constraints." << std::endl;
				tour.feasible = false;
				return false;
			}
		}
	}

	std::cout << "Solution is feasible according Loading Constraints." << std::endl;
	return true;
}


bool validator::ConstraintsLoading::checkVehicleWalls(const Item& item, const Vehicle& vehicle, const bool& msg) {
	if (item.max.x > vehicle.l || item.max.y > vehicle.w || item.max.z > vehicle.h) {
		if (msg) std::cerr << "Overlapping of item " << item.id << " with Vehicle Walls." << std::endl;
		return false;
	}
	return true;
}

bool validator::ConstraintsLoading::checkDimensions(const Item& item, const bool& msg) {
	unsigned int l = item.max.x - item.min.x;
	unsigned int w = item.max.y - item.min.y;
	unsigned int h = item.max.z - item.min.z;
	if (l + w + h != item.l + item.w + item.h) {
		if (msg) std::cerr << "Dimensions of " << item.id << " not fitting." << std::endl;
		return false;
	}
	return true;
}

bool validator::ConstraintsLoading::checkMaxCoordinates(const Item& item, const bool& msg) {
	if (item.max == Point(0, 0, 0)) {
		if (msg) std::cerr << "Maximal Coordinates of item " << item.id << " is Origin. " << std::endl;
		return false;
	}
	return true;
}

bool validator::ConstraintsLoading::checkOverlapping(Item& item, Tour& tour, const unsigned int& tourEndPos, const Instance& instance, const bool& msg) {
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
			if (msg) std::cerr << "Overlapping of item " << item.id << " and Item " << other.id << std::endl;
			return false;
		}
	}
	return true;
}


bool validator::ConstraintsLoading::checkUnloadingSequence(Item& item, Tour& tour, const unsigned int& tourEndPos, const UnloadingSequence& uSequence, Instance& instance, const bool& msg) {
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
					if (msg) std::cerr << "Unloading Sequence for Item " << item.id << " not respected due to Item " << other.id << std::endl;
					return false;
				}
				if ((uSequence == UnloadingSequence::LIFO && item.isBelow(other, false)) ||
					(uSequence == UnloadingSequence::MLIFO && item.isBelow(other, true))) {
					if (msg) std::cerr << "Unloading Sequence for Item " << item.id << " not respected due to Item " << other.id << std::endl;
					return false;
				}
			}	
		}
	}
	return true;
}

bool validator::ConstraintsLoading::checkReachability(Item& item, Tour& tour, const unsigned int& lambda, const bool& reachability, Instance& instance, const bool& msg) {
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
				maxX = std::max(maxX, other.max.x);
			}
		}
		// Check Reachability
		if (maxX - item.max.x > lambda) {
			if (msg) std::cerr << "Not reachable Item " << item.id << std::endl;
			return false;
		}
	}
	return true;
}

bool validator::ConstraintsLoading::checkStacking(Item& item, Tour& tour, const unsigned int& tourEndPos, const Stacking& stacking, Instance& instance, const bool& msg) {
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

bool validator::ConstraintsLoading::checkFragility(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance, const bool& msg) {
	for (size_t i = 0; i < tourEndPos; ++i) {
		const auto& item_id = tour.item_ids.at(i);
		auto& other = instance.items.at(item_id);
		if (other.isBelow(item, true) && other.fragility && !item.fragility) {
			if (msg) std::cerr << "Item " << item.id << " is placed on top of fragile item " << other.id << std::endl;
			return false;
		}
	}
	return true;
}

bool validator::ConstraintsLoading::checkVStability(Item& item, const VerticalStability& vStability, const float& alpha, Instance& instance, const bool& msg) {
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
	case VerticalStability::StaticStability:
		return checkStaticStability(item, instance, msg);
	case VerticalStability::none:
		return checkMinimalSupport(item, 0.1f, instance, msg);
	}
}

bool validator::ConstraintsLoading::checkMinimalSupport(Item& item, const float& alpha, Instance& instance, const bool& msg) {
	calcItemSupport(item, instance);

	// Check Support
	if (item.support < alpha * 0.99) {
		if (msg) std::cerr << "Item " << item.id << " not enough supported." << std::endl;
		return false;
	}
	return true;
}

bool validator::ConstraintsLoading::checkMultipleOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg) {
	double min_support = 1.0;
	// Item a is one level
	for (const auto& a_id : item.itemsBelow) {
		Item& aBelow = instance.items.at(a_id);
		double supportArea = calcSupportArea(item, aBelow);

		// Search for Items b, which lay in the same level of item a
		for (const auto& b_id : item.itemsBelow) {
			if (a_id == b_id) continue;
			Item& bBelow = instance.items.at(b_id);

			// If item b lies in the same level of item a
			if (bBelow.max.z >= aBelow.max.z && bBelow.min.z < aBelow.max.z)
				supportArea += calcSupportArea(bBelow, item);
			}
			double support = supportArea / ((double) item.l * item.w);
			min_support = std::min(support, min_support);
	}
	if (min_support < alpha * 0.99) {
		if (msg) std::cerr << "Item " << item.id << " not enough supported." << std::endl;
		return false;
	}
	return true;
}

bool validator::ConstraintsLoading::checkTopOverhanging(Item& item, const float& alpha, Instance& instance, const bool& msg) {
	if (item.support == 0) {
		calcItemSupport(item, instance);
	}

	// Last Item of Stack
	if (item.itemsAbove.size() == 0 && item.support < alpha * 0.99) {
		if (msg) std::cerr << "Item " << item.id << " of Customer " << item.customer_id << " not enough supported." << std::endl;
		return false;
	}
	
	// Other item can hang over current item
	for (const auto& otherId : item.itemsAbove) {
		auto& other = instance.items.at(otherId);		
		if (other.isAbove(item, 1) && item.support < 0.99) {
			if (msg) std::cerr << "Item " << item.id << " of Customer " << item.customer_id << " not enough supported." << std::endl;
			return false;
		}
	}

	return true;
}

bool validator::ConstraintsLoading::checkStaticStability(Item& item, Instance& instance, const bool& msg) {
	const double x = 0.5 * (item.max.x - item.min.x) + item.min.x;
	const double y = 0.5 * (item.max.y - item.min.y) + item.min.y;

	for (const auto& a_id : item.itemsBelow) {		
		Item& aBelow = instance.items.at(a_id);
		unsigned int minX = aBelow.min.x;
		unsigned int maxX = aBelow.max.x;
		unsigned int minY = aBelow.min.y;
		unsigned int maxY = aBelow.max.y;

		// Search for Items b, which lay in the same level of item a
		for (auto b_id : item.itemsBelow) {
			if (a_id == b_id) continue;
			Item& bBelow = instance.items.at(b_id);

			// If item b lies in the same level of item a
			if (bBelow.max.z >= aBelow.max.z && bBelow.min.z < aBelow.max.z) {
				minX = std::min(minX, bBelow.min.x);
				maxX = std::max(maxX, bBelow.max.x);
				minY = std::min(minY, bBelow.min.y);
				maxY = std::max(maxY, bBelow.max.y);
			}
		}
		
		if (x <= minX || x >= maxX || y <= minY || y >= maxY) {
			if (msg) std::cerr << "Item " << item.id << " not enough supported." << std::endl;
			return false;
		}
	}
	return true;
}


void validator::ConstraintsLoading::calcItemSupport(Item& item, Instance& instance) {
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
}


int validator::ConstraintsLoading::calcSupportArea(const Item& current, const Item& other) {
	const unsigned int minX = std::max(current.min.x, other.min.x);
	const unsigned int minY = std::max(current.min.y, other.min.y);
	const unsigned int maxX = std::min(current.max.x, other.max.x);
	const unsigned int maxY = std::min(current.max.y, other.max.y);
	const int area = (maxX - minX) * (maxY - minY);

	return std::max(area, 0);
}

bool validator::ConstraintsLoading::checkLBS(Item& below, Item& above, double weight, Instance& instance, const Stacking& stacking, const bool& msg) {
	if (above.support == 0) {
		calcItemSupport(above, instance);
	}

	// Get Support Area between above Item and below Item
	const int minY = std::max(above.min.y, below.min.y) - below.min.y;
	const int minX = std::max(above.min.x, below.min.x) - below.min.x;
	const int maxY = std::min(above.max.y, below.max.y) - below.min.y;
	const int maxX = std::min(above.max.x, below.max.x) - below.min.x;
	const int supportArea = (maxY - minY) * (maxX - minX);
	
	// Calculate part of support by above Item
	double support_below = (double) supportArea / ((double)above.l * above.w);
	support_below /= above.support;

	// Calculate part of load
	const double load_part   = weight * support_below;
	const double loadPerArea = load_part / (double) supportArea;

	// Check if Load per Area Unit exceeds in general the limit
	if (loadPerArea > below.lbs) return false;

	// Save total load
	below.sumLoad += load_part;

	// Check Load per Area Unit
	for (auto a = minX; a < maxX; ++a) { // Row
		for (auto b = minY; b < maxY; ++b) { // Column
			// Determine Square
			const auto square = b + (below.max.y - below.min.y) * a;

			// Save Load for the Square
			below.load.at(square) += loadPerArea;

			// Check Load
			if (below.load.at(square) > below.lbs)	
				return false;
		}
	}

	// Recursiv Call
	if (stacking == Stacking::LBSComplete) {
		return checkLBSComplete(below, instance, load_part, msg);
	}
	return true;
}

bool validator::ConstraintsLoading::checkLBSComplete(Item& item, Instance& instance, double load, const bool& msg) {
	for (auto& below_id : item.itemsBelow) {
		Item& below = instance.items.at(below_id);

		// Consider items with direct support
		if (below.max.z != item.min.z) continue;

		// Call CalcLBS with adapted load
		if (!checkLBS(below, item, load, instance, Stacking::LBSComplete, msg)) {
			if (msg) std::cerr << "Load exceeded for item " << below.id << std::endl;
			return false;
		}
	}
	return true;
}

bool validator::ConstraintsLoading::checkLBSSimple(Item& item, Instance& instance, const bool& msg) {
	for (const auto& below_id : item.itemsBelow) {
		Item& below = instance.items.at(below_id);

		// Call Calc_LBS with mass of above item
		if (!checkLBS(below, item, item.mass, instance, Stacking::LBSSimple, msg)) {
			if (msg) std::cerr << "Load exceeded for item " << below.id << std::endl;
			return false;
		}
	}

	return true;
}


bool validator::ConstraintsLoading::checkAxleWeights(Item& item, Tour& tour, const bool& axle_weights, const Instance& instance, const bool& msg) {
	if (axle_weights) {
		const double g = 9.81;
		long item_T = 0;

		// Calculate Item's Force
		const double item_F = item.mass * g;
		const double length = !item.rotated ? item.l : item.w;
		double s = 0;

		long force_rear_axle, force_front_axle, force_trailer_axle = 0;

		if (!instance.vehicle.axle_trailer) {

			// Calculate Item's Torque
			s = (double) instance.vehicle.l_FA_cargo + item.min.x + 0.5 * length;
			item_T = s * item_F;

			const int l_truck_FA = instance.vehicle.wheelbase - instance.vehicle.l_tractor_RA;

			// Calculate Axle Weights
			force_rear_axle = (tour.sum_T + item_T + instance.vehicle.mass_tractor * g * l_truck_FA) / instance.vehicle.wheelbase;
			
			force_front_axle = (tour.sum_F + item_F + instance.vehicle.mass_tractor * g) - force_rear_axle;
		}
		
		else {
			// Calculate Item's Torque
			s = (double) instance.vehicle.l_cargo_TA - item.min.x - 0.5 * length;
			item_T = s * item_F;
			
			// Calculate Axle Weights
			const long force_kingpin = (1.0 / instance.vehicle.l_kingpin_TA) * (tour.sum_T + item_T + instance.vehicle.mass_trailer * g * instance.vehicle.l_trailer_TA);

			force_trailer_axle = tour.sum_F + item_F + instance.vehicle.mass_trailer * g - force_kingpin;

			force_front_axle = (1.0 / instance.vehicle.wheelbase) * (force_kingpin * instance.vehicle.l_kingpin_RA + instance.vehicle.mass_tractor * g * instance.vehicle.l_tractor_RA);

			force_rear_axle = force_kingpin + instance.vehicle.mass_tractor * g - force_front_axle;
		}

		tour.sum_F += item_F;
		tour.sum_T += item_T;

		// Check Axle Weight Limits
		if (force_rear_axle > instance.vehicle.lim_rearAxle * g) {
			if (msg) std::cerr << "Overloaded Rear Axle of Tour " << tour.id << std::endl;
			return false;
		}

		if (force_front_axle > instance.vehicle.lim_frontAxle * g) {
			if (msg) std::cerr << "Overloaded Front Axle of Tour " << tour.id << std::endl;
			return false;
		}

		if (force_trailer_axle > instance.vehicle.lim_trailerAxle * g) {
			if (msg) std::cerr << "Overloaded Trailer Axle of Tour " << tour.id << std::endl;
			return false;
		}
	}
  	
	
	return true;
}


bool validator::ConstraintsLoading::checkBalancedLoading(Item& item, Tour& tour, const bool& balanced_loading, const float& balanced_part, const Instance& instance, const bool& msg) {
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
			if (msg) std::cerr << "Overloaded Left Vehicle Half of Tour" << tour.id << std::endl;
			return false;
		}

		if (tour.mass_R > limit) {
			if (msg) std::cerr << "Overloaded Right Vehicle Half of Tour " << tour.id << std::endl;
			return false;
		}
	}
	return true;
}


void validator::ConstraintsLoading::getRelevantItems(Item& item, Tour& tour, const unsigned int& tourEndPos, Instance& instance) {
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

int validator::ConstraintsLoading::getScaleFactor(const unsigned int& vehicleH) {
	// Scale Lambda
	unsigned int digits = log10(vehicleH) + 1;
	return pow(10, digits - 2);	
}