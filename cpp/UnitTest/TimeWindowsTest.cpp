#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsRouting.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace RoutingConstraints {
	TEST_CLASS(TimeWindowsTest) {
	public:

		TEST_METHOD(DistanceCalculation) {
			unsigned int x1 = 7;
			unsigned int x2 = 10;

			unsigned int y1 = 2;
			unsigned int y2 = 6;

			double expected = 5; // sqrt(4² + 3²) = sqrt(16 + 9) = sqrt(25)
			Assert::AreEqual(expected, ConstraintsRouting::calcEuclideanDistance(x1, y1, x2, y2));
		}
		TEST_METHOD(TWCalculation) {
			double time = 0;
			unsigned int depot_ST = 15, depot_DD = 50;
			unsigned int cust_x   = 10, cust_RT  = 30, cust_DD = 50, cust_ST = 10;

			// Instance Creation
			Customer    depot(0, 0,      0, 0, 0,       depot_DD, depot_ST, 0, 0);
			Customer customer(1, cust_x, 0, 0, cust_RT, cust_DD,  cust_ST,  0, 0);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer };
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 1);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{};
			Tour tour(1, customer_ids, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour };
			Solution solution(tours);

			double distance01 = ConstraintsRouting::calcEuclideanDistance(0, 0, cust_x, 0);
			Assert::AreEqual(10.0, distance01);

			unsigned int time_Diff = cust_RT - distance01 - depot_ST;
			double expected = depot_ST + distance01 + time_Diff + cust_ST;
			Assert::AreEqual(true, ConstraintsRouting::checkTimeWindowCustomer(1, 0, time, instance, false));
			Assert::AreEqual(expected, time);
			Assert::AreEqual(true, ConstraintsRouting::checkTimeWindows(solution, instance, false));


			// Add another customer and re-check Calculation
			unsigned int cust2_x  = 20, cust2_RT = 30, cust2_DD = 60, cust2_ST = 15;
			Customer customer2(2, cust2_x, 0, 0, cust2_RT, cust2_DD, cust2_ST, 0, 0);
			instance.customers.emplace_back(customer2);
			double distance12 = ConstraintsRouting::calcEuclideanDistance(cust_x, 0, cust2_x, 0);
			expected += distance12 + cust2_ST;

			time = 0;
			Assert::AreEqual(true, ConstraintsRouting::checkTimeWindowCustomer(1, 0, time, instance, false));
			Assert::AreEqual(true, ConstraintsRouting::checkTimeWindowCustomer(2, 1, time, instance, false));
			Assert::AreEqual(expected, time);
		}


		TEST_METHOD(ExceedingTW) {
			unsigned int depot_ST = 15;
			unsigned int depot_DD = 10; // Not Feasible

			unsigned int cust_x = 10;
			unsigned int cust_RT = 30;
			unsigned int cust_DD = 50;
			unsigned int cust_ST = 10;

			// Instance Creation
			Customer    depot(0, 0, 0, 0, 0, depot_DD, depot_ST, 0, 0);
			Customer customer(1, cust_x, 0, 0, cust_RT, cust_DD, cust_ST, 0, 0);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer};
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 1);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{};
			Tour tour(1, customer_ids, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour };
			Solution solution(tours);

			// Test
			Assert::AreEqual(false, ConstraintsRouting::checkTimeWindows(solution, instance, false));
		}
	};
}