#pragma once

namespace validator {

	class Point {

	public:

		// Constructor
		Point() : x(0), y(0), z(0) {}
		Point(const unsigned int x, const unsigned int y, const unsigned int z) : x(x), y(y), z(z) {}

		friend bool operator== (const Point& a, const Point& b);

		unsigned int x;
		unsigned int y;
		unsigned int z;
	};

}
