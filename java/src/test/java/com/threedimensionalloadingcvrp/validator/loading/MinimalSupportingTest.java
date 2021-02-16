package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkMinimalSupport;
import static com.threedimensionalloadingcvrp.validator.constraints.Loading.getRelevantItems;

public class MinimalSupportingTest {
    private float alpha = 0.75f;
    private int dim = 8;

    @Test
    public void InfeasiblePosition() {
        // Stacked Item is hanging over by 1/2

        ItemType type = new ItemType(1, dim, dim, dim, 0, false, 0);

        Item itemBottom = new Item(0, 1, false, type);
        Item itemTop = new Item(1, 1, false, type);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemBottom, itemTop));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1));

        instance.getItems().get(0).setPosition(new Point(0, 0, 0));
        instance.getItems().get(1).setPosition(new Point(0, dim / 2, dim));

        final int endPos = tour.getItem_ids().size();
        getRelevantItems (tour, endPos, instance);
        getRelevantItems (tour, endPos, instance);

        Assertions.assertTrue (checkMinimalSupport(instance.getItems().get(0), alpha, instance, true));
        Assertions.assertFalse(checkMinimalSupport(instance.getItems().get(1), alpha, instance, true));
        Assertions.assertEquals (1.0, instance.getItems().get(0).getSupport());
        Assertions.assertEquals (0.5, instance.getItems().get(1).getSupport());
    }

    @Test
    public void FeasiblePosition() {
        // Stacked Item is hanging over by 1/4

        ItemType type = new ItemType(1, dim, dim, dim, 0, false, 0);

        Item itemBottom = new Item(0, 1, false, type);
        Item itemTop = new Item(1, 1, false, type);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemBottom, itemTop));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1));

        instance.getItems().get(0).setPosition(new Point(0, 0, 0));
        instance.getItems().get(1).setPosition(new Point(0, dim / 4, dim));

        final int endPos = tour.getItem_ids().size();
        getRelevantItems (tour, endPos, instance);
        getRelevantItems (tour, endPos, instance);

        Assertions.assertTrue(checkMinimalSupport(instance.getItems().get(0), alpha, instance, true));
        Assertions.assertTrue(checkMinimalSupport(instance.getItems().get(1), alpha, instance, true));
        Assertions.assertEquals (1.0, instance.getItems().get(0).getSupport());
        Assertions.assertEquals (0.75, instance.getItems().get(1).getSupport());
    }
}
