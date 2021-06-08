#include "pch.h"
#include "CppUnitTest.h"
#include "../Validator/ConstraintsLoading.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace validator;

namespace LoadingConstraints
{
	TEST_CLASS(MaxCoordinatesTest) {
		unsigned int dim = 5;
	public:

		TEST_METHOD(WrongCoordinates) {
			Item item(1, 1, 0, ItemType(1, dim, dim, dim, 0, 0, 0));
			item.setPosition(Point(0, 0, 0));
			item.max = Point(0, 0, 0);
			Assert::AreEqual(false, ConstraintsLoading::checkMaxCoordinates(item, true));
		}

		TEST_METHOD(CorrectCoordinates) {
			Item item(1, 1, 0, ItemType(1, dim, dim, dim, 0, 0, 0));
			item.setPosition(Point(0, 0, 0));
			Assert::AreEqual(true, ConstraintsLoading::checkMaxCoordinates(item, true));
		}
	};
}
