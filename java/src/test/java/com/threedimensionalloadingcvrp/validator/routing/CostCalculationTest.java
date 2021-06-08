package com.threedimensionalloadingcvrp.validator.routing;

import com.threedimensionalloadingcvrp.validator.constraints.Routing;
import com.threedimensionalloadingcvrp.validator.model.Customer;
import com.threedimensionalloadingcvrp.validator.model.Instance;
import com.threedimensionalloadingcvrp.validator.model.Solution;
import com.threedimensionalloadingcvrp.validator.model.Tour;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

public class CostCalculationTest {

    @Test
    public void CorrectCost() {
        final int cust1_x   = 10;
        final int cust2_x  = 20;
        final int cust3_x  = 25;

        Customer depot     = new Customer(0, 0,    0, 0, 0, 0, 0, 0, 0);
        Customer customer1 = new Customer(1, cust1_x,  0, 0, 0, 0, 0, 0, 0);
        Customer customer2 = new Customer(2, cust2_x, 0, 0, 0, 0, 0, 0, 0);
        Customer customer3 = new Customer(3, cust3_x, 0, 0, 0, 0, 0, 0, 0);

        // Create Instance
        Instance instance = new Instance("", null, null, Arrays.asList(depot, customer1, customer2, customer3), 2, false);

        // Tours Creation
        Tour tour1 = new Tour(1, Arrays.asList(1), null);
        Tour tour2 = new Tour(2, Arrays.asList(2, 3), null);

        double distance01 = Routing.calcEuclideanDistance(depot.getX(), depot.getY(), customer1.getX(), customer1.getY());
        double distance02 = Routing.calcEuclideanDistance(depot.getX(), depot.getY(), customer2.getX(), customer2.getY());
        double distance23 = Routing.calcEuclideanDistance(customer2.getX(), customer2.getY(), customer3.getX(), customer3.getY());
        double distance30 = Routing.calcEuclideanDistance(customer3.getX(), customer3.getY(), depot.getX(), depot.getY());

        double cost = 2 * distance01 + distance02 + distance23 + distance30;

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2));
        solution.setTotal_travel_distance(cost);
        Assertions.assertTrue(Routing.checkCost(solution, instance, false));
    }

    @Test
    public void WrongCost() {
        final int cust1_x   = 10;
        final int cust2_x  = 20;
        final int cust3_x  = 25;

        Customer depot     = new Customer(0, 0,    0, 0, 0, 0, 0, 0, 0);
        Customer customer1 = new Customer(1, cust1_x,  0, 0, 0, 0, 0, 0, 0);
        Customer customer2 = new Customer(2, cust2_x, 0, 0, 0, 0, 0, 0, 0);
        Customer customer3 = new Customer(3, cust3_x, 0, 0, 0, 0, 0, 0, 0);

        // Create Instance
        Instance instance = new Instance("", null, null, Arrays.asList(depot, customer1, customer2, customer3), 2, false);

        // Tours Creation
        Tour tour1 = new Tour(1, Arrays.asList(1), null);
        Tour tour2 = new Tour(2, Arrays.asList(2, 3), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2));
        solution.setTotal_travel_distance(1);
        Assertions.assertFalse(Routing.checkCost(solution, instance, false));
    }
}
