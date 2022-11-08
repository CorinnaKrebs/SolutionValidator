package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.*;

public class MultipleOverhangingTest {
    float alpha = 0.75f;

    @Test
    public void FeasiblePosition() {
        // Arrangement consisting of two Towers
        // First Tower: Two Items
        // Second Tower: High Item
        // On Top: an Overhanging Item
        int dim = 6;

        ItemType typeNormal = new ItemType(1, dim, dim, dim,        0, false, 0);
        ItemType typeHigh   = new ItemType(2, dim, dim, 2 * dim,    0, false, 0);
        ItemType typeTop    = new ItemType(3, dim, 2 * dim +1, dim, 0, false, 0);

        Item itemNormal1 = new Item(0, 1, Rotation.none, typeNormal);
        Item itemNormal2 = new Item(1, 1, Rotation.none, typeNormal);
        Item itemHigh    = new Item(2, 1, Rotation.none, typeHigh);
        Item itemTop     = new Item(3, 1, Rotation.none, typeTop);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemNormal1, itemNormal2, itemHigh, itemTop));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2, 3));

        // Positions
        instance.getItems().get(itemNormal1.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(itemNormal2.getId()).setPosition(new Point(0, 0, dim));
        instance.getItems().get(itemHigh.getId())   .setPosition(new Point(0, dim, 0));
        instance.getItems().get(itemTop.getId())    .setPosition(new Point(0, 0, 2* dim));

        // Tests
        int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        Assertions.assertTrue(checkMultipleOverhanging(instance.getItems().get(itemTop.getId()), alpha, instance, true));
    }

    @Test
    public void InfeasiblePosition() {
        // Arrangement consisting of three stacked items,
        // which increase by 1/alpa per level

        int dim = 3;
        int w = 2 * dim;

        ItemType typeBottom = new ItemType(1, dim, w,                 dim, 0, false, 0);
        ItemType typeWide   = new ItemType(2, dim, (int) (w/alpha),           dim, 0, false, 0);
        ItemType typeTop    = new ItemType(3, dim, (int) (w/(alpha * alpha)), dim, 0, false, 0);

        Item itemBottom = new Item(0, 1, Rotation.none, typeBottom);
        Item itemWide   = new Item(1, 1, Rotation.none, typeWide);
        Item itemTop    = new Item(2, 1, Rotation.none, typeTop);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemBottom, itemWide, itemTop));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2));

        // Positions
        instance.getItems().get(itemBottom.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(itemWide.getId())  .setPosition(new Point(0, 0, dim));
        instance.getItems().get(itemTop.getId())   .setPosition(new Point(0, 0, 2 * dim));

		int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Tests
        Assertions.assertTrue(checkMinimalSupport(instance.getItems().get(itemWide.getId()), alpha, instance, true));
        Assertions.assertTrue(checkMinimalSupport(instance.getItems().get(itemTop.getId()),  alpha, instance, true));

        Assertions.assertFalse(checkMultipleOverhanging(instance.getItems().get(itemTop.getId()), alpha, instance, true));
    }
}
