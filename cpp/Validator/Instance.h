#pragma once
#include <vector>
#include "Customer.h"
#include "ItemType.h"
#include "Item.h"
#include "Vehicle.h"

namespace validator {

	/**
	 * The type Instance.
	*/
	class Instance {

	public:
		// Constructor
		/**
		* Instantiates a new Instance.
		*
		* @param name      the instance name
		* @param vehicle   the vehicle data, homogeneous fleet
		* @param itemTypes the list with all item types
		* @param customers the list with all customers
		* @param v_max     the maximal number of available vehicles
		* @param tw        the consideration of time windows
		* @param tw        the consideration of split deliveries
		*/
		Instance(const std::string name, const Vehicle& vehicle, const std::vector<ItemType>& itemTypes, const std::vector<Customer>& customers, const unsigned int v_max, const bool tw)
			: name(name), vehicle(vehicle), itemTypes(itemTypes), customers(customers), v_max(v_max), tw(tw){}

		/** the instance name */
		const std::string			name;

		/** the vehicle data, homogeneous fleet */
		const Vehicle				vehicle;

		/** the list with all item types */
		const std::vector<ItemType> itemTypes;

		/** the list with all customers */
		std::vector<Customer>		customers;

		/** the maximal number of available vehicles */
		const unsigned int			v_max;

		/** the consideration of time windows */
		const bool					tw;

		/** Contains all items and their position */
		std::vector<Item>			items;
	};
}