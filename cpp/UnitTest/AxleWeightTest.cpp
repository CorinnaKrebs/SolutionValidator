#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints {
	TEST_CLASS(AxleWeightTest) {
		unsigned int dim = 2;
		unsigned int D = 90;
		unsigned int max_FA = 50;
		unsigned int max_RA = 82;
		unsigned int WB = 48;
		unsigned int lF = 4;

	public:
		TEST_METHOD(FrontAxleExceeding) {
			unsigned int mass = 60;

			ItemType type(1, dim, dim, dim, mass, false, 0);

			Item item1(0, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(60, 25, 30, D, 0, WB, max_FA, max_RA, lF), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ };
			std::vector<unsigned int> item_ids{ };
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(item1.id).setPosition(Point(0, 0, 0));

			// Tests
			Assert::AreEqual(false, ConstraintsLoading::checkAxleWeights(instance.items.at(item1.id), tour, true, instance, true));
			Assert::AreEqual(mass * 9.81, tour.sum_F);
			Assert::AreEqual(mass * 9.81 * (lF + dim/2.0), tour.sum_T);
		}

		TEST_METHOD(RearAxleExceeding) {
			unsigned int mass = 80;

			ItemType type(1, dim, dim, dim, mass, false, 0);

			Item item1(0, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(60, 25, 30, D, 0, WB, max_FA, max_RA, lF), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ };
			std::vector<unsigned int> item_ids{ };
			Tour tour(1, customer_ids, item_ids);

			// Position
			instance.items.at(item1.id).setPosition(Point(55, 0, 0));

			// Tests
			Assert::AreEqual(false, ConstraintsLoading::checkAxleWeights(instance.items.at(item1.id), tour,  true, instance, true));
		}

		TEST_METHOD(Feasible) {
			unsigned int mass = 30;

			ItemType type(1, dim, dim, dim, mass, false, 0);

			Item item1(0, 1, Rotation::none, type);

			// Instance Creation
			Customer customer(1, 0, 0, 2, 0, 0, 0, 0, 0);
			std::vector<Customer> customers{ customer };
			std::vector<ItemType> itemtypes{ type };
			Instance instance("", Vehicle(60, 25, 30, D, 0, WB, max_FA, max_RA, lF), itemtypes, customers, 0, 0);
			instance.items.emplace_back(item1);

			// Tour Creation
			std::vector<unsigned int> customer_ids{ };
			std::vector<unsigned int> item_ids{ };
			Tour tour(1, customer_ids, item_ids);

			// Positions
			instance.items.at(item1.id).setPosition(Point(20, 0, 0));

			// Tests
			Assert::AreEqual(true, ConstraintsLoading::checkAxleWeights(instance.items.at(item1.id), tour, true, instance, true));
		}
	};
}