#pragma once

namespace validator {

	/**
	* The type ItemType.
	*/
	class ItemType {

	public:
		// Constructor
		/**
		* Instantiates a new Item type.
		*/
		ItemType() : id(0), l(0), w(0), h(0), mass(0), fragility(0), lbs(0) {}

		ItemType(const unsigned id, const unsigned int l, const unsigned int w, const unsigned int h, const double mass, const bool fragility, const double lbs)
			: id(id), l(l), w(w), h(h), mass(mass), fragility(fragility), lbs(lbs) {}

		/** Unique Id */
		const unsigned int id;

		/** Length */
		const unsigned int l;

		/** Width */
		const unsigned int w;

		/** Height */
		const unsigned int h;

		/** Mass */
		const double mass;

		/** Fragility flag, (1: fragile) */
		const bool fragility;

		/** Maximum Load per Area (Load Bearing Strength) */
		const double lbs;
	};
}
