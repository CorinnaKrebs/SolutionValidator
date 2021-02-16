package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkBalancedLoading;

public class BalancedLoadingTest {
    private final int dim  = 5;
    private final int mass = 6;
    private Tour tour;
    private Instance instance;
    private final float balanced_part = 0.7f;

    @BeforeEach
    public void init() {
        ItemType type = new ItemType(1, dim, dim + 1, dim, mass, false, 0);

        Item item1 = new Item(0, 1, false, type);
        Item item2 = new Item(1, 1, false, type);
        Item item3 = new Item(2, 1, false, type);

        // Instance Creation
        Vehicle vehicle = new Vehicle(5 * dim, 2 * dim, dim, 3.0 * mass, 0, 0, 0, 0, 0);
        instance = new Instance("", vehicle, null, null, 0, false, Arrays.asList(item1, item2, item3));

        // Tour Creation
        tour = new Tour(1, null, null);

    }

    @Test
    public void LeftExceeding() {

        // All in Left Vehicle Half
        instance.getItems().get(0).setPosition(new Point(0, 0, 0));
        instance.getItems().get(1).setPosition(new Point(1 * dim, 0, 0));
        instance.getItems().get(2).setPosition(new Point(2 * dim, 0, 0));

        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(0), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(1.0 * mass, tour.getMass_L());
        Assertions.assertEquals(0.0, tour.getMass_R());
        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(1), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(2.0 * mass, tour.getMass_L());
        Assertions.assertEquals(0.0, tour.getMass_R());
        Assertions.assertFalse(checkBalancedLoading(instance.getItems().get(2), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(3.0 * mass, tour.getMass_L());
        Assertions.assertEquals(0.0, tour.getMass_R());
    }

    @Test
    public void RightExceeding() {
        // All in Right Vehicle Half
        instance.getItems().get(0).setPosition(new Point(0,       (int) (dim + 1.0), 0));
        instance.getItems().get(1).setPosition(new Point(1 * dim, (int) (dim + 1.0), 0));
        instance.getItems().get(2).setPosition(new Point(2 * dim, (int) (dim + 1.0), 0));

        // Tests
        float balanced_part = 0.7f;
        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(0), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(1.0 * mass, tour.getMass_R());
        Assertions.assertEquals(0.0, tour.getMass_L());
        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(1), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(2.0 * mass, tour.getMass_R());
        Assertions.assertEquals(0.0, tour.getMass_L());
        Assertions.assertFalse(checkBalancedLoading(instance.getItems().get(2), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(3.0 * mass, tour.getMass_R());
        Assertions.assertEquals(0.0, tour.getMass_L());
    }

    @Test
    public void DistributedExceeding() {
        int inRightSide = 1;
        // Mixed
        instance.getItems().get(0).setPosition(new Point(0, inRightSide, 0));
        instance.getItems().get(1).setPosition(new Point(1 * dim, inRightSide, 0));
        instance.getItems().get(2).setPosition(new Point(2 * dim, inRightSide, 0));

        // Tests
        double part = (double) inRightSide / dim;
        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(0), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(1.0 * (1.0 - part) * mass, tour.getMass_L(), 0.1);
        Assertions.assertEquals(1.0 *        part  * mass, tour.getMass_R(), 0.1);

        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(1), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(2.0 * (1.0 - part) * mass, tour.getMass_L(), 0.1);
        Assertions.assertEquals(2.0 *        part  * mass, tour.getMass_R(), 0.1);

        Assertions.assertFalse(checkBalancedLoading(instance.getItems().get(2), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(3.0 * (1.0 - part) * mass, tour.getMass_L(), 0.1);
        Assertions.assertEquals(3.0 *         part * mass, tour.getMass_R(), 0.1);
    }

    @Test
    public void feasible() {
        instance.getItems().get(0).setPosition(new Point(0, 0, 0));
        instance.getItems().get(1).setPosition(new Point(1 * dim, dim, 0));
        instance.getItems().get(2).setPosition(new Point(2 * dim, 0, 0));

        // Tests
        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(0), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(1.0 * mass, tour.getMass_L());
        Assertions.assertEquals(0.0, tour.getMass_R());
        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(1), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(1.0 * mass, tour.getMass_L());
        Assertions.assertEquals(1.0 * mass, tour.getMass_R());
        Assertions.assertTrue(checkBalancedLoading(instance.getItems().get(2), tour, true, balanced_part, instance, true));
        Assertions.assertEquals(2.0 * mass, tour.getMass_L());
        Assertions.assertEquals(1.0 * mass, tour.getMass_R());
    }
}