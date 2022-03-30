#include <fstream>
#include <string>
#include <iomanip>
#include "Write.h"
#include "Instance.h"
#include "Solution.h"
#include "ConstraintSet.h"



void validator::Write::instanceFile(const std::string path, Instance& instance) {
	std::ofstream outputFile(path + instance.name + ".txt");

	outputFile << "Name\t\t\t\t" << instance.name << std::endl;
	outputFile << "Number_of_Customers\t\t" << instance.customers.size() - 1 << std::endl;
	outputFile << "Number_of_Items\t\t\t"   << instance.items.size() - 1     << std::endl;
	outputFile << "Number_of_ItemTypes\t\t" << instance.itemTypes.size() - 1 << std::endl;
	outputFile << "Number_of_Vehicles\t\t"  << instance.v_max                << std::endl;
	outputFile << "TimeWindows\t\t\t"       << instance.tw                   << std::endl;

	outputFile << std::endl;

	const Vehicle& vehicle = instance.vehicle;
	outputFile << "VEHICLE" << std::endl;
	outputFile << "Mass_Capacity\t\t\t"		<< vehicle.D << std::endl;
	outputFile << "CargoSpace_Length\t\t"	<< vehicle.l << std::endl;
	outputFile << "CargoSpace_Width\t\t"	<< vehicle.w << std::endl;
	outputFile << "CargoSpace_Height\t\t"	<< vehicle.h << std::endl;
	outputFile << "Wheelbase\t\t\t"			<< vehicle.wheelbase << std::endl;
	outputFile << "Max_Mass_FrontAxle\t\t"	<< vehicle.lim_frontAxle << std::endl;
	outputFile << "Max_Mass_RearAxle\t\t"	<< vehicle.lim_rearAxle  << std::endl;
	outputFile << "Distance_FrontAxle_CargoSpace\t" << vehicle.l_FA_cargo << std::endl;

	outputFile << std::endl;

	outputFile << "CUSTOMERS" << std::endl;
	outputFile << "i\t\tx\t\ty\t\tDemand\t\tReadyTime\tDueDate\t\tServiceTime\tDemandedMass\tDemandedVolume" << std::endl;
	for (int i = 0; i < instance.customers.size(); ++i) {
		const Customer& customer = instance.customers.at(i);
		outputFile << i					<< "\t\t";
		outputFile << customer.x		<< "\t\t";
		outputFile << customer.y		<< "\t\t";
		outputFile << customer.demand	<< "\t\t";
		outputFile << customer.readyTime << "\t\t";
		outputFile << customer.dueDate	 << "\t\t";
		outputFile << customer.serviceTime  << "\t\t";
		outputFile << customer.demandedMass << "\t\t";
		outputFile << customer.demandedVolume << std::endl;
	}

	outputFile << std::endl;
	
	outputFile << "ITEMS" << std::endl;
	outputFile << "Type\t\tLength\t\tWidth\t\tHeight\t\tMass\t\tFragility\t\tLoadBearingStrength" << std::endl;
	for (int i = 1; i < instance.itemTypes.size(); ++i) {
		const ItemType& type = instance.itemTypes.at(i);
		outputFile << "Bt" << i << "\t\t" << type.dimension.l << "\t\t" << type.dimension.w << "\t\t" << type.dimension.h << "\t\t";
		outputFile << type.mass			<< "\t\t";
		outputFile << type.fragility	<< "\t\t\t";
		outputFile << type.lbs			<< std::endl;
	}

	outputFile << std::endl;
	
	outputFile <<"DEMANDS PER CUSTOMER" << std::endl;
	outputFile << "i\tType Quantity"	<< std::endl;
	for (int i = 1; i < instance.customers.size(); ++i) {
		const Customer& customer = instance.customers.at(i);
		outputFile << i << "\t";
		for (int j = 0; j < customer.demands.size(); ++j) {
			const auto& demand = customer.demands.at(j);
			outputFile << "Bt" << demand.first << " " << demand.second << "  ";
		}
		outputFile << std::endl;
	}

	outputFile.close();
}

