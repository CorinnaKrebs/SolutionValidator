package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.*;

public class LBSTest {
    private final int dim = 2;

    @Test
    public void OverhangingTest() {
        // Two Items on the Ground
        // Third Item is stacked on them and is overhanging over by 1/4
        // Test checks calculation

        int mass = 2;

        ItemType typeNormal = new ItemType(1, dim,    dim, dim, mass, false, 2.0);
        ItemType typeLong   = new ItemType(2, dim, 2* dim, dim, mass, false, 2.0);

        // Bottom
        Item itemBottomLeft  = new Item(0, 1, false, typeNormal);
        Item itemBottomRight = new Item(1, 1, false, typeNormal);
        Item itemTop         = new Item(2, 1, false, typeLong);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemBottomLeft, itemBottomRight, itemTop));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2));

        // Set Positions
        instance.getItems().get(itemBottomLeft.getId()) .setPosition(new Point(0, 0,   0));
        instance.getItems().get(itemBottomRight.getId()).setPosition(new Point(0, dim, 0));
        instance.getItems().get(itemTop.getId())        .setPosition(new Point(0, 1,   dim));

		final int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Calculates the loads for the entire stack, begin with top Item.
        Assertions.assertTrue(checkLBSSimple(instance.getItems().get(itemTop.getId()), instance, true));


        Assertions.assertEquals(0.75, instance.getItems().get(itemTop.getId()).getSupport());
		final double support_top = 0.75;
        final double partLeft  = 0.25 / support_top;
        final double partRight = 0.50 / support_top;
        Assertions.assertEquals(1.0, partLeft + partRight);
        Assertions.assertEquals(support_top, instance.getItems().get(itemTop.getId()).getSupport());
        Assertions.assertEquals(mass * partLeft,  instance.getItems().get(itemBottomLeft.getId()).getSumLoad());
        Assertions.assertEquals(mass * partRight, instance.getItems().get(itemBottomRight.getId()).getSumLoad());
    }

    @Test
    public void LBSCompleteTest() {
        // Arrangement of the items like a pyramid
        // Same Arrangement as for LBSSimple

        ItemType type = new ItemType(1, dim, dim, dim, 2, false, 1.0);

        // Bottom
        Item itemBottomLeft   = new Item(0, 1, false, type);
        Item itemBottomCenter = new Item(1, 1, false, type);
        Item itemBottomRight  = new Item(2, 1, false, type);

        // Second Row
        Item itemCenterLeft  = new Item(3, 1, false, type);
        Item itemCenterRight = new Item(4, 1, false, type);

        // Top Row
        Item itemTop = new Item(5, 1, false, type);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false,
                Arrays.asList(itemBottomLeft, itemBottomCenter, itemBottomRight, itemCenterLeft, itemCenterRight, itemTop));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2, 3, 4, 5));

        // Positions
        instance.getItems().get(itemBottomLeft.getId())  .setPosition(new Point(0, 0,             0));
        instance.getItems().get(itemBottomCenter.getId()).setPosition(new Point(0, dim,           0));
        instance.getItems().get(itemBottomRight.getId()) .setPosition(new Point(0, 2 * dim,       0));
        instance.getItems().get(itemCenterLeft.getId())  .setPosition(new Point(0, dim/2,         dim));
        instance.getItems().get(itemCenterRight.getId()) .setPosition(new Point(0, ((int) dim + dim/2),   dim));
        instance.getItems().get(itemTop.getId())         .setPosition(new Point(0, dim,       2 * dim));

		final int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Calculates the loads for the entire stack, begin with top Item.
        Assertions.assertTrue(checkLBSComplete(instance.getItems().get(itemTop.getId()),         instance, instance.getItems().get(itemTop.getId()).getMass(), true));
        Assertions.assertTrue(checkLBSComplete(instance.getItems().get(itemCenterLeft.getId()),  instance, instance.getItems().get(itemCenterLeft.getId()).getMass(), true));
        Assertions.assertTrue(checkLBSComplete(instance.getItems().get(itemCenterRight.getId()), instance, instance.getItems().get(itemCenterRight.getId()).getMass(), true));

        Assertions.assertEquals(1.0, instance.getItems().get(itemCenterLeft.getId())  .getSumLoad());
        Assertions.assertEquals(1.0, instance.getItems().get(itemCenterRight.getId()) .getSumLoad());
        Assertions.assertEquals(1.5, instance.getItems().get(itemBottomLeft.getId())  .getSumLoad());
        Assertions.assertEquals(3.0, instance.getItems().get(itemBottomCenter.getId()).getSumLoad());
        Assertions.assertEquals(1.5, instance.getItems().get(itemBottomRight.getId()) .getSumLoad());

    }

    @Test
    public void LBSSimpleTest() {
        // Arrangement of the items like a pyramid
        // Same Arrangement as for LBSComplete
        ItemType type = new ItemType(1, dim, dim, dim, 2, false, 1.0);

        // Bottom
        Item itemBottomLeft = new Item(0, 1, false, type);
        Item itemBottomCenter = new Item(1, 1, false, type);
        Item itemBottomRight = new Item(2, 1, false, type);

        // Second Row
        Item itemCenterLeft = new Item(3, 1, false, type);
        Item itemCenterRight = new Item(4, 1, false, type);

        // Top Row
        Item itemTop = new Item(5, 1, false, type);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false,
                Arrays.asList(itemBottomLeft, itemBottomCenter, itemBottomRight, itemCenterLeft, itemCenterRight, itemTop));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2, 3, 4, 5));

        // Positions
        instance.getItems().get(itemBottomLeft.getId())  .setPosition(new Point(0, 0,           0));
        instance.getItems().get(itemBottomCenter.getId()).setPosition(new Point(0, dim,         0));
        instance.getItems().get(itemBottomRight.getId()) .setPosition(new Point(0, 2 * dim,     0));
        instance.getItems().get(itemCenterLeft.getId())  .setPosition(new Point(0, dim/2,       dim));
        instance.getItems().get(itemCenterRight.getId()) .setPosition(new Point(0, dim + dim/2, dim));
        instance.getItems().get(itemTop.getId())         .setPosition(new Point(0, dim,         2 * dim));

		final int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Calculates the loads for the entire stack, begin with top Item.
        Assertions.assertTrue(checkLBSSimple(instance.getItems().get(itemTop.getId()),         instance, true));
        Assertions.assertTrue(checkLBSSimple(instance.getItems().get(itemCenterLeft.getId()),  instance, true));
        Assertions.assertTrue(checkLBSSimple(instance.getItems().get(itemCenterRight.getId()), instance, true));

        Assertions.assertEquals(1.0, instance.getItems().get(itemCenterLeft.getId())  .getSumLoad());
        Assertions.assertEquals(1.0, instance.getItems().get(itemCenterRight.getId()) .getSumLoad());
        Assertions.assertEquals(1.0, instance.getItems().get(itemBottomLeft.getId())  .getSumLoad());
        Assertions.assertEquals(4.0, instance.getItems().get(itemBottomCenter.getId()).getSumLoad());
        Assertions.assertEquals(1.0, instance.getItems().get(itemBottomRight.getId()) .getSumLoad());

    }
}
