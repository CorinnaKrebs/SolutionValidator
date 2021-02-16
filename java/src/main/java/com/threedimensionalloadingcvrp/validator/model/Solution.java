package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.util.List;

@AllArgsConstructor
@Data
public class Solution {
    public Solution(final List<Tour> tours) {
        this.tours = tours;
        this.feasible = false;
        this.total_travel_distance = 0;
        this.total_iterations = 0;
        this.calculation_time = 0;
    }

    private List<Tour> tours;
    private double     total_travel_distance;
    private boolean    feasible;
    private int        total_iterations;
    private double     calculation_time;
}
