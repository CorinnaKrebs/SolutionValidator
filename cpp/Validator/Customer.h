#pragma once
#include "Item.h"
#include "Point.h"
#include <istream>
#include <vector>

namespace validator {

	class Customer {

	public:
		Customer(const unsigned int id, const float x, const float y, const unsigned int demand, const unsigned int readyTime, const unsigned int dueDate, const unsigned int serviceTime, const double demandedMass, const double demandedVolume)
			: id(id), x(x), y(y), demand(demand), readyTime(readyTime), dueDate(dueDate), serviceTime(serviceTime), demandedMass(demandedMass), demandedVolume(demandedVolume) {}


		// Members
		const unsigned int id;					// Unique Id
		const float x;							// x-Position of Customer
		const float y;							// y-Position of Customer
		const unsigned int demand;				// Total Demand of Items
		const unsigned int readyTime;			// ReadyTime
		const unsigned int dueDate;				// DueDate
		const unsigned int serviceTime;			// ServiceTime
		const double demandedMass;				// Total Mass   of all demanded Items
		const double demandedVolume;			// Total Volume of all demanded Items
		std::vector<unsigned int> item_ids;		// All demanded Item Ids
		std::vector <std::pair<unsigned int, unsigned int>> demands; // Pair < ItemType Id, Quantity >
	};
}