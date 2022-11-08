#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(MinimalSupportingTest) {
		double alpha = 0.75;
		unsigned int dim = 8;

	public:
		TEST_METHOD(InfeasiblePosition) {
			// Stacked Item is hanging over by 1/2

			ItemType type(1, dim, dim, dim, 0, false, 0);

			Item itemBottom(0, 1, Rotation::none, type);
			Item itemTop(1, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemBottom);
			instance.items.emplace_back(itemTop);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1 };
			Tour tour(1, customer_ids, item_ids);

			instance.items.at(itemBottom.id).setPosition(Point(0, 0,  0));
			instance.items.at(itemTop.id)   .setPosition(Point(0, dim/2, dim));

			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(itemBottom.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(itemTop.id),    tour, endPos, instance);

			Assert::AreEqual(true, ConstraintsLoading::checkMinimalSupport(instance.items.at(itemBottom.id), alpha, instance, true));
			Assert::AreEqual(false, ConstraintsLoading::checkMinimalSupport(instance.items.at(itemTop.id), alpha, instance, true));
			Assert::AreEqual(1.0, instance.items.at(itemBottom.id).support);
			Assert::AreEqual(0.5, instance.items.at(itemTop.id).support);
		}

		TEST_METHOD(FeasiblePosition) {
			// Stacked Item is hanging over by 1/4

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
			std::vector<unsigned int> item_ids{ 0, 1 };
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));
			instance.items.at(item2.id).setPosition(Point(0, dim/4, dim));

			const unsigned int endPos = tour.item_ids.size();
			ConstraintsLoading::getRelevantItems(instance.items.at(item1.id), tour, endPos, instance);
			ConstraintsLoading::getRelevantItems(instance.items.at(item2.id), tour, endPos, instance);

			// Tests
			Assert::AreEqual(true, ConstraintsLoading::checkMinimalSupport(instance.items.at(item1.id), alpha, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkMinimalSupport(instance.items.at(item2.id), alpha, instance, true));
			Assert::AreEqual(1.0,  instance.items.at(item1.id).support);
			Assert::AreEqual(0.75, instance.items.at(item2.id).support);
		}
	};
}