package com.threedimensionalloadingcvrp.validator.routing;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Routing.*;

public class TimeWindowsTest {

    @Test
    public void DistanceCalculation() {
        final int x1 = 7;
        final int x2 = 10;

        final int y1 = 2;
        final int y2 = 6;

        double expected = 5.0; // sqrt(4² + 3²) = sqrt(16 + 9) = sqrt(25)
        Assertions.assertEquals(expected, calcEuclideanDistance(x1, y1, x2, y2));
    }
    @Test
    public void TWCalculation() {
        double time = 0;
        final int depot_ST = 15, depot_DD = 50;
        final int cust_x   = 10, cust_RT  = 30, cust_DD = 50, cust_ST = 10;
        final int cust2_x  = 20, cust2_RT = 30, cust2_DD = 60, cust2_ST = 15;

        Customer    depot = new Customer(0, 0,      0, 0, 0,       depot_DD, depot_ST, 0, 0);
        Customer customer = new Customer(1, cust_x, 0, 0, cust_RT, cust_DD,  cust_ST,  0, 0);
        Customer customer2 = new Customer(2, cust2_x, 0, 0, cust2_RT, cust2_DD, cust2_ST, 0, 0);

        // Instance Creation
        Instance instance = new Instance("", null, null, Arrays.asList(depot, customer, customer2), 0, true, null);

        // Tour Creation
        Tour tour = new Tour(1, Arrays.asList(1), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour));

        double distance01 = calcEuclideanDistance(0, 0, cust_x, 0);
        double distance12 = calcEuclideanDistance(cust_x, 0, cust2_x, 0);
        Assertions.assertEquals(10.0, distance01);
        Assertions.assertEquals(10.0, distance12);

        final double time_Diff = cust_RT - distance01 - depot_ST;

        Assertions.assertTrue(checkTimeWindows(solution, instance, false));

        time = calculateTime(1, 0, time, instance, false);
        Assertions.assertTrue(time > 0);
        double expected = depot_ST + distance01 + time_Diff + cust_ST;
        Assertions.assertEquals(expected, time);

        time = calculateTime(2, 1, time, instance, false);
        Assertions.assertTrue(time > 0);
        expected += distance12 + cust2_ST;
        Assertions.assertEquals(expected, time);
    }


    @Test
    public void ExceedingTW() {
        int depot_ST = 15;
        int depot_DD = 10; // Not Feasible

        int cust_x = 10;
        int cust_RT = 30;
        int cust_DD = 50;
        int cust_ST = 10;

        // Instance Creation
        Customer depot = new Customer(0, 0, 0, 0, 0, depot_DD, depot_ST, 0, 0);
        Customer customer = new Customer(1, cust_x, 0, 0, cust_RT, cust_DD, cust_ST, 0, 0);

        // Instance Creation
        Instance instance = new Instance("", null, null, Arrays.asList(depot, customer), 0, true, null);

        // Tour Creation
        Tour tour = new Tour(1, Arrays.asList(1), null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour));

        // Test
        Assertions.assertFalse(checkTimeWindows(solution, instance, false));
    }
}
