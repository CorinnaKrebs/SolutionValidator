package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.*;
import static com.threedimensionalloadingcvrp.validator.model.UnloadingSequence.LIFO;

public class ReachabilityTest {
        private final int dim = 5;
        private final int lambda = dim;

        @Test
        public void InfeasiblePosition() {
            // Bottom Item is too long to reach Top Item
            // LIFO is obeyed

            ItemType typeBottom = new ItemType(1,    3 * dim, dim, dim, 0, false, 0);
            ItemType typeTop    = new ItemType(2, dim,      dim, dim, 0, false, 0);

            Item itemTop    = new Item(0, 1, false, typeBottom);
            Item itemBottom = new Item(1, 2, false, typeTop);


            Customer customerBottom = new Customer(1, 0, 0, 0, 0, 0, 0, 0, 0);
            Customer customerTop    = new Customer(2, 0, 0, 0, 0, 0, 0, 0, 0);

            Instance instance = new Instance("", null, null, Arrays.asList(customerBottom, customerTop), 0, false, Arrays.asList(itemBottom, itemTop));

            // Tour Creation
            Tour tour = new Tour(1, Arrays.asList(2, 1), Arrays.asList(0, 1));

            instance.getItems().get(itemBottom.getId()).setPosition(new Point(0, 0, 0));
            instance.getItems().get(itemTop.getId())   .setPosition(new Point(0, 0, dim));

			int endPos = tour.getItem_ids().size();
            Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemTop.getId()), tour, endPos, LIFO, instance, true));
            Assertions.assertFalse(checkReachability(instance.getItems().get(itemTop.getId()), tour, lambda,  true, instance, true));
        }

        @Test
        public void FeasiblePosition() {
            // length of ItemBottom = 2 * dim

            ItemType typeBottom = new ItemType(1, 2 * dim, dim, dim, 0, false, 0);
            ItemType typeTop    = new ItemType(2, dim, dim, dim, 0, false, 0);

            Item itemTop    = new Item(0, 1, false, typeBottom);
            Item itemBottom = new Item(1, 2, false, typeTop);


            Customer customerBottom = new Customer(1, 0, 0, 0, 0, 0, 0, 0, 0);
            Customer customerTop    = new Customer(2, 0, 0, 0, 0, 0, 0, 0, 0);

            Instance instance = new Instance("", null, null, Arrays.asList(customerBottom, customerTop), 0, false, Arrays.asList(itemBottom, itemTop));

            // Tour Creation
            Tour tour = new Tour(1, Arrays.asList(2, 1), Arrays.asList(0, 1));

            instance.getItems().get(itemBottom.getId()).setPosition(new Point(0, 0, 0));
            instance.getItems().get(itemTop.getId())   .setPosition(new Point(0, 0, dim));

            int endPos = tour.getItem_ids().size();
            Assertions.assertTrue(checkUnloadingSequence(instance.getItems().get(itemTop.getId()), tour, endPos, LIFO, instance, true));
            Assertions.assertTrue(checkReachability(instance.getItems().get(itemTop.getId()), tour, lambda,  true, instance, true));
        }

        @Test
        public void ScaleFactorTest() {
            Assertions.assertEquals(1,  getScaleFactor(60));
            Assertions.assertEquals(10, getScaleFactor(600));
        }
}
