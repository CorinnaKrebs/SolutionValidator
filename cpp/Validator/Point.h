#pragma once

namespace validator {

	/**
	* The 3D Point.
	*/
	class Point {

	public:

		// Constructor
		Point() : x(0), y(0), z(0) {}
		Point(const unsigned int x, const unsigned int y, const unsigned int z) : x(x), y(y), z(z) {}

		/**
		* Equals Operator.
		*
		* @param a Point A
		* @param b Point b
		* @return true if two points are equal.
		*/
		friend bool operator== (const Point& a, const Point& b);

		/** x-Position */
		unsigned int x;

		/** y-Position */
		unsigned int y;

		/** z-Position */
		unsigned int z;
	};

}
