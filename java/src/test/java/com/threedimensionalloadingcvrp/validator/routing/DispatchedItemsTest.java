package com.threedimensionalloadingcvrp.validator.routing;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;

import static com.threedimensionalloadingcvrp.validator.constraints.Routing.checkDispatchedItems;

public class DispatchedItemsTest {
    @Test
    public void Demand2NotFulfilled() {
        ItemType type1 = new ItemType(1, 0, 0, 0, 0, false, 0);
        ItemType type2 = new ItemType(2, 0, 0, 0, 0, false, 0);

        Item item1 = new Item(0, 1, false, type1);
        Item item2 = new Item(1, 1, false, type1);
        Item item3 = new Item(2, 1, false, type1);

        Item item4 = new Item(3, 1, false, type2);
        Item item5 = new Item(4, 1, false, type2);

        // Instance Creation
        Customer depot = new Customer(0, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer = new Customer(1, 0, 0, 5, 0, 0, 0, 0, 0);
        Instance instance = new Instance("", null, null, Arrays.asList(depot, customer), 0, false, Arrays.asList(item1, item2, item3, item4, item5));
        Demand demand1 = new Demand(1, 3);
        Demand demand2 = new Demand(2, 3);
        instance.getCustomers().get(1).getDemands().add(demand1);
        instance.getCustomers().get(1).getDemands().add(demand2);

        // Tour Creation
        Tour tour = new Tour(1, Collections.singletonList(1), Arrays.asList(0, 1, 2, 3, 4));

        // Solution Creation
        Solution solution = new Solution(Collections.singletonList(tour));

        // Test
        Assertions.assertFalse(checkDispatchedItems(solution, instance, false));
    }

    @Test
    public void DemandFulfilled() {
        ItemType type1 = new ItemType(1, 0, 0, 0, 0, false, 0);
        ItemType type2 = new ItemType(2, 0, 0, 0, 0, false, 0);

        Item item1 = new Item(0, 1, false, type1);
        Item item2 = new Item(1, 1, false, type1);
        Item item3 = new Item(2, 1, false, type1);

        Item item4 = new Item(3, 1, false, type2);
        Item item5 = new Item(4, 1, false, type2);

        // Instance Creation
        Customer depot = new Customer(0, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer = new Customer(1, 0, 0, 5, 0, 0, 0, 0, 0);
        Instance instance = new Instance("", null, null, Arrays.asList(depot, customer), 0, false, Arrays.asList(item1, item2, item3, item4, item5));
        Demand demand1 = new Demand(1, 3);
        Demand demand2 = new Demand(2, 2);
        instance.getCustomers().get(1).getDemands().add(demand1);
        instance.getCustomers().get(1).getDemands().add(demand2);

        // Tour Creation
        Tour tour = new Tour(1, Collections.singletonList(1), Arrays.asList(0, 1, 2, 3, 4));

        // Solution Creation
        Solution solution = new Solution(Collections.singletonList(tour));

        // Test
        Assertions.assertTrue(checkDispatchedItems(solution, instance, false));
    }
}
