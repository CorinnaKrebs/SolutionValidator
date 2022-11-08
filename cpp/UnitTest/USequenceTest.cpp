#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(USequenceTest) {
		unsigned int dim = 5;
	public:
		TEST_METHOD(InfeasiblePositions) {
			ItemType type(1, dim, dim, dim, 0, false, 0);

			Item item1(0, 1, Rotation::none, type);
			Item item2(1, 2, Rotation::none, type);

			// Instance Creation
			Customer customer1(1, 0, 0, 1, 0, 0, 0, 0, 0);
			Customer customer2(2, 0, 0, 1, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer1, customer2 };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 5, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1, 2 };
			std::vector<unsigned int> item_ids{ 0, 1 };
			Tour tour(1, customer_ids, item_ids);

			// First Served Item Behind Second Served Item
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(dim, 0, 0));

			// Tests
			const unsigned int endPos = tour.item_ids.size();
			Assert::AreEqual(false, ConstraintsLoading::checkUnloadingSequence(instance.items.at(item1.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkUnloadingSequence(instance.items.at(item2.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(false, ConstraintsLoading::checkUnloadingSequence(instance.items.at(item1.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkUnloadingSequence(instance.items.at(item2.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));

			// First Served Item Under Second Served Item
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(0, 0, dim));

			// Tests
			Assert::AreEqual(false, ConstraintsLoading::checkUnloadingSequence(instance.items.at(item1.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkUnloadingSequence(instance.items.at(item2.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(false, ConstraintsLoading::checkUnloadingSequence(instance.items.at(item1.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkUnloadingSequence(instance.items.at(item2.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));
		}

		TEST_METHOD(Differences) {
			ItemType typeNormal(1,     dim,   dim,     dim,   0, false, 0);
			ItemType typeHigh  (2,     dim,   dim, 2 * dim,   0, false, 0);
			ItemType typeLong  (3, 2 * dim,   dim,     dim,   0, false, 0);

			Item itemC1(0, 1, Rotation::none, typeNormal);
			Item itemC2High(1, 2, Rotation::none, typeHigh);
			Item itemC2Long(2, 2, Rotation::none, typeLong);

			// Instance Creation
			Customer customer1(1, 0, 0, 1, 0, 0, 0, 0, 0);
			Customer customer2(2, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer1, customer2 };
			std::vector<ItemType> itemtypes{ typeNormal, typeHigh, typeLong };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemC1);
			instance.items.emplace_back(itemC2High);
			instance.items.emplace_back(itemC2Long);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1, 2 };
			std::vector<unsigned int> item_ids{ 0, 1, 2 };
			Tour tour(1, customer_ids, item_ids);

			// Customer2 : Item High in Origin, Long on Top of High -> Overhanging
			// Customer1 : Item in Front of High
			instance.items.at(itemC2High.id).setPosition(Point(0,   0, 0));
			instance.items.at(itemC2Long.id).setPosition(Point(0,   0, 2 * dim));
			instance.items.at(itemC1.id)    .setPosition(Point(dim, 0, 0));

			// According MLIFO: everything is fine
			const unsigned int endPos = tour.item_ids.size();
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC2High.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC2Long.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC1.id),     tour, endPos, UnloadingSequence::MLIFO, instance, true));

			// According LIFO: Item of Customer 1 has no feasible position 
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC2High.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC2Long.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(false, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC1.id),    tour, endPos, UnloadingSequence::LIFO, instance, true));
		}

		TEST_METHOD(FeasiblePosition) {
			ItemType type(1, dim, dim, dim, 0, false, 0);

			Item itemC1(0, 1, Rotation::none, type);
			Item itemC2(1, 2, Rotation::none, type);

			// Instance Creation
			Customer customer1(1, 0, 0, 1, 0, 0, 0, 0, 0);
			Customer customer2(2, 0, 0, 1, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer1, customer2 };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemC1);
			instance.items.emplace_back(itemC2);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1, 2 };
			std::vector<unsigned int> item_ids{ 1, 0 };
			Tour tour(1, customer_ids, item_ids);

			// Customer2 : Item in Origin
			// Customer1 : Item in Front
			instance.items.at(itemC2.id).setPosition(Point(0,   0, 0));
			instance.items.at(itemC1.id).setPosition(Point(dim, 0, 0));

			// Tests
			const unsigned int endPos = tour.item_ids.size();
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC1.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC2.id), tour, endPos, UnloadingSequence::MLIFO, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC1.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemC2.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
		}

	};
}