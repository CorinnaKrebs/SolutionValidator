#pragma once
#include <string>
#include "Instance.h"
#include "Solution.h"
#include "Customer.h"
#include "Vehicle.h"
#include "ConstraintSet.h"

namespace validator {
	/**
	* The type Read.
	*/
	class Read {
	public:
		/**
		* Read the instance file.
		* Get Customers, Items, ItemTypes and Vehicles.
		*
		* @param instancePath the path to the instance file
		* @return the instance
		*/
		static Instance		 readInstanceFile(const std::string instancePath);

		/**
		* Read solution file.
		*
		* @param solutionPath the path to the solution file
		* @param instance     the corresponding instance
		* @return the solution
		*/
		static Solution		 readSolutionFile(const std::string solutionPath, Instance& instance);

		/**
		* Read constraint file.
		*
		*
		* @param constraintPath the path to the constraint file.
		* @return the constraint set
		*/
		static ConstraintSet readConstraintFile(const std::string constraintPath);

	private:

		/**
		* Read a Customer.
		* Get Customer Data (coordinates, total demand, demanded volume, demanded mass)
		*
		* @param inputFile the file to read
		* @return the Customer
		*/
		static Customer readCustomer(std::ifstream& inputFile);

		/**
		* Read the Vehicle.
		* Get Vehicle Data (e.g. Capacity, Length, Width, Height, Axle Weight Parameters)
		*
		* @param inputFile the file to read
		* @return the Vehicle
		*/
		static Vehicle  readVehicle(std::ifstream& inputFile);

		/**
		* Read an ItemType.
		* Get ItemType Data (e.g. Dimensions, Fragility Flag, LBS Values)
		*
		* @param inputFile the file to read
		* @return the Customer
		*/
		static ItemType readItemType(std::ifstream& inputFile);

		/**
		* Read an Item with its position.
		*
		* @param line         the line to read
		* @param instance     the corresponding instance
		* @return the Item
		*/
		static Item     readItem(const std::string line, Instance& instance);

		/**
		* Read a Demand.
		* Get demanded ItemType and the corresponding quantity.
		*
		* @param inputFile the file to read
		* @return the Demand
		*/
		static std::pair<unsigned int, unsigned int> readDemand(std::ifstream& inputFile);
	};
}