#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(OverlappingTest) {
		unsigned int dim = 5; // > 1
	public:
		TEST_METHOD(ItemsOverlapping) {
			
			ItemType itemType(1, dim, dim, dim, 0, 0, 0);
			Item item1(0, 1, Rotation::none, itemType);
			Item item2(1, 1, Rotation::none, itemType);

			// Instance Creation
			Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ itemType };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 1 };
			std::vector<unsigned int> item_ids{ 0, 1 };
			Tour tour(1, customer_ids, item_ids);

			instance.items.emplace_back(item1);
			instance.items.emplace_back(item2);

			// Position
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));

			const unsigned int endPos = tour.item_ids.size();

			// Overlapping Right Side of Item 1
			instance.items.at(item2.id).setPosition(Point(0, 1, 0));
			Assert::AreEqual(false, ConstraintsLoading::checkOverlapping(instance.items.at(item1.id), tour, endPos, instance, true));

			// Overlapping Front Side of Item 1
			instance.items.at(item2.id).setPosition(Point(1, 0, 0));
			Assert::AreEqual(false, ConstraintsLoading::checkOverlapping(instance.items.at(item1.id), tour, endPos, instance, true));

			// Overlapping Top Side of Item 1
			instance.items.at(item2.id).setPosition(Point(0, 0, 1));
			Assert::AreEqual(false, ConstraintsLoading::checkOverlapping(instance.items.at(item1.id), tour, endPos, instance, true));

			// Still Overlapping
			instance.items.at(item2.id).setPosition(Point(1, 1, 1));
			Assert::AreEqual(false, ConstraintsLoading::checkOverlapping(instance.items.at(item1.id), tour, endPos, instance, true));

			// Feasible
			instance.items.at(item2.id).setPosition(Point(0, dim, 0));
			Assert::AreEqual(true, ConstraintsLoading::checkOverlapping(instance.items.at(item1.id), tour, endPos, instance, true));
		}
	};
}
