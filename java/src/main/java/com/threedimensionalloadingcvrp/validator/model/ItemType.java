package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * The type ItemType.
 */
@Getter
@AllArgsConstructor
public class ItemType {
    /** Unique Id */
    private final int id;

    /** Length */
    private final int l;

    /** Width */
    private final int w;

    /** Height */
    private final int h;

    /** Mass */
    private final double  mass;

    /** Fragility flag, (1: fragile) */
    private final boolean fragile;

    /** Maximum Load per Area (Load Bearing Strength) */
    private final double  lbs;


    /**
     * Instantiates a new Item type.
     */
    ItemType() {
        this.id = 0;
        this.l = 0;
        this.w = 0;
        this.h = 0;
        this.mass = 0;
        this.fragile = false;
        this.lbs = 0;
    }
}
