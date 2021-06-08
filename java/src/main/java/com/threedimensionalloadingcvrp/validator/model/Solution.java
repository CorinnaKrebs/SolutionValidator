package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.util.List;

/**
 * The type Solution.
 */
@AllArgsConstructor
@Data
public class Solution {
    /** the list of Tours */
    private List<Tour> tours;

    /** Total Travel Distance */
    private double     total_travel_distance;

    /** Feasibility of the Solution (1: feasible) */
    private boolean    feasible;

    /** Total Number of conducted Iterations */
    private int        total_iterations;

    /** Total Calculation Time */
    private double     calculation_time;

    /**
     * Instantiates a new Solution.
     *
     * @param tours the list of Tours
     */
    public Solution(final List<Tour> tours) {
        this.tours = tours;
        this.feasible = false;
        this.total_travel_distance = 0;
        this.total_iterations = 0;
        this.calculation_time = 0;
    }
}
