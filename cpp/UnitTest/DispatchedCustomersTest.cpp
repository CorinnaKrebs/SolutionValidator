#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsRouting.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace RoutingConstraints {
	TEST_CLASS(DispatchedCustomersTest) {
	public:
		TEST_METHOD(Customer1Twice) {
			Customer     depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer1(1, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer2(2, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer3(3, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer4(4, 0, 0, 0, 0, 0, 0, 0, 0);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer1, customer2, customer3, customer4 };
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 3, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids1{ 2, 3 };
			std::vector<unsigned int> customer_ids2{ 1, 4};
			std::vector<unsigned int> customer_ids3{ 1 };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids1, item_ids);
			Tour tour2(2, customer_ids2, item_ids);
			Tour tour3(3, customer_ids3, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1, tour2, tour3 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(false, ConstraintsRouting::checkDispatchedCustomers(solution, false, instance, false));
			Assert::AreEqual(true, ConstraintsRouting::checkDispatchedCustomers(solution, true, instance, false));
		}

		TEST_METHOD(Customer1NotDispatched) {
			Customer     depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer1(1, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer2(2, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer3(3, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer4(4, 0, 0, 0, 0, 0, 0, 0, 0);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer1, customer2, customer3, customer4 };
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 3, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids1{ 2, 3 };
			std::vector<unsigned int> customer_ids2{ 4 };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids1, item_ids);
			Tour tour2(2, customer_ids2, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1, tour2 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(false, ConstraintsRouting::checkDispatchedCustomers(solution, false, instance, false));
			Assert::AreEqual(false, ConstraintsRouting::checkDispatchedCustomers(solution, true, instance, false));
		}

		TEST_METHOD(TourOnlyDepot) {
			Customer     depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer1(1, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer2(2, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer3(3, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer4(4, 0, 0, 0, 0, 0, 0, 0, 0);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer1, customer2, customer3, customer4 };
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 3, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids1{ 2, 3 };
			std::vector<unsigned int> customer_ids2{ 1, 4 };
			std::vector<unsigned int> customer_ids3{ 0 };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids1, item_ids);
			Tour tour2(2, customer_ids2, item_ids);
			Tour tour3(3, customer_ids3, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1, tour2, tour3 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(false, ConstraintsRouting::checkDispatchedCustomers(solution, false, instance, false));
		}

		TEST_METHOD(Feasible) {
			Customer     depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer1(1, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer2(2, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer3(3, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer4(4, 0, 0, 0, 0, 0, 0, 0, 0);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer1, customer2, customer3, customer4 };
			std::vector<ItemType> itemtypes{ };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 3, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids1{ 2, 3 };
			std::vector<unsigned int> customer_ids2{ 1, 4 };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids1, item_ids);
			Tour tour2(2, customer_ids2, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1, tour2 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(true, ConstraintsRouting::checkDispatchedCustomers(solution, false, instance, false));
		}
	};
}