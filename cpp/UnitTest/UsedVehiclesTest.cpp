#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsRouting.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace RoutingConstraints {
	TEST_CLASS(UsedVehiclesTest) {
	public:
		TEST_METHOD(ExceedingVMax) {
			const unsigned int v_max = 1;
			// Instance Creation
			std::vector<Customer> customers{ };
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, v_max, 1);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids, item_ids);
			Tour tour2(2, customer_ids, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1, tour2 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(true, ConstraintsRouting::checkUsedVehicles(solution, instance));
		}

		TEST_METHOD(FeasibleVMax) {
			const unsigned int v_max = 2;

			// Instance Creation
			std::vector<Customer> customers{ };
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, v_max, 1);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids, item_ids);
			Tour tour2(2, customer_ids, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1, tour2 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(true, ConstraintsRouting::checkUsedVehicles(solution, instance));
		}
	};
}