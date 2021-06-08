package com.threedimensionalloadingcvrp.validator.model;

import lombok.Getter;

import java.util.ArrayList;
import java.util.List;

/**
 * The type Customer.
 */
@Getter
public class Customer {

    // Members
    /** the unique id */
    private final int id;

    /** the x-Position */
    private final float x;

    /** the y-Position */
    private final float y;

    /** the total number of demanded items */
    private final int demand;

    /** ready time */
    private final int readyTime;

    /** due date */
    private final int dueDate;

    /** service time */
    private final int serviceTime;

    /** the total mass of all demanded items */
    private final double demandedMass;

    /** the total volume of all demanded items */
    private final double demandedVolume;

    /** all demanded Item ids */
    private List<Integer> item_ids;

    /** demand (itemType, quantity) */
    private List<Demand>  demands;


    /**
     * Instantiates a new Customer.
     *
     * @param id             the unique id
     * @param x              the x-Position
     * @param y              the y-Position
     * @param demand         the total number of demanded items
     * @param readyTime      the ready time
     * @param dueDate        the due date
     * @param serviceTime    the service time
     * @param demandedMass   the total mass of all demanded items
     * @param demandedVolume the total volume of all demanded items
     */
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
}
