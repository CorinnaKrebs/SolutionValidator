#pragma once
#include "Point.h"
#include "ItemType.h"
#include <vector>
#include <set>

namespace validator {

	class Item {
		friend class ItemType;

	public:
		Item(const int id, const int customer_id, const bool rotated, const ItemType& type);
		Item();

		unsigned int id;			// Unique Id
		unsigned int type_id;		// Type Id
		unsigned int customer_id;	// Customer Id
		bool		 rotated;

		unsigned int l;				// Length
		unsigned int w;				// Width
		unsigned int h;				// Height

		double mass;				// Mass
		bool   fragility;			// Fragility flag (1: fragile)
		double lbs;					// Maximum Load per Area (Load Bearing Strength)
		double sumLoad;				// Total Load 

		Point min;
		Point max;

		// Calculated Members
		double support;							// Percentage of direct Support of Items
		std::set<unsigned int> itemsFront;		// Contains all ItemIds which are in Front
		std::set<unsigned int> itemsBehind;		// Contains all ItemIds which are Behind
		std::set<unsigned int> itemsBelow;		// Contains all ItemIds which are Below
		std::set<unsigned int> itemsAbove;		// Contains all ItemIds which are Above

		std::vector<double> load;				// Current load per Area Unit

		// Functions
		void setPosition(Point point);						// Sets the position
		bool isBelow(Item& other, const bool touch) const;	// Checks if other item is below
		bool isAbove(Item& other, const bool touch);		// Checks if other item is above
		bool isBehind(Item& other);							// Checks if other item is behind
		bool isInFront(Item& other);						// Checks if other item is in front
	};
}
