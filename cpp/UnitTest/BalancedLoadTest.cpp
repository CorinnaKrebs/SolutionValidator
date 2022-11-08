#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;


namespace LoadingConstraints {
	TEST_CLASS(BalancedLoadTest) {
		unsigned int dim  = 5;
		unsigned int mass = 6;

	public:
		TEST_METHOD(LeftExceeding) {	
			ItemType type(1, dim, dim + 0.5, dim, mass, false, 0);

			Item item1(0, 1, Rotation::none, type);
			Item item2(1, 1, Rotation::none, type);
			Item item3(2, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 3, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(5 * dim, 2 * dim, dim, 4.0 * mass, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2};
			Tour tour(1, customer_ids, item_ids);

			// All in Left Vehicle Half
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(1 * dim, 0, 0));
			instance.items.at(item3.id).setPosition(Point(2 * dim, 0, 0));

			double balanced_part = 0.7;
			const unsigned int endPos = tour.item_ids.size();
			Assert::AreEqual(true,  ConstraintsLoading::checkBalancedLoading(instance.items.at(item1.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(1.0 * mass, tour.mass_L);
			Assert::AreEqual(0.0, tour.mass_R);
			Assert::AreEqual(true,  ConstraintsLoading::checkBalancedLoading(instance.items.at(item2.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(2.0 * mass, tour.mass_L);
			Assert::AreEqual(0.0, tour.mass_R);
			Assert::AreEqual(false,  ConstraintsLoading::checkBalancedLoading(instance.items.at(item3.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(3.0 * mass, tour.mass_L);
			Assert::AreEqual(0.0, tour.mass_R);
		}

		TEST_METHOD(RightExceeding) {
			ItemType type(1, dim, dim, dim, mass, false, 0);

			Item item1(0, 1, Rotation::none, type);
			Item item2(1, 1, Rotation::none, type);
			Item item3(2, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(5 * dim, 2 * dim, dim, 4.0 * mass, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2 };
			Tour tour(1, customer_ids, item_ids);

			// All in Right Vehicle Half
			instance.items.at(item1.id).setPosition(Point(0,       dim + 1.0, 0));
			instance.items.at(item2.id).setPosition(Point(1 * dim, dim + 1.0, 0));
			instance.items.at(item3.id).setPosition(Point(2 * dim, dim + 1.0, 0));

			// Tests
			double balanced_part = 0.7;
			Assert::AreEqual(true, ConstraintsLoading::checkBalancedLoading(instance.items.at(item1.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(1.0 * mass, tour.mass_R);
			Assert::AreEqual(0.0, tour.mass_L);
			Assert::AreEqual(true, ConstraintsLoading::checkBalancedLoading(instance.items.at(item2.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(2.0 * mass, tour.mass_R);
			Assert::AreEqual(0.0, tour.mass_L);
			Assert::AreEqual(false, ConstraintsLoading::checkBalancedLoading(instance.items.at(item3.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(3.0 * mass, tour.mass_R);
			Assert::AreEqual(0.0, tour.mass_L);
		}

		TEST_METHOD(DistributedExceeding) {
			unsigned int inRightSide = 1.0;
			unsigned int itemW = dim + inRightSide;
			unsigned int vehW  = 2 * dim;
			ItemType type(1, dim, itemW, dim, mass, false, 0);

			Item item1(0, 1, Rotation::none, type);
			Item item2(1, 1, Rotation::none, type);
			Item item3(2, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 3, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(5 * dim, vehW, dim, 4.0 * mass, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2 };
			Tour tour(1, customer_ids, item_ids);

			// Mixed
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(1 * dim, 0, 0));
			instance.items.at(item3.id).setPosition(Point(2 * dim, 0, 0));

			// Tests
			double balanced_part = 0.7;
			double part = inRightSide / vehW;
			Assert::AreEqual(true, ConstraintsLoading::checkBalancedLoading(instance.items.at(item1.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(1.0 * (1.0 - part) * mass, tour.mass_L);
			Assert::AreEqual(1.0 *        part  * mass, tour.mass_R);

			Assert::AreEqual(true, ConstraintsLoading::checkBalancedLoading(instance.items.at(item1.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(2.0 * (1.0 - part) * mass, tour.mass_L);
			Assert::AreEqual(2.0 *        part  * mass, tour.mass_R);

			Assert::AreEqual(false, ConstraintsLoading::checkBalancedLoading(instance.items.at(item1.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(3.0 * (1.0 - part) * mass, tour.mass_L);
			Assert::AreEqual(3.0 *        part * mass, tour.mass_R);
		}

		TEST_METHOD(Feasible) {
			ItemType type(1, dim, dim, dim, mass, false, 0);

			Item item1(0, 1, Rotation::none, type);
			Item item2(1, 1, Rotation::none, type);
			Item item3(2, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 3, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(5 * dim, 2 * dim, dim, 4.0 * mass, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2 };
			Tour tour(1, customer_ids, item_ids);

			// Mixed
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(1 * dim, dim, 0));
			instance.items.at(item3.id).setPosition(Point(2 * dim, 0, 0));

			// Tests
			double balanced_part = 0.7;
			Assert::AreEqual(true, ConstraintsLoading::checkBalancedLoading(instance.items.at(item1.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(1.0 * mass, tour.mass_L);
			Assert::AreEqual(0.0, tour.mass_R);
			Assert::AreEqual(true, ConstraintsLoading::checkBalancedLoading(instance.items.at(item2.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(1.0 * mass, tour.mass_L);
			Assert::AreEqual(1.0 * mass, tour.mass_R);
			Assert::AreEqual(true, ConstraintsLoading::checkBalancedLoading(instance.items.at(item3.id), tour, true, balanced_part, instance, true));
			Assert::AreEqual(2.0 * mass, tour.mass_L);
			Assert::AreEqual(1.0 * mass, tour.mass_R);
		}
	};
}