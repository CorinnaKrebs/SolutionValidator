#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsRouting.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace RoutingConstraints {
	TEST_CLASS(DispatchedItemsTest) {
	public:
		TEST_METHOD(Demand2NotFulfilled) {
			ItemType type1(1, 0, 0, 0, 0, false, 0);
			ItemType type2(2, 0, 0, 0, 0, false, 0);

			Item item1(0, 1, 0, type1);
			Item item2(1, 1, 0, type1);
			Item item3(2, 1, 0, type1);

			Item item4(3, 1, 0, type2);
			Item item5(4, 1, 0, type2);

			// Instance Creation
			Customer depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer(1, 0, 0, 5, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ depot, customer };
			std::vector<ItemType> itemtypes{ type1, type2 };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);
			instance.items.emplace_back(item4);
			instance.items.emplace_back(item5);
			std::pair<int, int> demand1(1, 3);
			std::pair<int, int> demand2(2, 3);
			instance.customers.at(1).demands.emplace_back(demand1);
			instance.customers.at(1).demands.emplace_back(demand2);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2, 3, 4  };
			Tour tour(1, customer_ids, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour };
			Solution solution(tours);

			// Test
			Assert::AreEqual(false, ConstraintsRouting::checkDispatchedItems(solution, instance, false));
		}

		TEST_METHOD(DemandFulfilled) {
			ItemType type1(1, 0, 0, 0, 0, false, 0);
			ItemType type2(2, 0, 0, 0, 0, false, 0);

			Item item1(0, 1, 0, type1);
			Item item2(1, 1, 0, type1);
			Item item3(2, 1, 0, type1);

			Item item4(3, 1, 0, type2);
			Item item5(4, 1, 0, type2);

			// Instance Creation
			Customer depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer(1, 0, 0, 5, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ depot, customer };
			std::vector<ItemType> itemtypes{ type1, type2 };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);
			instance.items.emplace_back(item4);
			instance.items.emplace_back(item5);
			std::pair<int, int> demand1(1, 3);
			std::pair<int, int> demand2(2, 2);
			instance.customers.at(1).demands.emplace_back(demand1);
			instance.customers.at(1).demands.emplace_back(demand2);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2, 3, 4 };
			Tour tour(1, customer_ids, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour };
			Solution solution(tours);

			// Test
			Assert::AreEqual(true, ConstraintsRouting::checkDispatchedItems(solution, instance, false));
		}
	};
}