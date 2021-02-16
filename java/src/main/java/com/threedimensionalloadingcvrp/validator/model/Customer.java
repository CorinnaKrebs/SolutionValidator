package com.threedimensionalloadingcvrp.validator.model;

import lombok.Getter;

import java.util.ArrayList;
import java.util.List;

@Getter
public class Customer {
    public Customer(final int id, final float x, final float y, final int demand, final int readyTime, final int dueDate, final int serviceTime, final double demandedMass, final double demandedVolume) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.demand = demand;
        this.readyTime = readyTime;
        this.dueDate = dueDate;
        this.serviceTime = serviceTime;
        this.demandedMass = demandedMass;
        this.demandedVolume = demandedVolume;
        this.item_ids = new ArrayList<>();
        this.demands = new ArrayList<>();
    }

    // Members
	private final int id;					    // Unique Id
    private final float x;				        // x-Position of Customer
    private final float y;				        // y-Position of Customer
    private final int demand;				    // Total Demand of Items
    private final int readyTime;			    // ReadyTime
    private final int dueDate;			        // DueDate
    private final int serviceTime;		        // ServiceTime
    private final double demandedMass;	        // Total Mass   of all demanded Items
    private final double demandedVolume;	    // Total Volume of all demanded Items

    private List<Integer> item_ids;	            // All demanded Item Ids
    private List<Demand>  demands;              // Demand (Itemtype, Quantity)
}
