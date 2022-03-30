#pragma once
#include "Dimension.h"

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
		ItemType() : id(0), dimension(0, 0, 0), mass(0), fragility(0), lbs(0) {}

		ItemType(const unsigned id, const unsigned int l, const unsigned int w, const unsigned int h, const double mass, const bool fragility, const double lbs)
			: id(id), dimension(l, w, h), mass(mass), fragility(fragility), lbs(lbs) {}

		/** Unique Id */
		const unsigned int id;

		/* contains length, width, height*/
		Dimension dimension;

		/** Mass */
		const double mass;

		/** Fragility flag, (1: fragile) */
		const bool fragility;

		/** Maximum Load per Area (Load Bearing Strength) */
		const double lbs;
	};
}
