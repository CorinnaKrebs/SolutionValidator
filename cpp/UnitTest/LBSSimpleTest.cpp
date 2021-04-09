#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LoadingConstraints {
	TEST_CLASS(LBSCompleteTest) {
public:

	TEST_METHOD(Overhanging) {
		// Two Items on the Ground
		// Third Item is stacked on them and is overhanging for 1/4

		int mass = 2;

		ItemType typeNormal(1, 2, 2, 2, mass, false, 2.0);
		ItemType typeLong(2, 2, 4, 2, mass, false, 2.0);

		// Bottom
		Item itemBottomLeft(0, 1, 0, typeNormal);
		Item itemBottomRight(1, 1, 0, typeNormal);
		Item itemTop(2, 1, 0, typeLong);

		// Instance Creation
		Customer customer(1, 0, 0, 3, 0, 0, 0, 0, 0);
		std::vector<Customer> customers{ customer };
		std::vector<ItemType> itemtypes{ typeNormal, typeLong };
		Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

		instance.items.emplace_back(itemBottomLeft);
		instance.items.emplace_back(itemBottomRight);
		instance.items.emplace_back(itemTop);

		// Tour Creation
		std::vector<unsigned int> customer_ids{ 1 };
		std::vector<unsigned int> item_ids{ 0, 1, 2 };
		Tour tour(1, customer_ids, item_ids);

		// Set Positions
		instance.items.at(itemBottomLeft.id).setPosition(Point(0, 0, 0));
		instance.items.at(itemBottomRight.id).setPosition(Point(0, 2, 0));
		instance.items.at(itemTop.id).setPosition(Point(0, 1, 2));

		ConstraintsLoading::getRelevantItems(instance.items.at(itemBottomLeft.id), tour, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(itemBottomRight.id), tour, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(itemTop.id), tour, instance);

		// Calculates the loads for the entire stack, begin with top Item.
		Assert::AreEqual(true, ConstraintsLoading::checkLBSComplete(instance.items.at(itemTop.id), instance, instance.items.at(itemTop.id).mass));


		Assert::AreEqual(0.75, instance.items.at(itemTop.id).support);
		const double support_top = 0.75;
		const double partLeft = 0.25 / support_top;
		const double partRight = 0.50 / support_top;
		Assert::AreEqual(1.0, partLeft + partRight);
		Assert::AreEqual(support_top, instance.items.at(itemTop.id).support);
		Assert::AreEqual(mass * partLeft, instance.items.at(itemBottomLeft.id).sumLoad);
		Assert::AreEqual(mass * partRight, instance.items.at(itemBottomRight.id).sumLoad);
	}

	TEST_METHOD(Pyramid) {
		// Arrangement of the items like a pyramid
		// Same Arrangement as for LBSSimple

		ItemType type(1, 2, 2, 2, 2, false, 1.0);

		// Bottom
		Item itemBottomLeft(0, 1, 0, type);
		Item itemBottomCenter(1, 1, 0, type);
		Item itemBottomRight(2, 1, 0, type);

		// Second Row
		Item itemCenterLeft(3, 1, 0, type);
		Item itemCenterRight(4, 1, 0, type);

		// Top Row
		Item itemTop(5, 1, 0, type);

		// Instance Creation
		Customer customer(1, 0, 0, 5, 0, 0, 0, 0, 0);
		std::vector<Customer> customers{ customer };
		std::vector<ItemType> itemtypes{ type };
		Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

		instance.items.emplace_back(itemBottomLeft);
		instance.items.emplace_back(itemBottomCenter);
		instance.items.emplace_back(itemBottomRight);
		instance.items.emplace_back(itemCenterLeft);
		instance.items.emplace_back(itemCenterRight);
		instance.items.emplace_back(itemTop);

		// Tour Creation
		std::vector<unsigned int> customer_ids{ 1 };
		std::vector<unsigned int> item_ids{ 0, 1, 2, 3, 4, 5 };
		Tour tour(1, customer_ids, item_ids);

		// Set Positions
		instance.items.at(itemBottomLeft.id).setPosition(Point(0, 0, 0));
		instance.items.at(itemBottomCenter.id).setPosition(Point(0, 2, 0));
		instance.items.at(itemBottomRight.id).setPosition(Point(0, 4, 0));
		instance.items.at(itemCenterLeft.id).setPosition(Point(0, 1, 2));
		instance.items.at(itemCenterRight.id).setPosition(Point(0, 3, 2));
		instance.items.at(itemTop.id).setPosition(Point(0, 2, 4));

		ConstraintsLoading::getRelevantItems(instance.items.at(itemBottomLeft.id), tour, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(itemBottomCenter.id), tour, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(itemBottomRight.id), tour, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(itemCenterLeft.id), tour, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(itemCenterRight.id), tour, instance);
		ConstraintsLoading::getRelevantItems(instance.items.at(itemTop.id), tour, instance);

		// Calculates the loads for the entire stack, begin with top Item.
		Assert::AreEqual(true, ConstraintsLoading::checkLBSComplete(instance.items.at(itemTop.id), instance, instance.items.at(itemTop.id).mass));
		Assert::AreEqual(true, ConstraintsLoading::checkLBSComplete(instance.items.at(itemCenterLeft.id), instance, instance.items.at(itemCenterLeft.id).mass));
		Assert::AreEqual(true, ConstraintsLoading::checkLBSComplete(instance.items.at(itemCenterRight.id), instance, instance.items.at(itemCenterRight.id).mass));

		Assert::AreEqual(1.0, instance.items.at(itemCenterLeft.id).sumLoad);
		Assert::AreEqual(1.0, instance.items.at(itemCenterRight.id).sumLoad);
		Assert::AreEqual(1.5, instance.items.at(itemBottomLeft.id).sumLoad);
		Assert::AreEqual(3.0, instance.items.at(itemBottomCenter.id).sumLoad);
		Assert::AreEqual(1.5, instance.items.at(itemBottomRight.id).sumLoad);

	}
	};
}