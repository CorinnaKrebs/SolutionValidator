package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;

@Data
public class Item {
    public Item(final int id, final int customer_id, final boolean rotated, ItemType type) {
        this.id = id;
        this.customer_id = customer_id;
        this.rotated = rotated;
        this.type_id = type.getId();
        this.l = type.getL();
        this.w = type.getW();
        this.h = type.getH();
        this.mass = type.getMass();
        this.fragile = type.isFragile();
        this.lbs = type.getLbs();
        min = new Point(0, 0, 0);
        max = new Point(0, 0, 0);
        this.support = 0;
        this.sumLoad = 0;
        this.load = new ArrayList<>(Collections.nCopies(this.l * this.w, 0.0));
        itemsAbove = new HashSet<>();
        itemsBelow = new HashSet<>();
        itemsFront = new HashSet<>();
        itemsBehind = new HashSet<>();
    }

    private int id;			    // Unique Id
    private int type_id;		// Type Id
    private int customer_id;	// Customer Id
    private boolean rotated;    // Rotation Flag (1: rotated (l = w, w = l))

    private int l;				// Length
    private int w;				// Width
    private int h;				// Height

    private double mass;		// Mass
    private boolean fragile;	// Fragility flag (1: fragile)
    private double lbs;			// Maximum Load per Area (Load Bearing Strength)
    private double sumLoad;		// Total Load

    private Point min;
    private Point max;

    // Calculated Members
    private double support;					// Percentage of direct Support of Items
    private HashSet<Integer> itemsFront;	// Contains all ItemIds which are in Front
    private HashSet<Integer> itemsBehind;	// Contains all ItemIds which are Behind
    private HashSet<Integer> itemsBelow;	// Contains all ItemIds which are Below
    private HashSet<Integer> itemsAbove;	// Contains all ItemIds which are Above

    private List<Double> load;				// Current load per Area Unit

    // Functions
    public void setPosition(final Point point) {
        min = point;
        int maxX = !rotated ? min.getX() + l : min.getX() + w;
        int maxY = !rotated ? min.getY() + w : min.getY() + l;
        int maxZ = min.getZ() + h;
        max = new Point(maxX, maxY, maxZ);
    }

    // Checks if other item is below
    public boolean isBelow(Item other, final boolean touch) {
        return (((touch && max.getZ() == other.min.getZ())
                || (!touch && max.getZ() <= other.min.getZ()))
		        && min.getY()       < other.max.getY()
		        && other.min.getY() < max.getY()
                && min.getX()       < other.max.getX()
		        && other.min.getX() < max.getX());
    }


    // Checks if other item is above
    public boolean isAbove(Item other, final boolean touch) {
        return other.isBelow(this, touch);
    }

    // Checks if other item is behind
    public boolean isBehind(Item other) {
        return
                max.getX() <= other.min.getX() &&
                max.getZ() > other.min.getZ() &&
                min.getZ() < other.max.getZ() &&
                max.getY() > other.min.getY() &&
                min.getY() < other.max.getY();
    }

    // Checks if other item is in front
    public boolean isInFront(Item other) {
        return other.isBehind(this);
    }
}
