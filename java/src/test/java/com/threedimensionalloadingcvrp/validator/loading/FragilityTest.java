package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkFragility;
import static com.threedimensionalloadingcvrp.validator.constraints.Loading.getRelevantItems;

public class FragilityTest {
    private final int dim = 5;

    @Test
    public void InfeasiblePosition() {
        // Non-Fragile on Top of Fragile -> Not Feasible
        Item itemFragile = new Item(0, 1, Rotation.none, new ItemType (1,  dim, dim, dim, 0, true, 0));
        Item itemNormal  = new Item(1, 1, Rotation.none, new ItemType (1,  dim, dim, dim, 0, false, 0));

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemFragile, itemNormal));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1));

        // Positions
        instance.getItems().get(itemFragile.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(itemNormal.getId()). setPosition(new Point(0, 0, dim));

        final int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Tests
        Assertions.assertTrue (checkFragility(instance.getItems().get(itemFragile.getId()), tour, endPos, instance, true));
        Assertions.assertFalse(checkFragility(instance.getItems().get(itemNormal.getId()), tour, endPos, instance, true));
    }

    @Test
    public void FeasiblePositions() {
        ItemType typeNormal  = new ItemType(1,  dim, dim, dim, 0,  false, 0);
        ItemType typeFragile = new ItemType(2, dim, dim, dim, 0, true, 0);

        Item itemNormal1 = new Item(0, 1, Rotation.none, typeNormal);
        Item itemNormal2 = new Item(1, 1, Rotation.none, typeNormal);
        Item itemFragile1 = new Item(2, 1, Rotation.none, typeFragile);
        Item itemFragile2 = new Item(3, 1, Rotation.none, typeFragile);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemNormal1, itemNormal2, itemFragile1, itemFragile2));

        // Tour Creation
        Tour tour = new Tour(1, null, Arrays.asList(0, 1, 2, 3));

        // Non-Fragile on Top of Non-Fragile -> Feasible
        instance.getItems().get(itemNormal1.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(itemNormal2.getId()).setPosition(new Point(0, 0, dim));

        // Fragile on Top of Fragile -> Feasible
        instance.getItems().get(itemFragile1.getId()).setPosition(new Point(0, dim, 0));
        instance.getItems().get(itemFragile2.getId()).setPosition(new Point(0, dim, dim));

        final int endPos = tour.getItem_ids().size();
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);
        getRelevantItems(tour, endPos, instance);

        // Tests
        Assertions.assertTrue(checkFragility(instance.getItems().get(itemNormal1.getId()),  tour, endPos, instance, true));
        Assertions.assertTrue(checkFragility(instance.getItems().get(itemNormal2.getId()),  tour, endPos, instance, true));
        Assertions.assertTrue(checkFragility(instance.getItems().get(itemFragile1.getId()), tour, endPos, instance, true));
        Assertions.assertTrue(checkFragility(instance.getItems().get(itemFragile2.getId()), tour, endPos, instance, true));
    }
}
