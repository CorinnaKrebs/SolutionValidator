#pragma once
#include "Point.h"
#include "ItemType.h"
#include "Dimension.h"
#include <vector>
#include <set>

namespace validator {
	/**
	* The type Item.
	*/
	class Item {
		friend class ItemType;

	public:
		/**
		 * Instantiates a new Item.
		 *
		 * @param id          the unique item id
		 * @param customer_id the customer id
		 * @param rotated     the rotation flag
		 * @param type        the ItemType
		 */
		Item(const int id, const int customer_id, const bool rotated, const ItemType& type);
		Item();

		/** Unique Id */
		unsigned int id;

		/** ItemType Id */
		unsigned int type_id;

		/** Customer Id */
		unsigned int customer_id;

		/** Rotation Flag (1: rotated (l = w, w = l)) */
		bool		 rotated;

		/* contains length, width, height*/
		Dimension dimension;

		/** Mass */
		double mass;

		/** Fragility flag (1: fragile) */
		bool   fragility;

		/** Maximum Load per Area (Load Bearing Strength) */
		double lbs;

		/** Total Load */
		double sumLoad;

		/** Position of Item, leftMost, deepest, bottommost corner */
		Point min;

		/** Maximum Corner of Item */
		Point max;

		// Calculated Members
		
		/** Percentage of direct Support of Items */
		double support;

		/** Contains all ItemIds which are in Front */
		std::set<unsigned int> itemsFront;

		/** Contains all ItemIds which are Behind */
		std::set<unsigned int> itemsBehind;

		/** Contains all ItemIds which are Below */
		std::set<unsigned int> itemsBelow;

		/** Contains all ItemIds which are Above */
		std::set<unsigned int> itemsAbove;

		/** Current load per Area Unit */
		std::vector<double> load;

		// Functions
		/**
		* Sets the position of the Item.
		*
		* @param point the minimum corner point
		*/
		void setPosition(Point point);

		/**
		* Is this item below another item.
		*
		* @param other the other item
		* @param touch is touching surface required
		* @return true if item is below another item.
		*/
		bool isBelow(Item& other, const bool touch) const;

		/**
		* Is this item above another item.
		*
		* @param other the other item
		* @param touch is touching surface required
		* @return true if item is above another item.
		*/
		bool isAbove(Item& other, const bool touch);

		/**
		* Is this item behind another item below.
		*
		* @param other the other item
		* @return true if item is behind another item.
		*/
		bool isBehind(Item& other);

		/**
		* Is this item in front of another item.
		*
		* @param other the other item
		* @return true if item is in front of another item.
		*/
		bool isInFront(Item& other);
	};
}
