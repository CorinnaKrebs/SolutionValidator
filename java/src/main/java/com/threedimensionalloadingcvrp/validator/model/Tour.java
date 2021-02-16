package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.util.List;

@Data
public class Tour {
    private int id;
    private List<Integer> customer_ids;     // Sequence of Customer Ids in Visiting Order
    private List<Integer> item_ids;		    // Sequence of Item Ids in Constraints.Loading Order

    private double	sum_T;				    // Current sum of torques for rear axle
    private double	sum_F;				    // Current sum of force
    private double	mass_R;				    // Current load of the right vehicle side
    private double	mass_L;				    // Current load of the left vehicle side
    private boolean	feasible;			    // Feasibility of Tour (0: infeasible, 1: feasible)

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
