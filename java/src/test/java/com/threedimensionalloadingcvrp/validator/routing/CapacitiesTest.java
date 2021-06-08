package com.threedimensionalloadingcvrp.validator.routing;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;

import static com.threedimensionalloadingcvrp.validator.constraints.Routing.checkCapacities;


public class CapacitiesTest {
    @Test
    public void LoadVolumeCheck() {
        int max_vol = 250;
        int c1_vol = 100;
        int c2_vol = 200;

        Customer depot = new Customer(0, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer1 = new Customer(1, 0, 0, 0, 0, 0, 0, 0, c1_vol);
        Customer customer2 = new Customer(2, 0, 0, 0, 0, 0, 0, 0, c2_vol);

        // Instance Creation
        Vehicle vehicle = new Vehicle(0, 0, 0, 0, max_vol, 0, 0, 0, 0);
        Instance instance = new Instance("", vehicle, null, Arrays.asList(depot, customer1, customer2), 0, false, null);

        // Tour Creation
        Tour tour1 = new Tour(1, Arrays.asList(1, 2), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1));

        // Test
        Assertions.assertFalse(checkCapacities(solution, true, instance, false));
    }

    @Test
    public void LoadMassCheck() {
        int D = 30;
        int c1_mass = 20;
        int c2_mass = 20;

        Customer depot = new Customer(0, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer1 = new Customer(1, 0, 0, 0, 0, 0, 0, c1_mass, 0);
        Customer customer2 = new Customer(2, 0, 0, 0, 0, 0, 0, c2_mass, 0);

        // Instance Creation
        Vehicle vehicle = new Vehicle(0, 0, 0, D, 0, 0, 0, 0, 0);
        Instance instance = new Instance("", vehicle, null, Arrays.asList(depot, customer1, customer2), 0, false, null);

        // Tour Creation
        Tour tour1 = new Tour(1, Arrays.asList(1, 2), null);

        // Solution Creation
        Solution solution = new Solution(Collections.singletonList(tour1));

        // Test
        Assertions.assertFalse(checkCapacities(solution, true, instance, false));
    }

    @Test
    public void Feasible() {
        int D = 50;
        int c1_mass = 20;
        int c2_mass = 20;

        int max_vol = 500;
        int c1_vol = 100;
        int c2_vol = 200;

        Vehicle vehicle = new Vehicle(0, 0, 0, D, max_vol, 0, 0, 0, 0);
        Customer depot = new Customer(0, 0f, 0f, 0, 0, 0, 0, 0.0, 0);
        Customer customer1 = new Customer(1, 0, 0, 0, 0, 0, 0, c1_mass, c1_vol);
        Customer customer2 = new Customer(2, 0, 0, 0, 0, 0, 0, c2_mass, c2_vol);

        // Instance Creation
        Instance instance = new Instance("", vehicle, null, Arrays.asList(depot, customer1, customer2), 0, false, null);

        // Tour Creation
        Tour tour1 = new Tour(1, Arrays.asList(1, 2), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1));

        // Test
        Assertions.assertTrue(checkCapacities(solution, true, instance, false));
    }
}
