package com.threedimensionalloadingcvrp.validator.routing;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Routing.checkUsedVehicles;

public class UsedVehiclesTest {
    @Test
    public void ExceedingVMax() {
        final int v_max = 1;
        // Instance Creation
        Instance instance = new Instance("", null, null, null, v_max, true, null);

        // Tour Creation
        Tour tour1 = new Tour(1, null, null);
        Tour tour2 = new Tour(2, null, null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2));

        // Test
        Assertions.assertTrue(checkUsedVehicles(solution, instance, false));
    }

    @Test
    public void FeasibleVMax() {
        final int v_max = 2;

        // Instance Creation
        Instance instance = new Instance("", null, null, null, v_max, true, null);

        // Tour Creation
        Tour tour1 = new Tour(1, null, null);
        Tour tour2 = new Tour(2, null, null);

        // Solution Creation
        Solution solution = new Solution(Arrays.asList(tour1, tour2));

        // Test
        Assertions.assertTrue(checkUsedVehicles(solution, instance, false));
    }
}
