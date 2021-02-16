#include "ConstraintsRouting.h"
#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

bool ConstraintsRouting::checkRoutingConstraints(Solution& solution, const ConstraintSet& cSet, Instance& instance) {
	if (!checkTimeWindows(solution, instance)
		|| !checkCapacities(solution, cSet.load_capacity, instance)
		|| !checkDispatchedCustomers(solution, instance)
		|| !checkDispatchedItems(solution, instance)
		|| !checkUsedVehicles(solution, instance)
		|| !checkCost(solution, instance)) {
		cerr << "Solution is NOT feasible according Routing Constraints." << endl;
		return false;
	}
	cout << "Solution is feasible according Routing Constraints." << endl;
	return true;
}

bool ConstraintsRouting::checkCost(const Solution& solution, const Instance& instance) {
	double totalCost = 0;
	for (Tour tour : solution.tours) {
		
		tour.customer_ids.erase(
			std::remove(tour.customer_ids.begin(), tour.customer_ids.end(), 0),
			tour.customer_ids.end());
		
		int idI = 0;
		for (int idJ : tour.customer_ids) {
			Customer customerI = instance.customers.at(idI);
			Customer customerJ = instance.customers.at(idJ);
			totalCost += calcEuclideanDistance(customerI.x, customerI.y, customerJ.x, customerJ.y);
			idI = idJ;
		}
		Customer customerI = instance.customers.at(idI);
		Customer customerJ = instance.customers.at(0);
		totalCost += calcEuclideanDistance(customerI.x, customerI.y, customerJ.x, customerJ.y);
	}
	double solCost = solution.total_travel_distance;
	if (max(totalCost * 0.999, solCost) != min(totalCost * 1.003, solCost)) {
		cerr << "Total Travel Distance is wrong" << endl;
		return false;
	}
	return true;
}


bool ConstraintsRouting::checkTimeWindows(const Solution& solution, const Instance& instance) {
	if (instance.tw) {
		auto& depot = instance.customers.at(0);
		for (auto& tour : solution.tours) {
			unsigned int customer_last_id = 0;
			double time = depot.serviceTime;
			// Check Customers
			for (auto customer_id : tour.customer_ids) {
				if (!checkTimeWindowCustomer(customer_id, customer_last_id, time, instance)) {
					return false;
				}
				customer_last_id = customer_id;
			}
			
			// Check DueDate of Depot
			const auto& customer_last = instance.customers.at(customer_last_id);
			double distance = calcEuclideanDistance(customer_last.x, customer_last.y, depot.x, depot.y);
			time += distance;
			if (time > depot.dueDate) {
				cerr << "Arrived at Depot after Due Date." << endl;
				return false;
			}
		}
	}
	return true;
}


bool ConstraintsRouting::checkTimeWindowCustomer(const unsigned int customer_id, const unsigned int customer_last_id, double& time, const Instance& instance) {
	const auto& customer_current = instance.customers.at(customer_id);
	const auto& customer_last    = instance.customers.at(customer_last_id);
	const double distance = calcEuclideanDistance(customer_last.x, customer_last.y, customer_current.x, customer_current.y);
	time += distance;
	if (time < customer_current.readyTime) {
		double waiting_time = customer_current.readyTime - time;
		time += waiting_time;
	}
	if (time > customer_current.dueDate) {
		cerr << "Arrived at Customer " << customer_id << "after Due Date." << endl;
		return false;
	}
	time += customer_current.serviceTime;
	return true;
}


double ConstraintsRouting::calcEuclideanDistance(const float ax, const float ay, const float bx, const float by) {
	auto x = ax - bx;
	auto y = ay - by;
	return sqrt(pow(x, 2) + pow(y, 2));
}


bool ConstraintsRouting::checkCapacities(const Solution& solution, const bool load_capacity, const Instance& instance) {
	for (const auto& tour : solution.tours) {
		double sumVolume = 0;
		double sumMass = 0;
		for (const auto customer_id : tour.customer_ids) {
			const Customer &customer = instance.customers.at(customer_id);
			sumVolume += customer.demandedVolume;
			sumMass += customer.demandedMass;
		}

		if (sumVolume > instance.vehicle.load_volume) {
			cerr << "Exceeding Volume in Tour " << tour.id << endl;
			return false;
		}
		if (sumMass > instance.vehicle.D && load_capacity) {
			cerr << "Exceeding Mass in Tour:  " << tour.id << endl;
			return false;
		}
	}
	return true;
}


bool ConstraintsRouting::checkDispatchedCustomers(const Solution& solution, const Instance& instance) {
	// Constraint: Each customer is visited exactly once;
	// Constraint: Each route visits at least one customer;

	set<unsigned int> dispatched_customers;
	for (const auto& tour : solution.tours) {
		if (tour.customer_ids.size() == 0 
			|| count(tour.customer_ids.begin(), tour.customer_ids.end(), 0) == tour.customer_ids.size()) {
			cerr << "Tour " << tour.id << " without customers." << endl;
			return false;
		}

		for (const auto customer_id : tour.customer_ids) {
			if (dispatched_customers.find(customer_id) != dispatched_customers.end()) {
				cerr << "Customer " << customer_id << " dispatched several times." << endl;
				return false;
			}

			dispatched_customers.insert(customer_id);
		}
	}

	if (dispatched_customers.size() < instance.customers.size() - 1) {
		cerr << "Not all Customers are dispatched." << endl;
		return false;
	}

	return  true;
}


bool ConstraintsRouting::checkDispatchedItems(const Solution& solution, const Instance& instance) {
	for (const auto& tour : solution.tours) {
		for (unsigned int customer_id : tour.customer_ids) {
			auto& customer = instance.customers.at(customer_id);
			for (auto& demand : customer.demands) {
				unsigned int typeId = demand.first;
				auto qt = count_if(tour.item_ids.begin(), tour.item_ids.end(), [typeId, customer_id, instance](const unsigned int id) {
					auto& item = instance.items.at(id);
					return item.type_id == typeId && item.customer_id == customer_id; 
					});
				if (qt != demand.second) {
					cerr << "Customer " << customer.id << ": Demand is not fulfiled. Expected items of type " << typeId << ": " << demand.second << ", dispatched: " << qt << endl;
					return false;
				}
			}
		}
	}
	return true;
}


bool ConstraintsRouting::checkUsedVehicles(const Solution& solution, const Instance& instance) {
	// Not an error since Constraints can lead to an exceedence
	if (solution.tours.size() > instance.v_max) {
		cerr << "Number of used vehicles increases number of available vehicles." << endl;
	}
	return true;
}
