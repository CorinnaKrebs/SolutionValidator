#pragma once

namespace validator {

	class ItemType {

	public:
		// Constructor
		ItemType() : id(0), l(0), w(0), h(0), mass(0), fragility(0), lbs(0) {}

		ItemType(const unsigned id, const unsigned int l, const unsigned int w, const unsigned int h, const double mass, const bool fragility, const double lbs)
			: id(id), l(l), w(w), h(h), mass(mass), fragility(fragility), lbs(lbs) {}

		const unsigned int id;		// Unique Id
		const unsigned int l;		// Length
		const unsigned int w;		// Width
		const unsigned int h;		// Height
		const double mass;			// Mass
		const bool fragility;		// Fragility flag, (1: fragile)
		const double lbs;			// Maximum Load per Area (Load Bearing Strength)
	};
}
