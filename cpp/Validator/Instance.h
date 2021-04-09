#pragma once
#include <vector>
#include "Customer.h"
#include "ItemType.h"
#include "Item.h"
#include "Vehicle.h"

namespace validator {

	class Instance {

	public:
		// Constructor
		Instance(const std::string name, const Vehicle& vehicle, const std::vector<ItemType>& itemTypes, const std::vector<Customer>& customers, const unsigned int v_max, const bool tw)
			: name(name), vehicle(vehicle), itemTypes(itemTypes), customers(customers), v_max(v_max), tw(tw) {}

		const std::string			name;		// Instance Name
		const Vehicle				vehicle;	// Description of Vehicle Type, Homogeneous Fleet
		const std::vector<ItemType> itemTypes;	// Contains all ItemTypes
		std::vector<Customer>		customers;	// Contains all customers
		const unsigned int			v_max;		// Maximal Number of available Vehicles
		const bool					tw;			// Consideration of Time Windows

		std::vector<Item>			items;		// Contains all items and their position
	};
}