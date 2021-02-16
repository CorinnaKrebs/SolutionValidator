package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkTopOverhanging;
import static com.threedimensionalloadingcvrp.validator.constraints.Loading.getRelevantItems;

public class TopOverhangingTest {
    private float alpha = 0.75f;
    private int dim = 4;
    @Test
    public void InfeasiblePosition() {
        // 3rd Item is Overhanging, 4 Items in Stack
        ItemType type = new ItemType(1, dim, dim, dim, 0, false, 0);
        ItemType typeLong = new ItemType(1, (int) (1.25 * dim), dim, dim, 0, false, 0);

        Item item1 = new Item(0, 1, false, type);
        Item item2 = new Item(1, 1, false, type);
        Item item3 = new Item(2, 1, false, typeLong);
        Item item4 = new Item(3, 1, false, type);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(item1, item2, item3, item4));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2, 3));

        // Positions
        instance.getItems().get(item1.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(item2.getId()).setPosition(new Point(0, 0, 1 * dim));
        instance.getItems().get(item3.getId()).setPosition(new Point(0, 0, 2 * dim));
        instance.getItems().get(item4.getId()).setPosition(new Point(0, 0, 3 * dim));

		final int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Tests
        Assertions.assertTrue (checkTopOverhanging(instance.getItems().get(item1.getId()), alpha, instance, true));
        Assertions.assertTrue (checkTopOverhanging(instance.getItems().get(item2.getId()), alpha, instance, true));
        Assertions.assertFalse(checkTopOverhanging(instance.getItems().get(item3.getId()), alpha, instance, true));
        Assertions.assertTrue (checkTopOverhanging(instance.getItems().get(item4.getId()), alpha, instance, true));
        Assertions.assertEquals(1.0, instance.getItems().get(item1.getId()).getSupport());
        Assertions.assertEquals(1.0, instance.getItems().get(item2.getId()).getSupport());
        Assertions.assertEquals(1.0, instance.getItems().get(item4.getId()).getSupport());
        Assertions.assertTrue(instance.getItems().get(item3.getId()).getSupport() > alpha);
    }

    @Test
    public void FeasiblePosition() {
        // Top Item (4th) is Overhanging
        ItemType type     = new ItemType(1, dim, dim, dim, 0, false, 0);
        ItemType typeLong = new ItemType(1, (int) (1.25 * dim), dim, dim, 0, false, 0);

        Item item1 = new Item(0, 1, false, type);
        Item item2 = new Item(1, 1, false, type);
        Item item3 = new Item(2, 1, false, type);
        Item item4 = new Item(3, 1, false, typeLong);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(item1, item2, item3, item4));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2, 3));

        // Positions
        instance.getItems().get(item1.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(item2.getId()).setPosition(new Point(0, 0, 1 * dim));
        instance.getItems().get(item3.getId()).setPosition(new Point(0, 0, 2 * dim));
        instance.getItems().get(item4.getId()).setPosition(new Point(0, 0, 3 * dim));

		int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Tests
        Assertions.assertTrue(checkTopOverhanging(instance.getItems().get(item1.getId()), alpha, instance, true));
        Assertions.assertTrue(checkTopOverhanging(instance.getItems().get(item2.getId()), alpha, instance, true));
        Assertions.assertTrue(checkTopOverhanging(instance.getItems().get(item3.getId()), alpha, instance, true));
        Assertions.assertTrue(checkTopOverhanging(instance.getItems().get(item4.getId()), alpha, instance, true));
        Assertions.assertEquals(1.0, instance.getItems().get(item1.getId()).getSupport());
        Assertions.assertEquals(1.0, instance.getItems().get(item2.getId()).getSupport());
        Assertions.assertEquals(1.0, instance.getItems().get(item3.getId()).getSupport());
        Assertions.assertTrue(instance.getItems().get(item4.getId()).getSupport() > alpha);
    }
}
