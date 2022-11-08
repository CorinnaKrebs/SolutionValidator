package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkUnloadingSequence;
import static com.threedimensionalloadingcvrp.validator.model.UnloadingSequence.LIFO;
import static com.threedimensionalloadingcvrp.validator.model.UnloadingSequence.MLIFO;

public class USequenceTest {
    private final int dim = 5;

    @Test
    public void InfeasiblePositions() {
        ItemType type = new ItemType(1, dim, dim, dim, 0, false, 0);

        Item item1 = new Item(0, 1, Rotation.none, type);
        Item item2 = new Item(1, 2, Rotation.none, type);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 5, false, Arrays.asList(item1, item2));

        // Tour Creation
        Tour tour = new Tour(1, Arrays.asList(1, 2), Arrays.asList(0, 1));

        // First Served Item Behind Second Served Item
        instance.getItems().get(item1.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(item2.getId()).setPosition(new Point(dim, 0, 0));

        // Tests
		final int endPos = tour.getItem_ids().size();
        Assertions.assertFalse(checkUnloadingSequence(instance.getItems().get(item1.getId()), tour, endPos, LIFO, instance, true));
        Assertions.assertTrue (checkUnloadingSequence(instance.getItems().get(item2.getId()), tour, endPos, LIFO, instance, true));
        Assertions.assertFalse(checkUnloadingSequence(instance.getItems().get(item1.getId()), tour, endPos, MLIFO, instance, true));
        Assertions.assertTrue (checkUnloadingSequence(instance.getItems().get(item2.getId()), tour, endPos, MLIFO, instance, true));

        // First Served Item Under Second Served Item
        instance.getItems().get(item1.getId()).setPosition(new Point(0, 0, 0));
        instance.getItems().get(item2.getId()).setPosition(new Point(0, 0, dim));

        // Tests
        Assertions.assertFalse(checkUnloadingSequence(instance.getItems().get(item1.getId()), tour, endPos, LIFO, instance, true));
        Assertions.assertTrue (checkUnloadingSequence(instance.getItems().get(item2.getId()), tour, endPos, LIFO, instance, true));
        Assertions.assertFalse(checkUnloadingSequence(instance.getItems().get(item1.getId()), tour, endPos, MLIFO, instance, true));
        Assertions.assertTrue (checkUnloadingSequence(instance.getItems().get(item2.getId()), tour, endPos, MLIFO, instance, true));
    }

    @Test
    public void Differences() {
        ItemType typeNormal = new ItemType(1,     dim,   dim,     dim,   0, false, 0);
        ItemType typeHigh   = new ItemType(2,     dim,   dim, 2 * dim,   0, false, 0);
        ItemType typeLong   = new ItemType(3, 2 * dim,   dim,     dim,   0, false, 0);

        Item itemC1 = new Item(0, 1, Rotation.none, typeNormal);
        Item itemC2High = new Item(1, 2, Rotation.none, typeHigh);
        Item itemC2Long = new Item(2, 2, Rotation.none, typeLong);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemC1, itemC2High, itemC2Long));

        // Tour Creation
        Tour tour = new Tour(1, Arrays.asList(1, 2), Arrays.asList(0, 1, 2));

        // Customer2 : Item High in Origin, Long on Top of High -> Overhanging
        // Customer1 : Item in Front of High
        instance.getItems().get(itemC2High.getId()).setPosition(new Point(0,   0, 0));
        instance.getItems().get(itemC2Long.getId()).setPosition(new Point(0,   0, 2 * dim));
        instance.getItems().get(itemC1.getId())    .setPosition(new Point(dim, 0, 0));

        // According MLIFO: everything is fine
        final int endPos = tour.getItem_ids().size();
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC2High.getId()), tour, endPos, MLIFO, instance, true));
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC2Long.getId()), tour, endPos, MLIFO, instance, true));
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC1.getId()),     tour, endPos, MLIFO, instance, true));

        // According LIFO: Item of Customer 1 has no feasible position
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC2High.getId()), tour, endPos, LIFO, instance, true));
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC2Long.getId()), tour, endPos, LIFO, instance, true));
        Assertions.assertFalse(checkUnloadingSequence(instance.getItems().get(itemC1.getId()),    tour, endPos, LIFO, instance, true));
    }

    @Test
    public void FeasiblePosition() {
        ItemType type = new ItemType(1, dim, dim, dim, 0, false, 0);

        Item itemC1 = new Item(0, 1, Rotation.none, type);
        Item itemC2 = new Item(1, 2, Rotation.none, type);

        // Instance Creation
        Instance instance = new Instance("", null, null, null, 0, false, Arrays.asList(itemC1, itemC2));

        // Tour Creation
        Tour tour = new Tour(1, Arrays.asList(1, 2), Arrays.asList(1, 0));

        // Customer2 : Item in Origin
        // Customer1 : Item in Front
        instance.getItems().get(itemC2.getId()).setPosition(new Point(0,   0, 0));
        instance.getItems().get(itemC1.getId()).setPosition(new Point(dim, 0, 0));

        // Tests
		final int endPos = tour.getItem_ids().size();
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC1.getId()), tour, endPos, MLIFO, instance, true));
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC2.getId()), tour, endPos, MLIFO, instance, true));
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC1.getId()), tour, endPos, LIFO, instance, true));
        Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemC2.getId()), tour, endPos, LIFO, instance, true));
    }
}
