package com.threedimensionalloadingcvrp.validator.model;

import lombok.Data;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;

/**
 * The type Item.
 */
@Data
public class Item {
    /** Unique Id */
    private int id;

    /** ItemType Id */
    private int type_id;

    /** Customer Id */
    private int customer_id;

    /** Rotation Flag (1: rotated (l = w, w = l)) */
    private boolean rotated;

    /** Length */
    private int l;

    /** Width */
    private int w;

    /** Height */
    private int h;

    /** Mass */
    private double mass;

    /** Fragility flag (1: fragile) */
    private boolean fragile;

    /** Maximum Load per Area (Load Bearing Strength) */
    private double lbs;

    /** Total Load */
    private double sumLoad;

    /** Position of Item, leftMost, deepest, bottommost corner */
    private Point min;

    /** Maximum Corner of Item */
    private Point max;

    // Calculated Members

    /** Percentage of direct Support of Items */
    private double support;

    /** Contains all ItemIds which are in Front */
    private HashSet<Integer> itemsFront;

    /** Contains all ItemIds which are Behind */
    private HashSet<Integer> itemsBehind;

    /** Contains all ItemIds which are Below */
    private HashSet<Integer> itemsBelow;

    /** Contains all ItemIds which are Above */
    private HashSet<Integer> itemsAbove;

    /** Current load per Area Unit */
    private List<Double> load;

    /**
     * Instantiates a new Item.
     *
     * @param id          the unique item id
     * @param customer_id the customer id
     * @param rotated     the rotation flag
     * @param type        the ItemType
     */
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

    // Functions
    /**
     * Sets the position of the Item.
     *
     * @param point the minimum corner point
     */
    public void setPosition(final Point point) {
        min = point;
        int maxX = !rotated ? min.getX() + l : min.getX() + w;
        int maxY = !rotated ? min.getY() + w : min.getY() + l;
        int maxZ = min.getZ() + h;
        max = new Point(maxX, maxY, maxZ);
    }

    /**
     * Is this item below another item.
     *
     * @param other the other item
     * @param touch is touching surface required
     * @return true if item is below another item.
     */
    public boolean isBelow(Item other, final boolean touch) {
        return (((touch && max.getZ() == other.min.getZ())
                || (!touch && max.getZ() <= other.min.getZ()))
		        && min.getY()       < other.max.getY()
		        && other.min.getY() < max.getY()
                && min.getX()       < other.max.getX()
		        && other.min.getX() < max.getX());
    }


    /**
     * Is this item above another item.
     *
     * @param other the other item
     * @param touch is touching surface required
     * @return true if item is above another item.
     */
    public boolean isAbove(Item other, final boolean touch) {
        return other.isBelow(this, touch);
    }

    /**
     * Is this item behind another item below.
     *
     * @param other the other item
     * @return true if item is behind another item.
     */
    public boolean isBehind(Item other) {
        return
                max.getX() <= other.min.getX() &&
                max.getZ() > other.min.getZ() &&
                min.getZ() < other.max.getZ() &&
                max.getY() > other.min.getY() &&
                min.getY() < other.max.getY();
    }

    /**
     * Is this item in front of another item.
     *
     * @param other the other item
     * @return true if item is in front of another item.
     */
    public boolean isInFront(Item other) {
        return other.isBehind(this);
    }
}
