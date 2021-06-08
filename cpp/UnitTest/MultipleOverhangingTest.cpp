#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(MultipleOverhangingTest) {
		double alpha = 0.75;

	public:
		TEST_METHOD(FeasiblePosition) {
			// Arrangement consisting of two Towers
			// First Tower: Two Items
			// Second Tower: High Item
			// On Top: an Overhanging Item
			unsigned int dim = 6;

			ItemType typeNormal(1, dim, dim, dim,        0, false, 0);
			ItemType typeHigh  (2, dim, dim, 2 * dim,    0, false, 0);
			ItemType typeTop   (3, dim, 2 * dim +1, dim, 0, false, 0);

			Item itemNormal1(0, 1, 0, typeNormal);
			Item itemNormal2(1, 1, 0, typeNormal);
			Item itemHigh   (2, 1, 0, typeHigh);
			Item itemTop    (3, 1, 0, typeTop);

			// Instance Creation
			Customer customer(1, 0, 0, 4, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ typeNormal, typeHigh, typeTop };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemNormal1);
			instance.items.emplace_back(itemNormal2);
			instance.items.emplace_back(itemHigh);
			instance.items.emplace_back(itemTop);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2, 3 };
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(itemNormal1.id).setPosition(Point(0, 0, 0));
			instance.items.at(itemNormal2.id).setPosition(Point(0, 0, dim));
			instance.items.at(itemHigh.id)   .setPosition(Point(0, dim, 0));
			instance.items.at(itemTop.id)    .setPosition(Point(0, 0, 2* dim));

			// Tests
			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(itemNormal1.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemNormal2.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemHigh.id),    tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemTop.id),     tour, endPos, instance);

			Assert::AreEqual(true, ConstraintsLoading::checkMultipleOverhanging(instance.items.at(itemTop.id), alpha, instance, true));
		}

		TEST_METHOD(InfeasiblePosition) {
			// Arrangement consisting of three stacked items,
			// which increase by 1/alpa per level

			unsigned int dim = 3;
			double w = 2 * dim;

			ItemType typeBottom(1, dim, w,                 dim, 0, false, 0);
			ItemType typeWide  (2, dim, w/alpha,           dim, 0, false, 0);
			ItemType typeTop   (3, dim, w/(alpha * alpha), dim, 0, false, 0);

			Item itemBottom(0, 1, 0, typeBottom);
			Item itemWide  (1, 1, 0, typeWide);
			Item itemTop   (2, 1, 0, typeTop);

			// Instance Creation
			Customer customer(1, 0, 0, 3, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ typeBottom, typeWide, typeTop };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemBottom);
			instance.items.emplace_back(itemWide);
			instance.items.emplace_back(itemTop);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1, 2 };
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(itemBottom.id).setPosition(Point(0, 0, 0));
			instance.items.at(itemWide.id)  .setPosition(Point(0, 0, dim));
			instance.items.at(itemTop.id)   .setPosition(Point(0, 0, 2 * dim));

			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(itemBottom.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemWide.id),   tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemTop.id),    tour, endPos, instance);
			
			// Tests
			Assert::AreEqual(true, ConstraintsLoading::checkMinimalSupport(instance.items.at(itemWide.id), alpha, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkMinimalSupport(instance.items.at(itemTop.id),  alpha, instance, true));

			Assert::AreEqual(false, ConstraintsLoading::checkMultipleOverhanging(instance.items.at(itemTop.id), alpha, instance, true));
		}
	};
}