#include <fstream>
#include <string>
#include <iomanip>
#include "Write.h"
#include "Instance.h"
#include "Solution.h"

void validator::Write::writePackingPlan(const std::string path, Solution& solution, Instance& instance, const int constraintNo, const int seed) {
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
		int noItems = 0;
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
				outputFile << std::to_string(item.min.x) << "\t" << std::to_string(item.min.y) << "\t" << std::to_string(item.min.z) << "\t" << std::to_string(item.l) << "\t" << std::to_string(item.w) << "\t" << std::to_string(item.h) << "\t";
				outputFile << std::fixed << std::setprecision(2) << item.mass << "\t" << std::to_string(item.fragility) << "\t\t";
				outputFile << std::fixed << std::setprecision(6) << item.lbs << std::endl;
			}

			outputFile << std::endl;
			outputFile << std::endl;
		}
	}
	outputFile.close();

}