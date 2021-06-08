package com.threedimensionalloadingcvrp.validator.model;

import lombok.Data;

import java.util.List;

/**
 * The type Tour.
 */
@Data
public class Tour {

    /** Unique id */
    private int id;

    /** Sequence of Customer Ids in Visiting Order */
    private List<Integer> customer_ids;

    /** Sequence of Item Ids */
    private List<Integer> item_ids;

    /** Current sum of torques */
    private double	sum_T;

    /** Current sum of forces */
    private double	sum_F;

    /** Current load of the right vehicle side */
    private double	mass_R;

    /** Current load of the left vehicle side */
    private double	mass_L;

    /** Feasibility of Tour (0: infeasible, 1: feasible) */
    private boolean	feasible;

    /**
     * Instantiates a new Tour.
     *
     * @param id           the Unique id
     * @param customer_ids the Sequence of Customer Ids in Visiting Order
     * @param item_ids     the Sequence of Item Ids
     */
    public Tour(final int id, final List<Integer> customer_ids, final List<Integer> item_ids) {
        this.id = id;
        this.customer_ids = customer_ids;
        this.item_ids = item_ids;
        this.sum_F = 0;
        this.sum_T = 0;
        this.mass_L = 0;
        this.mass_R = 0;
        this.feasible = false;
    }
}
