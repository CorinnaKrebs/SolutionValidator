#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(FragilityTest) {
		unsigned int dim = 5;
	public:
		TEST_METHOD(InfeasiblePosition) {
			// Non-Fragile on Top of Fragile -> Not Feasible

			ItemType typeNormal (1,  dim, dim, dim, 0, false, 0);
			ItemType typeFragile(2,  dim, dim, dim, 0, true, 0);

			Item itemFragile(0, 1, Rotation::none, typeFragile);
			Item itemNormal (1, 1, Rotation::none, typeNormal);

			// Instance Creation
			Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ typeNormal, typeFragile };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemFragile);
			instance.items.emplace_back(itemNormal);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1 };
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(itemFragile.id).setPosition(Point(0, 0, 0));
			instance.items.at(itemNormal.id). setPosition(Point(0, 0, dim));

			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(itemFragile.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemNormal.id),  tour, endPos, instance);

			// Tests
			Assert::AreEqual(true,  ConstraintsLoading::checkFragility(instance.items.at(itemFragile.id), tour, endPos, instance, true));
			Assert::AreEqual(false, ConstraintsLoading::checkFragility(instance.items.at(itemNormal.id), tour, endPos, instance, true));
		}
		
		TEST_METHOD(FeasiblePositions) {
			ItemType typeNormal(1,  dim, dim, dim, 0,  false, 0);
			ItemType typeFragile(2, dim, dim, dim, 0, true, 0);

			Item itemNormal1 (0, 1, Rotation::none, typeNormal);
			Item itemNormal2 (1, 1, Rotation::none, typeNormal);
			Item itemFragile1(2, 1, Rotation::none, typeFragile);
			Item itemFragile2(3, 1, Rotation::none, typeFragile);

			// Instance Creation
			Customer customer(1, 0, 0, 4, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ typeNormal, typeFragile };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemNormal1);
			instance.items.emplace_back(itemNormal2);
			instance.items.emplace_back(itemFragile1);
			instance.items.emplace_back(itemFragile2);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2, 3 };
			Tour tour(1, customer_ids, item_ids);

			// Non-Fragile on Top of Non-Fragile -> Feasible
			instance.items.at(itemNormal1.id).setPosition(Point(0, 0, 0));
			instance.items.at(itemNormal2.id).setPosition(Point(0, 0, dim));

			// Fragile on Top of Fragile -> Feasible
			instance.items.at(itemFragile1.id).setPosition(Point(0, dim, 0));
			instance.items.at(itemFragile2.id).setPosition(Point(0, dim, dim));

			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(itemNormal1.id),  tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemNormal2.id),  tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemFragile1.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemFragile2.id), tour, endPos, instance);

			// Tests
			Assert::AreEqual(true,  ConstraintsLoading::checkFragility(instance.items.at(itemNormal1.id),  tour, endPos, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkFragility(instance.items.at(itemNormal2.id),  tour, endPos, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkFragility(instance.items.at(itemFragile1.id), tour, endPos, instance, true));
			Assert::AreEqual(true,  ConstraintsLoading::checkFragility(instance.items.at(itemFragile2.id), tour, endPos, instance, true));
		}
	};
}
