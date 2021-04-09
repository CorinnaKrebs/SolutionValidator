#pragma once
#include <string>
#include "Instance.h"
#include "Solution.h"
#include "Customer.h"
#include "Vehicle.h"
#include "ConstraintSet.h"

namespace validator {
	class Read {
	public:
		static Instance		 readInstanceFile(const std::string instancePath);
		static Solution		 readSolutionFile(const std::string solutionPath, Instance& instance);
		static ConstraintSet readConstraintFile(const std::string constraintPath);

	private:
		static Customer readCustomer(std::ifstream& inputFile);
		static Vehicle  readVehicle(std::ifstream& inputFile);
		static ItemType readItemType(std::ifstream& inputFile);
		static Item     readItem(const std::string line, Instance& instance);
		static std::pair<unsigned int, unsigned int> readDemand(std::ifstream& inputFile);
	};
}