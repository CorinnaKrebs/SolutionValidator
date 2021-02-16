package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Getter;

@Getter
@AllArgsConstructor
public class ItemType {
    ItemType() {
        this.id = 0;
        this.l = 0;
        this.w = 0;
        this.h = 0;
        this.mass = 0;
        this.fragile = false;
        this.lbs = 0;
    }

    private int id;		        // Unique Id
    private int l;		        // Length
    private int w;		        // Width
    private int h;		        // Height
	private double  mass;		// Mass
	private boolean fragile;	// Fragility flag, (1: fragile)
	private double  lbs;			// Maximum Load per Area (Load Bearing Strength)
}
