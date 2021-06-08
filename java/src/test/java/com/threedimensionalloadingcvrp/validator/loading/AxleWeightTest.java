package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import java.util.Collections;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkAxleWeights;

public class AxleWeightTest {
    private final int dim = 2;
    private final int mass = 90;
    private final int lF = 4;
    private Instance instance;
    private Tour tour;

    @BeforeEach
    public void init() {
        ItemType type = new ItemType (1, dim, dim, dim, mass, false, 0);
        Item item = new Item(0, 1, false, type);

        // Instance Creation
        Vehicle vehicle = new Vehicle(60, 25, 30, 90, 0, 48, 50, 82, lF);
        instance = new Instance("", vehicle, null, null, 0, false, Collections.singletonList(item));

        // Tour Creation
        tour = new Tour (1, null, null);
    }

    @Test
    public void FrontAxleExceeding() {

        // Positions
        instance.getItems().get(0).setPosition(new Point(0, 0, 0));
        
        // Tests
        boolean result = checkAxleWeights(instance.getItems().get(0), tour, true, instance, true);
        Assertions.assertFalse(result);
        Assertions.assertEquals(mass * 9.81, tour.getSum_F(), 1);
        Assertions.assertEquals(mass * 9.81 * (lF + dim / 2.0), tour.getSum_T(), 1);
    }

    @Test
    public void RearAxleExceeding() {
        // Positions
        instance.getItems().get(0).setPosition(new Point(55, 0, 0));

        // Tests
        boolean result = checkAxleWeights(instance.getItems().get(0), tour, true, instance, true);
        Assertions.assertFalse(result);
    }

    @Test
    public void feasible() {
        // Positions
        instance.getItems().get(0).setPosition(new Point(20, 0, 0));

        // Tests
        Assertions.assertTrue(checkAxleWeights(instance.getItems().get(0), tour, true, instance, true));
    }
}
