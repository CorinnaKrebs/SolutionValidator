#pragma once
#include "Item.h"
#include "Point.h"
#include <istream>
#include <vector>

namespace validator {

	/**
	* The type Customer.
	*/
	class Customer {

	public:
		/**
		* Instantiates a new Customer.
		*
		* @param id             the unique id
		* @param x              the x-Position
		* @param y              the y-Position
		* @param demand         the total number of demanded items
		* @param readyTime      the ready time
		* @param dueDate        the due date
		* @param serviceTime    the service time
		* @param demandedMass   the total mass of all demanded items
		* @param demandedVolume the total volume of all demanded items
		*/
		Customer(const unsigned int id, const float x, const float y, const unsigned int demand, const unsigned int readyTime, const unsigned int dueDate, const unsigned int serviceTime, const double demandedMass, const double demandedVolume)
			: id(id), x(x), y(y), demand(demand), readyTime(readyTime), dueDate(dueDate), serviceTime(serviceTime), demandedMass(demandedMass), demandedVolume(demandedVolume) {}


		// Members
		/** the unique id */
		const unsigned int id;
		
		/** the x-Position */
		const float x;

		/** the y-Position */
		const float y;

		/** the total number of demanded items */
		const unsigned int demand;

		/** ready time */
		const unsigned int readyTime;

		/** due date */
		const unsigned int dueDate;

		/** service time */
		const unsigned int serviceTime;

		/** the total mass of all demanded items */
		const double demandedMass;

		/** the total volume of all demanded items */
		const double demandedVolume;

		/** all demanded Item ids */
		std::vector<unsigned int> item_ids;

		/** demand (itemType, quantity) */
		std::vector <std::pair<unsigned int, unsigned int>> demands;
	};
}