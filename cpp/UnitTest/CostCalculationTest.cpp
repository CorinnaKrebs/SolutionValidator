#include "pch.h"
#include <vector>
#include "../Validator/Solution.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsRouting.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace RoutingConstraints {
    TEST_CLASS(CostCalculationTest) {
	public:
	    TEST_METHOD(CorrectCost) {
	        const int cust1_x = 10;
	        const int cust2_x = 20;
	        const int cust3_x = 25;

	        Customer depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
	        Customer customer1(1, cust1_x, 0, 0, 0, 0, 0, 0, 0);
	        Customer customer2(2, cust2_x, 0, 0, 0, 0, 0, 0, 0);
	        Customer customer3(3, cust3_x, 0, 0, 0, 0, 0, 0, 0);

	        // Instance Creation
	        std::vector<Customer> customers{ depot, customer1, customer2, customer3 };
	        std::vector<ItemType> itemtypes{ };

	        Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

	        // Tours Creation
	        Tour tour1(1, { 1 }, { });
	        Tour tour2(2, { 2, 3 }, { });

	        double distance01 = ConstraintsRouting::calcEuclideanDistance(depot.x, depot.y, customer1.x, customer1.y);
	        double distance02 = ConstraintsRouting::calcEuclideanDistance(depot.x, depot.y, customer2.x, customer2.y);
	        double distance23 = ConstraintsRouting::calcEuclideanDistance(customer2.x, customer2.y, customer3.x, customer3.y);
	        double distance30 = ConstraintsRouting::calcEuclideanDistance(customer3.x, customer3.y, depot.x, depot.y);

	        double cost = 2 * distance01 + distance02 + distance23 + distance30;

	        // Solution Creation
	        std::vector<Tour> tours = { tour1, tour2 };
	        Solution solution(tours);
	        solution.total_travel_distance = cost;
	        Assert::IsTrue(ConstraintsRouting::checkCost(solution, instance));
	    }
	    
	    TEST_METHOD(WrongCost) {
	        const int cust1_x = 10;
	        const int cust2_x = 20;
	        const int cust3_x = 25;

	        Customer depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
	        Customer customer1(1, cust1_x, 0, 0, 0, 0, 0, 0, 0);
	        Customer customer2(2, cust2_x, 0, 0, 0, 0, 0, 0, 0);
	        Customer customer3(3, cust3_x, 0, 0, 0, 0, 0, 0, 0);

	        // Instance Creation
	        std::vector<Customer> customers{ depot, customer1, customer2, customer3 };
	        std::vector<ItemType> itemtypes{ };

	        Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

	        // Tours Creation
	        Tour tour1(1, { 1 }, { });
	        Tour tour2(2, { 2, 3 }, { });

	        // Solution Creation
	        std::vector<Tour> tours = { tour1, tour2 };
	        Solution solution(tours);
	        solution.total_travel_distance = 1;
	        Assert::IsFalse(ConstraintsRouting::checkCost(solution, instance));
	    }    	
    };
}
