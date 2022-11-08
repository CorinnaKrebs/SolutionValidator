package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkOverlapping;

public class OverlappingTest {
    private int dim = 5; // > 1

    @Test
    public void OverlappingCheck() {

        ItemType itemType = new ItemType(1, dim, dim, dim, 0, false, 0);
        Item item1 = new Item(0, 1, Rotation.none, itemType);
        Item item2 = new Item(1, 1, Rotation.none, itemType);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(item1, item2));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1));

        // Position
        instance.getItems().get(0).setPosition(new Point(0, 0, 0));
        int endPos = tour.getItem_ids().size();

        // Overlapping Right Side of Item 1
        instance.getItems().get(1).setPosition(new Point(0, 1, 0));
        Assertions.assertFalse(checkOverlapping(item1, tour, endPos, instance, true));

        // Overlapping Front Side of Item 1
        instance.getItems().get(1).setPosition(new Point(1, 0, 0));
        Assertions.assertFalse(checkOverlapping(item1, tour, endPos, instance, true));

        // Overlapping Top Side of Item 1
        instance.getItems().get(1).setPosition(new Point(0, 0, 1));
        Assertions.assertFalse(checkOverlapping(item1, tour, endPos, instance, true));

        // Still Overlapping
        instance.getItems().get(1).setPosition(new Point(1, 1, 1));
        Assertions.assertFalse(checkOverlapping(item1, tour, endPos, instance, true));

        // Feasible Position
        instance.getItems().get(1).setPosition(new Point(0, dim, 0));
        Assertions.assertTrue(checkOverlapping(item1, tour, endPos, instance, true));
    }
}
