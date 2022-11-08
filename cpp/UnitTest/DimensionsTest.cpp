#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints
{
	TEST_CLASS(DimensionsTest) {
		unsigned int dim = 5;
	public:

		TEST_METHOD(WrongDimensions) {
			Item item(1, 1, Rotation::none, ItemType(1, dim, dim, dim, 0, 0, 0));
			item.setPosition(Point(0, 0, 0));
			item.max = Point(1, 1, 1);
			Assert::AreEqual(false, ConstraintsLoading::checkDimensions(item, true));
		}

		TEST_METHOD(CorrectDimensions) {
			Item item(1, 1, Rotation::none, ItemType(1, dim, dim, dim, 0, 0, 0));
			item.setPosition(Point(0, 0, 0));
			Assert::AreEqual(true, ConstraintsLoading::checkDimensions(item, true));
		}
	};
}