void validator::Write::constraintFile(const std::string path, ConstraintSet& constraintSet, const int constraintNumber) {
	std::ofstream outputFile(path + "P" + std::to_string(constraintNumber) + ".txt");
	
	outputFile << "// Parameter for the Constraints" << std::endl;
	outputFile << "alpha\t\t"		<< constraintSet.alpha			<< "\t\t// Support Parameter\t\t\tin %"				<< std::endl;
	outputFile << "lambda\t\t"		<< constraintSet.lambda			<< "\t\t// distance for reachability\t\tin dm"		<< std::endl;
	outputFile << "balanced_part\t" << constraintSet.balanced_part	<< "\t\t// Part of balanced loading\t\tin %"		<< std::endl;

	outputFile << std::endl;
	outputFile << "// (De-)activation of Constraints" << std::endl;

	outputFile << "rotation\t\t"			<< constraintSet.rotation					<< "\t// Rotation of Items\t\t(0: n, 1: y)"					<< std::endl;
	outputFile << "capacity\t\t"			<< constraintSet.load_capacity				<< "\t// Load Capacity\t\t(0: n, 1: y)"						<< std::endl;
	outputFile << "unloading_sequence\t"	<< static_cast<int>(constraintSet.uSequence	) << "\t// Unloading Sequence\t\t(0: n; 1: lifo,\t2: mlifo)"  << std::endl;
	outputFile << "vertical_stability\t"	<< static_cast<int>(constraintSet.vStability) << "\t// Vertical Stability\t\t(0: n, 1: minimal support area,\t2: robust stability (multiple overhanging),\t3: robust stability 2 (top overhanging))" << std::endl;
	outputFile << "stacking\t\t"			<< static_cast<int>(constraintSet.stacking)  << "\t// Stacking Constraints\t\t(0: n, 1: fragility,\t2: LBS Simplified,\t3: LBS Complete)" << std::endl;
	outputFile << "reachability\t\t"		<< constraintSet.reachability				<< "\t// Reachability\t\t\t(0: n, 1: y)"				<< std::endl;
	outputFile << "axle_weights\t\t"		<< constraintSet.axle_weights				<< "\t// Axle Weight Constraint\t(0: n, 1: y)"			<< std::endl;
	outputFile << "balancing\t\t"			<< constraintSet.balanced_loading			<< "\t// Balanced Loading\t\t(0: n, 1: y)"				<< std::endl;

	outputFile.close();
}

void validator::Write::solutionFile(const std::string path, Solution& solution, Instance& instance, const int constraintNo, const int seed) {
	std::ofstream outputFile(path + instance.name + "_P" + std::to_string(constraintNo) + "_" + std::to_string(seed) + ".txt");
	outputFile << "Name:\t\t\t\t" + instance.name << std::endl;
	outputFile << "Problem:\t\t\t";
	const std::string problem = instance.tw ? "3L-VRPTW" : "3L-CVRP";
	outputFile << problem << std::endl;

	outputFile << "Number_of_used_Vehicles:\t" << solution.tours.size() << std::endl;
	outputFile << "Total_Travel_Distance:\t\t" << solution.total_travel_distance << std::endl;
	outputFile << "Calculation_Time:\t\t" << solution.calculation_time << std::endl;
	outputFile << "Total_Iterations:\t\t" << solution.total_iterations << std::endl;
	outputFile << "ConstraintSet:\t\t\t" << "P" << std::to_string(constraintNo) << std::endl;

	outputFile << std::endl;

	std::string breakLine = "-------------------------------------------------------------------------------------------------------------------\n";

	int id = 1;
	for (auto& tour : solution.tours) {
		if (tour.customer_ids.empty()) {
			continue;
		}
		size_t noItems = 0;
		for (const auto& customerId : tour.customer_ids) {
			Customer& customer = instance.customers.at(customerId);
			noItems += customer.item_ids.size();
		}
		outputFile << breakLine;
		outputFile << "Tour_Id:\t\t\t" << std::to_string(id++) << std::endl;
		outputFile << "No_of_Customers:\t\t" << std::to_string(tour.customer_ids.size()) << std::endl;
		outputFile << "No_of_Items:\t\t\t" << std::to_string(noItems) << std::endl;
		outputFile << "Customer_Sequence:\t\t";
		for (const auto& customerId : tour.customer_ids) {
			outputFile << std::to_string(customerId) + " ";
		}
		outputFile << std::endl << std::endl;
		if (tour.customer_ids.size() > 1) {
			outputFile << "CustId\tId\tTypeId\tRotated\tx\ty\tz\tLength\tWidth\tHeight\tmass\tFragility\tLoadBearingStrength" << std::endl;
			for (const auto& itemId : tour.item_ids) {
				auto& item = instance.items.at(itemId);
				outputFile << std::to_string(item.customer_id) << "\t" << itemId << "\t" << std::to_string(item.type_id) << "\t" << std::to_string(item.rotated) << "\t";
				outputFile << std::to_string(item.min.x) << "\t" << std::to_string(item.min.y) << "\t" << std::to_string(item.min.z) << "\t" << std::to_string(item.dimension.l) << "\t" << std::to_string(item.dimension.w) << "\t" << std::to_string(item.dimension.h) << "\t";
				outputFile << std::fixed << std::setprecision(2) << item.mass << "\t" << std::to_string(item.fragility) << "\t\t";
				outputFile << std::fixed << std::setprecision(6) << item.lbs << std::endl;
			}

			outputFile << std::endl;
			outputFile << std::endl;
		}
	}
	outputFile.close();

}