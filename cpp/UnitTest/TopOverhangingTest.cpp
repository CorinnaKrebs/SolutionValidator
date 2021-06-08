#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(TopOverhangingTest) {
		double alpha = 0.75;
		float dim = 4;
	public:
		TEST_METHOD(InfeasiblePosition) {
			// 3rd Item is Overhanging, 4 Items in Stack
			ItemType type(1, dim, dim, dim, 0, false, 0);
			ItemType typeLong(1, 1.25 * dim, dim, dim, 0, false, 0);

			Item item1(0, 1, 0, type);
			Item item2(1, 1, 0, type);
			Item item3(2, 1, 0, typeLong);
			Item item4(3, 1, 0, type);

			// Instance Creation
			Customer customer(1, 0, 0, 4, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);
			instance.items.emplace_back(item4);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1 , 2, 3};
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(0, 0, 1 * dim));
			instance.items.at(item3.id).setPosition(Point(0, 0, 2 * dim));
			instance.items.at(item4.id).setPosition(Point(0, 0, 3 * dim));

			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(item1.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(item2.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(item3.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(item4.id), tour, endPos, instance);

			// Tests
			Assert::AreEqual(true,  ConstraintsLoading::checkTopOverhanging(instance.items.at(item1.id), alpha, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkTopOverhanging(instance.items.at(item2.id), alpha, instance, true));
			Assert::AreEqual(false, ConstraintsLoading::checkTopOverhanging(instance.items.at(item3.id), alpha, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkTopOverhanging(instance.items.at(item4.id), alpha, instance, true));
			Assert::AreEqual(1.0, instance.items.at(item1.id).support);
			Assert::AreEqual(1.0, instance.items.at(item2.id).support);
			Assert::AreEqual(1.0, instance.items.at(item4.id).support);
			Assert::IsTrue(instance.items.at(item3.id).support > alpha);
		}

		TEST_METHOD(FeasiblePosition) {
			// Top Item (4th) is Overhanging
			ItemType type(1, dim, dim, dim, 0, false, 0);
			ItemType typeLong(1, 1.25 * dim, dim, dim, 0, false, 0);

			Item item1(0, 1, 0, type);
			Item item2(1, 1, 0, type);
			Item item3(2, 1, 0, type);
			Item item4(3, 1, 0, typeLong);

			// Instance Creation
			Customer customer(1, 0, 0, 4, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);
			instance.items.emplace_back(item3);
			instance.items.emplace_back(item4);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1 , 2, 3 };
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(0, 0, 1 * dim));
			instance.items.at(item3.id).setPosition(Point(0, 0, 2 * dim));
			instance.items.at(item4.id).setPosition(Point(0, 0, 3 * dim));

			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(item1.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(item2.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(item3.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(item4.id), tour, endPos, instance);

			// Tests
			Assert::AreEqual(true, ConstraintsLoading::checkTopOverhanging(instance.items.at(item1.id), alpha, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkTopOverhanging(instance.items.at(item2.id), alpha, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkTopOverhanging(instance.items.at(item3.id), alpha, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkTopOverhanging(instance.items.at(item4.id), alpha, instance, true));
			Assert::AreEqual(1.0, instance.items.at(item1.id).support);
			Assert::AreEqual(1.0, instance.items.at(item2.id).support);
			Assert::AreEqual(1.0, instance.items.at(item3.id).support);
			Assert::IsTrue(instance.items.at(item4.id).support > alpha);
		}
	};
}