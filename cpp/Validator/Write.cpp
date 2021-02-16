#include <fstream>
#include <string>
#include <iomanip>
#include "Write.h"
#include "Instance.h"
#include "Solution.h"
using namespace std;

void Write::writePackingPlan(const std::string path, Solution& solution, Instance& instance, const int constraintNo, const int seed) {
	ofstream outputFile(path + instance.name + "_P" + to_string(constraintNo) + "_" + to_string(seed) + ".txt");
	outputFile << "Name:\t\t\t\t" + instance.name << endl;
	outputFile << "Problem:\t\t\t";
	const string problem = instance.tw ? "3L-VRPTW" : "3L-CVRP";
	outputFile << problem << endl;

	outputFile << "Number_of_used_Vehicles:\t" << solution.tours.size() << endl;
	outputFile << "Total_Travel_Distance:\t\t" << solution.total_travel_distance << endl;
	outputFile << "Calculation_Time:\t\t" << solution.calculation_time << endl;
	outputFile << "Total_Iterations:\t\t" << solution.total_iterations << endl;
	outputFile << "ConstraintSet:\t\t\t" << "P" << to_string(constraintNo) << endl;

	outputFile << endl;

	string breakLine = "-------------------------------------------------------------------------------------------------------------------\n";

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
		outputFile << "Tour_Id:\t\t\t" << to_string(id++) << endl;
		outputFile << "No_of_Customers:\t\t" << to_string(tour.customer_ids.size()) << endl;
		outputFile << "No_of_Items:\t\t\t" << to_string(noItems) << endl;
		outputFile << "Customer_Sequence:\t\t";
		for (const auto& customerId : tour.customer_ids) {
			outputFile << to_string(customerId) + " ";
		}
		outputFile << endl << endl;
		if (tour.customer_ids.size() > 1) {
			outputFile << "CustId\tId\tTypeId\tRotated\tx\ty\tz\tLength\tWidth\tHeight\tmass\tFragility\tLoadBearingStrength" << endl;
			for (const auto& itemId : tour.item_ids) {
				auto& item = instance.items.at(itemId);
				outputFile << to_string(item.customer_id) << "\t" << itemId << "\t" << to_string(item.type_id) << "\t" << to_string(item.rotated) << "\t" << to_string(item.min.x) << "\t" << to_string(item.min.y) << "\t" << to_string(item.min.z) << "\t" << to_string(item.l) << "\t" << to_string(item.w) << "\t" << to_string(item.h) << "\t";
				outputFile << fixed << std::setprecision(2) << item.mass << "\t" << to_string(item.fragility) << "\t\t";
				outputFile << fixed << std::setprecision(6) << item.lbs << endl;
			}

			outputFile << std::endl;
			outputFile << endl;
		}
	}
	outputFile.close();

}