#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(StaticStabilityTest) {
		float dim = 4;
public:
	TEST_METHOD(InfeasibleTwoItems) {
		// 2nd item is half overhanging
		ItemType type(1, dim, dim, dim, 0, false, 0);

		Item item1(0, 1, Rotation::none, type);
		Item item2(1, 1, Rotation::none, type);

		// Instance Creation
		Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
		std::vector<Customer> customers{ customer };
		std::vector<ItemType> itemtypes{ type };
		Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
		instance.items.emplace_back(item1);
		instance.items.emplace_back(item2);

		// Tour Creation
		std::vector<unsigned int> customer_ids{ 1 };
		std::vector<unsigned int> item_ids{ 0, 1};
		Tour tour(1, customer_ids, item_ids);

		// Positions
		instance.items.at(item1.id).setPosition(Point(0, 0, 0));
		instance.items.at(item2.id).setPosition(Point(0, ceil(dim / 2.0), dim));

		const unsigned int endPos = tour.item_ids.size();
		ConstraintsLoading::getRelevantItems(instance.items.at(item1.id), tour, endPos, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(item2.id), tour, endPos, instance);

		// Tests
		Assert::AreEqual(true, ConstraintsLoading::checkStaticStability(instance.items.at(item1.id), instance, true));
		Assert::AreEqual(false, ConstraintsLoading::checkStaticStability(instance.items.at(item2.id), instance, true));
	}

	TEST_METHOD(InfeasibleSixItems) {
		// 2nd item is half overhanging
		ItemType type(1, dim, dim, dim, 0, false, 0);

		// Instance Creation
		Customer customer(1, 0, 0, 4, 0, 0, 0, 0, 0);
		std::vector<Customer> customers{ customer };
		std::vector<ItemType> itemtypes{ type };
		Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

		// Create Items and set Position
		for (int i = 0; i < 6; ++i) {
			Item item(i, 1, Rotation::none, type);
			bool odd = i % 2;
			int y = odd * dim + (i / 2) * 0.25 * dim;
			int z = (i / 2) * dim;
			item.setPosition(Point(0, y, z));
			instance.items.emplace_back(item);
		}		

		// Tour Creation
		std::vector<unsigned int> customer_ids{ 1 };
		std::vector<unsigned int> item_ids{ 0, 1 , 2, 3, 4, 5};
		Tour tour(1, customer_ids, item_ids);

		const unsigned int endPos = tour.item_ids.size();
		for (auto& item : instance.items)
			ConstraintsLoading::getRelevantItems(item, tour, endPos, instance);

		// Tests
		Assert::AreEqual(true, ConstraintsLoading::checkStaticStability(instance.items.at(0), instance, true));
		Assert::AreEqual(true, ConstraintsLoading::checkStaticStability(instance.items.at(1), instance, true));
		Assert::AreEqual(true, ConstraintsLoading::checkStaticStability(instance.items.at(2), instance, true));
		Assert::AreEqual(true, ConstraintsLoading::checkStaticStability(instance.items.at(3), instance, true));
		Assert::AreEqual(true, ConstraintsLoading::checkStaticStability(instance.items.at(4), instance, true));
		Assert::AreEqual(false, ConstraintsLoading::checkStaticStability(instance.items.at(5), instance, true));
	}

	TEST_METHOD(FeasibleSixItems) {
		ItemType type(1, dim, dim, dim, 0, false, 0);

		// Instance Creation
		Customer customer(1, 0, 0, 4, 0, 0, 0, 0, 0);
		std::vector<Customer> customers{ customer };
		std::vector<ItemType> itemtypes{ type };
		Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

		// Create Items and set Position
		for (int i = 0; i < 6; ++i) {
			Item item(i, 1, Rotation::none, type);
			bool odd = i % 2;
			int y = odd * dim + odd * 0.25 * dim;
			int z = (i / 2) * dim;
			item.setPosition(Point(0, y, z));
			instance.items.emplace_back(item);
		}

		// Tour Creation
		std::vector<unsigned int> customer_ids{ 1 };
		std::vector<unsigned int> item_ids{ 0, 1 , 2, 3, 4, 5 };
		Tour tour(1, customer_ids, item_ids);

		const unsigned int endPos = tour.item_ids.size();
		for (auto& item : instance.items)
			ConstraintsLoading::getRelevantItems(item, tour, endPos, instance);

		// Tests
		for (auto& item : instance.items)
			Assert::AreEqual(true, ConstraintsLoading::checkStaticStability(item, instance, true));
	}
};
}