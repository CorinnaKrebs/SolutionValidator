#include "Point.h"

namespace  validator {

	bool operator==(const Point& a, const Point& b) {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
}
