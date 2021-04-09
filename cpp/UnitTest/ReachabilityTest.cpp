#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(ReachabilityTest) {
		public:
			unsigned int dim = 5;
		TEST_METHOD(InfeasiblePosition) {
			// Bottom Item is too long to reach Top Item
			// LIFO is obeyed

			double lambda = dim;

			ItemType typeBottom(1, 3 * dim,  dim, dim, 0, false, 0);
			ItemType typeTop   (2, dim,      dim, dim, 0, false, 0);

			Item itemTop(0, 1, 0, typeBottom);
			Item itemBottom(1, 2, 0, typeTop);


			Customer customerBottom(1, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customerTop   (2, 0, 0, 0, 0, 0, 0, 0, 0);

			std::vector<Customer> customers{ customerBottom, customerTop };
			std::vector<ItemType> itemtypes{ typeBottom, typeTop };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemBottom);
			instance.items.emplace_back(itemTop);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 2, 1 };
			std::vector<unsigned int> item_ids{ 0, 1 };
			Tour tour(1, customer_ids, item_ids);

			instance.items.at(itemBottom.id).setPosition(Point(0, 0, 0));
			instance.items.at(itemTop.id)   .setPosition(Point(0, 0, dim));

			const unsigned int endPos = tour.item_ids.size();
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemTop.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(false, ConstraintsLoading::checkReachability(instance.items.at(itemTop.id), tour, lambda,  true, instance, true));
		}

		TEST_METHOD(FeasiblePosition) {
			// length of ItemBottom = 2 * dim

			double lambda = dim;

			ItemType typeBottom(1, 2 * dim, dim, dim, 0, false, 0);
			ItemType typeTop(2, dim, dim, dim, 0, false, 0);

			Item itemTop(0, 1, 0, typeBottom);
			Item itemBottom(1, 2, 0, typeTop);


			Customer customerBottom(1, 0, 0, 0, 0, 0, 0, 0, 0);
			Customer customerTop   (2, 0, 0, 0, 0, 0, 0, 0, 0);

			std::vector<Customer> customers{ customerBottom, customerTop };
			std::vector<ItemType> itemtypes{ typeBottom, typeTop };
			Instance instance("", Vehicle(0, 0, 0, 0, 0, 0, 0, 0, 0), itemtypes, customers, 0, 0);
			instance.items.emplace_back(itemBottom);
			instance.items.emplace_back(itemTop);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ 2, 1 };
			std::vector<unsigned int> item_ids{ 0, 1 };
			Tour tour(1, customer_ids, item_ids);

			instance.items.at(itemBottom.id).setPosition(Point(0, 0, 0));
			instance.items.at(itemTop.id)   .setPosition(Point(0, 0, dim));

			const unsigned int endPos = tour.item_ids.size();
			Assert::AreEqual(true, ConstraintsLoading::checkUnloadingSequence(instance.items.at(itemTop.id), tour, endPos, UnloadingSequence::LIFO, instance, true));
			Assert::AreEqual(true, ConstraintsLoading::checkReachability(instance.items.at(itemTop.id), tour, lambda, true, instance, true));
		}

		TEST_METHOD(ScaleFactorTest) {
			Assert::AreEqual(1, ConstraintsLoading::getScaleFactor(60));
			Assert::AreEqual(10, ConstraintsLoading::getScaleFactor(600));
			
		}

	};
}