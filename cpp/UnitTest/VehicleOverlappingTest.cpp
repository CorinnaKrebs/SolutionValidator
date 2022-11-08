#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints
{
	TEST_CLASS(VehicleOverlappingTest) {
		unsigned int dim = 5;
	public:
		
		TEST_METHOD(ItemOverlappingVehicle) {
			Item item(1, 1, Rotation::none, ItemType(1, dim + 1, dim, dim, 0, 0, 0));
			item.setPosition(Point(0, 0, 0));

			Vehicle vehicle(dim, dim, dim, 0, 0, 0, 0, 0, 0);
			Assert::AreEqual(false, ConstraintsLoading::checkVehicleWalls(item, vehicle, true));
		}

		TEST_METHOD(ItemNotOverlapping) {
			Item item(1, 1, Rotation::none, ItemType(1, dim, dim, dim, 0, 0, 0));
			item.setPosition(Point(0, 0, 0));

			Vehicle vehicle(dim, dim, dim, 0, 0, 0, 0, 0, 0);

			Assert::AreEqual(true, ConstraintsLoading::checkVehicleWalls(item, vehicle, true));
		}
	};
}
