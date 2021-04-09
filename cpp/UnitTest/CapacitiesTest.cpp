#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsRouting.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace RoutingConstraints {
	TEST_CLASS(CapacitiesTest) {
	public:
		TEST_METHOD(LoadVolumeCheck) {
			unsigned int max_vol = 250;
			unsigned int c1_vol = 100;
			unsigned int c2_vol = 200;

			Customer     depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer1(1, 0, 0, 0, 0, 0, 0, 0, c1_vol);
			Customer customer2(2, 0, 0, 0, 0, 0, 0, 0, c2_vol);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer1, customer2};
			std::vector<ItemType> itemtypes{ };
			
			Instance instance("", Vehicle(0, 0, 0, 0, max_vol, 0, 0, 0, 0), itemtypes, customers, 0, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids1{ 1, 2 };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids1, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(false, ConstraintsRouting::checkCapacities(solution, true, instance));
		}

		TEST_METHOD(LoadMassCheck) {
			unsigned int D = 30;
			unsigned int c1_mass = 20;
			unsigned int c2_mass = 20;

			Customer     depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer1(1, 0, 0, 0, 0, 0, 0, c1_mass, 0);
			Customer customer2(2, 0, 0, 0, 0, 0, 0, c2_mass, 0);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer1, customer2 };
			std::vector<ItemType> itemtypes{ };

			Instance instance("", Vehicle(0, 0, 0, D, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids1{ 1, 2 };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids1, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(false, ConstraintsRouting::checkCapacities(solution, true, instance));
		}

		TEST_METHOD(Feasible) {
			unsigned int D = 50;
			unsigned int c1_mass = 20;
			unsigned int c2_mass = 20;

			unsigned int max_vol = 500;
			unsigned int c1_vol = 100;
			unsigned int c2_vol = 200;

			Customer     depot(0, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customer1(1, 0, 0, 0, 0, 0, 0, c1_mass, c1_vol);
			Customer customer2(2, 0, 0, 0, 0, 0, 0, c2_mass, c2_vol);

			// Instance Creation
			std::vector<Customer> customers{ depot, customer1, customer2 };
			std::vector<ItemType> itemtypes{ };

			Instance instance("", Vehicle(0, 0, 0, D, max_vol, 0, 0, 0, 0), itemtypes, customers, 0, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids1{ 1, 2 };
			std::vector<unsigned int> item_ids{};
			Tour tour1(1, customer_ids1, item_ids);

			// Solution Creation
			std::vector<Tour> tours{ tour1 };
			Solution solution(tours);

			// Test
			Assert::AreEqual(true, ConstraintsRouting::checkCapacities(solution, true, instance));
		}
	};
}