package com.threedimensionalloadingcvrp.validator.routing;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Routing.checkDispatchedCustomers;


public class DispatchedCustomersTests {
    @Test
    public void Customer1Twice() {
        // Instance Creation
        Instance instance = new Instance("", null, null, null, 3, false, null);

        // Tour Creation
        Tour tour1 = new Tour(1, Arrays.asList(2, 3), null);
        Tour tour2 = new Tour(2, Arrays.asList(1,4), null);
        Tour tour3 = new Tour(3, Arrays.asList(1), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2, tour3));

        // Test
        Assertions.assertFalse(checkDispatchedCustomers(solution, instance));
    }

    @Test
    public void Customer1NotDispatched() {
        Customer     depot = new Customer(0, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer1 = new Customer(1, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer2 = new Customer(2, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer3 = new Customer(3, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer4 = new Customer(4, 0, 0, 0, 0, 0, 0, 0, 0);

        // Instance Creation
        Instance instance = new Instance("", null, null,
                Arrays.asList(depot, customer1, customer2, customer3, customer4), 3, false, null);

        // Tour Creation
        Tour tour1 = new Tour(1, Arrays.asList(2, 3), null);
        Tour tour2 = new Tour(2, Arrays.asList(4), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2));

        // Test
        Assertions.assertFalse(checkDispatchedCustomers(solution, instance));
    }

    @Test
    public void TourOnlyDepot() {
        // Instance Creation
        Instance instance = new Instance("", null, null, null, 3, false, null);

        // Tour Creation
        Tour tour1 = new Tour(1, Arrays.asList(2, 3), null);
        Tour tour2 = new Tour(2, Arrays.asList(1, 4), null);
        Tour tour3 = new Tour(3, Arrays.asList(0), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2, tour3));

        // Test
        Assertions.assertFalse(checkDispatchedCustomers(solution, instance));
    }

    @Test
    public void Feasible() {
        Customer     depot = new Customer(0, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer1 = new Customer(1, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer2 = new Customer(2, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer3 = new Customer(3, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer4 = new Customer(4, 0, 0, 0, 0, 0, 0, 0, 0);


        // Instance Creation
        Instance instance = new Instance("", null, null,
                Arrays.asList(depot, customer1, customer2, customer3, customer4), 3, false, null);

        // Tour Creation
        Tour tour1 = new Tour(1, Arrays.asList(2, 3), null);
        Tour tour2 = new Tour(2, Arrays.asList(1, 4), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2));

        // Test
        Assertions.assertTrue(checkDispatchedCustomers(solution, instance));
    }
}
