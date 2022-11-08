#include "Read.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

namespace validator {

	// Due to const members, overloading istream operator is not possible
	Instance Read::readInstanceFile(std::string instancePath) {
		std::istringstream	iss;
		std::string			line, var;

		std::ifstream inputFile(instancePath, std::ifstream::in);

		if (!inputFile) {
			std::cerr << "Could not open Instance file " << instancePath << std::endl;
			exit(-1);
		}

		std::string name;
		unsigned int no_customers, no_items, no_types, v_max;
		bool tw;
		std::vector<Customer> customers;
		std::vector<ItemType> itemtypes;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Name")					iss >> name;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Number_of_Customers")	iss >> no_customers;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Number_of_Items")		iss >> no_items;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Number_of_ItemTypes")	iss >> no_types;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Number_of_Vehicles")	iss >> v_max;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "TimeWindows")			iss >> tw;

		getline(inputFile, line);
		getline(inputFile, line);

		Vehicle vehicle = readVehicle(inputFile);

		getline(inputFile, line);

		if (line != "CUSTOMERS") {
			std::cerr << "Wrong Line. Should be Header CUSTOMERS, current: " + line << std::endl;
		}
		getline(inputFile, line);

		// Read Customers
		for (unsigned int i = 0; i <= no_customers; ++i) {
			Customer customer = readCustomer(inputFile);
			customers.emplace_back(customer);
		}

		getline(inputFile, line);
		getline(inputFile, line);
		getline(inputFile, line);
		getline(inputFile, line);

		// Read ItemTypes
		itemtypes.emplace_back(ItemType());
		for (unsigned int i = 0; i < no_types; ++i) {
			ItemType type = readItemType(inputFile);
			itemtypes.emplace_back(type);
		}

		getline(inputFile, line);
		getline(inputFile, line);
		getline(inputFile, line);

		// Read Demands
		for (unsigned int i = 1; i <= no_customers; ++i) {
			unsigned int id;
			Customer& customer = customers.at(i);
			const unsigned int totalDemand = customers.at(i).demand;
			inputFile >> id;
			if (customer.id != id) {
				std::cerr << "Wrong Line. Expected: " << customer.id << ", read: " << id << std::endl;
			}
			for (unsigned int j = 0; j < totalDemand;) {
				std::pair<unsigned int, unsigned int> demand = readDemand(inputFile);
				customer.demands.emplace_back(demand);
				j += demand.second;
			}
		}
		Instance instance(name, vehicle, itemtypes, customers, v_max, tw);
		instance.items.resize(no_items + 1);
		return instance;
	}


	Vehicle  Read::readVehicle(std::ifstream& inputFile) {
		std::istringstream	iss;
		std::string			line, var;

		unsigned int l = 0, w = 0, h = 0, wheelbase = 0, l_f = 0, l_tractor_RA = 0;
		double		 D = 0, lim_frontAxle = 0, lim_rearAxle = 0, mass_tractor = 0;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Mass_Capacity")			iss >> D;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "CargoSpace_Length")		iss >> l;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "CargoSpace_Width")		iss >> w;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "CargoSpace_Height")		iss >> h;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Wheelbase")				iss >> wheelbase;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Max_Mass_FrontAxle")	iss >> lim_frontAxle;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Max_Mass_RearAxle")		iss >> lim_rearAxle;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Distance_FrontAxle_CargoSpace") {
			iss >> l_f;
			const unsigned int volume = l * w * h;

			getline(inputFile, line);
			if (line.empty())							return Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f);

			if (var == "Distance_Mass_Truck_RearAxle")	iss >> l_tractor_RA;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Mass_Truck")					iss >> mass_tractor;

			return Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f, l_tractor_RA, mass_tractor);

		}
		else {
			double lim_trailer = 0, mass_trailer = 0;
			unsigned int l_kingpin_RA = 0, l_kingpin_TA = 0, l_cargo_TA = 0, l_trailer_TA;

			if (var == "Max_Mass_TrailerAxle")				iss >> lim_trailer;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Distance_Kingpin_RearAxle")			iss >> l_kingpin_RA;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Distance_Kingpin_TrailerAxle")		iss >> l_kingpin_TA;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Distance_CargoSpace_TrailerAxle")	iss >> l_cargo_TA;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Distance_Mass_Tractor_RearAxle")	iss >> l_tractor_RA;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Distance_Mass_Trailer_TrailerAxle")	iss >> l_trailer_TA;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Mass_Tractor")						iss >> mass_tractor;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Mass_Trailer")						iss >> mass_trailer;

			getline(inputFile, line);

			const unsigned int volume = l * w * h;
			return Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, lim_trailer, l_kingpin_RA, l_kingpin_TA,
				l_cargo_TA, l_tractor_RA, l_trailer_TA, mass_tractor, mass_trailer);
		}
	}


	Customer  Read::readCustomer(std::ifstream& inputFile) {
		float x, y;
		double demandedMass, demandedVolume;
		unsigned int id, demand, readyTime, dueDate, serviceTime;
		inputFile >> id >> x >> y >> demand >> readyTime >> dueDate >> serviceTime >> demandedMass >> demandedVolume;
		return Customer(id, x, y, demand, readyTime, dueDate, serviceTime, demandedMass, demandedVolume);
	}


	ItemType Read::readItemType(std::ifstream& inputFile) {
		std::istringstream	iss;
		std::string			line;

		unsigned int l, w, h;
		float mass;
		bool fragility;
		double lbs;

		// Get ItemType Id
		getline(inputFile, line);
		iss.clear(); iss.str(line); iss >> line;
		std::string no = line.substr(2);
		unsigned int id = stoi(no);

		// Get other members
		iss >> l >> w >> h >> mass >> fragility >> lbs;

		return ItemType(id, l, w, h, mass, fragility, lbs);
	}


	std::pair<unsigned int, unsigned int>  Read::readDemand(std::ifstream& inputFile) {
		std::string str;
		unsigned int qt;
		inputFile >> str >> qt;
		std::string no = str.substr(2);
		unsigned int id = stoi(no);
		return std::pair<unsigned int, unsigned int>(id, qt);
	}


	ConstraintSet Read::readConstraintFile(const std::string constraintPath, const bool hasTimeWindows) {
		std::ifstream inputFile(constraintPath, std::ifstream::in);
		std::istringstream	iss;
		std::string			line, var;

		if (!inputFile) {
			std::cerr << "Could not open ConstraintSet file " << constraintPath << std::endl;
			exit(-1);
		}

		bool rotation, load_capacity, reachability, axle_weights, balanced_loading, timeWindows = hasTimeWindows, splitDelivery = false;
		unsigned int uSequence, vStability, stacking, lambda;
		float alpha, balanced_part;

		getline(inputFile, line); // Skip first Line

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "alpha")					iss >> alpha;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "lambda")				iss >> lambda;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "balanced_part")			iss >> balanced_part;

		getline(inputFile, line);
		getline(inputFile, line);

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "rotation")				iss >> rotation;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "capacity")				iss >> load_capacity;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "unloading_sequence")	iss >> uSequence;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "vertical_stability")	iss >> vStability;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "stacking")				iss >> stacking;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "reachability")			iss >> reachability;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "axle_weights")			iss >> axle_weights;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "balancing")				iss >> balanced_loading;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "TimeWindows")			iss >> timeWindows;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "SplitDelivery")			iss >> splitDelivery;



		return ConstraintSet(rotation, load_capacity, static_cast<UnloadingSequence>(uSequence), static_cast<VerticalStability>(vStability),
			static_cast<Stacking>(stacking), reachability, axle_weights, balanced_loading, alpha, lambda, balanced_part, timeWindows, splitDelivery);
	}

	Solution Read::readSolutionFile(const std::string solutionPath, Instance& instance) {
		std::ifstream inputFile(solutionPath, std::ifstream::in);
		std::istringstream	iss;
		std::string			line, var, name, problem, totalTravelDistance, calculationTime, iterations, cSet;

		if (!inputFile) {
			std::cerr << "Could not open Packplan file " << solutionPath << std::endl;
			exit(-1);
		}

		unsigned int vUsed;
		std::vector<Tour> tours;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Name:")							iss >> name;

		if (name != instance.name) {
			std::cerr << "Wrong Packing Plan found. Expected: " << instance.name << ", read: " << line;
			exit(-1);
		}

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Problem:")						iss >> problem;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Number_of_used_Vehicles:")		iss >> vUsed;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Total_Travel_Distance:")		iss >> totalTravelDistance;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Calculation_Time:")				iss >> calculationTime;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "Total_Iterations:")				iss >> iterations;

		getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
		if (var == "ConstraintSet:")				iss >> cSet;

		getline(inputFile, line);

		// Read Routes
		for (size_t r = 1; r <= vUsed; ++r) {
			unsigned int id, noItems, noCustomers, customerId;
			std::vector<unsigned int> customerIds;

			do {
				getline(inputFile, line);
			} while (!inputFile.eof() && line.find("Tour_Id") == std::string::npos);

			iss.clear(); iss.str(line); iss >> var; iss >> id;
			if (r != id) {
				std::cerr << "Wrong TourId found. Expected: " << r << ", read: " << id << std::endl;
				exit(-1);
			}

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "No_of_Customers:")		iss >> noCustomers;


			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "No_of_Items:")			iss >> noItems;

			getline(inputFile, line); iss.clear(); iss.str(line); iss >> var;
			if (var == "Customer_Sequence:") {
				for (size_t i = 0; i < noCustomers; ++i) {
					iss >> customerId;
					customerIds.emplace_back(customerId);
				}
			}

			// One Customer per Tour is always feasible
			if (noCustomers > 1) {

				getline(inputFile, line);
				getline(inputFile, line);

				// Read Items
				std::vector<unsigned int> itemIds;
				for (size_t i = 0; i < noItems; ++i) {
					getline(inputFile, line);
					Item item = readItem(line, instance);
					instance.items[item.id] = item;
					auto& customer = instance.customers.at(item.customer_id);
					itemIds.emplace_back(item.id);
					customer.item_ids.emplace_back(item.id);
				}
				tours.emplace_back(Tour(r, customerIds, itemIds));
				getline(inputFile, line);
				getline(inputFile, line);
			}
			else {
				instance.customers.at(customerId).demands.clear();
				tours.emplace_back(Tour(r, customerIds, {}));
			}
		}

		Solution solution(tours);
		solution.calculation_time = stod(calculationTime);
		solution.total_iterations = stoi(iterations);
		solution.feasible = true;
		solution.total_travel_distance = stod(totalTravelDistance);
		return solution;
	}

	Item  Read::readItem(const std::string line, Instance& instance) {
		std::istringstream	iss;

		unsigned int id, type_id, customer_id, intRotation;

		unsigned int x, y, z;

		iss.clear(); iss.str(line);
		iss >> customer_id >> id >> type_id >> intRotation >> x >> y >> z;

		Rotation rotation = static_cast<Rotation>(intRotation);
		const ItemType& itemtype = instance.itemTypes.at(type_id);

		Item item(id, customer_id, rotation, itemtype);
		item.setPosition(Point(x, y, z));

		return item;
	}

}