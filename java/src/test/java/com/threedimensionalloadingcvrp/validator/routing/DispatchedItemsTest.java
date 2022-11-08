package com.threedimensionalloadingcvrp.validator.routing;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static com.threedimensionalloadingcvrp.validator.constraints.Routing.checkDispatchedItems;

public class DispatchedItemsTest {
    @Test
    public void Demand2NotFulfilled() {
        ItemType type1 = new ItemType(1, 0, 0, 0, 0, false, 0);
        ItemType type2 = new ItemType(2, 0, 0, 0, 0, false, 0);

        Item item1 = new Item(0, 1, Rotation.none, type1);
        Item item2 = new Item(1, 1, Rotation.none, type1);
        Item item3 = new Item(2, 1, Rotation.none, type1);

        Item item4 = new Item(3, 1, Rotation.none, type2);
        Item item5 = new Item(4, 1, Rotation.none, type2);

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
    public void Demand2Split() {
        ItemType type1 = new ItemType(1, 0, 0, 0, 0, false, 0);
        ItemType type2 = new ItemType(2, 0, 0, 0, 0, false, 0);

        Item item1 = new Item(0, 1, Rotation.none, type1);
        Item item2 = new Item(1, 1, Rotation.none, type1);
        Item item3 = new Item(2, 1, Rotation.none, type1);
        Item item4 = new Item(3, 1, Rotation.none, type2);

        Item item5 = new Item(4, 2, Rotation.none, type2);
        Item item6 = new Item(5, 2, Rotation.none, type2);

        // Instance Creation
        Customer depot = new Customer(0, 0, 0, 0, 0, 0, 0, 0, 0);
        Customer customer1 = new Customer(1, 0, 0, 4, 0, 0, 0, 0, 0);
        Customer customer2 = new Customer(2, 0, 0, 2, 0, 0, 0, 0, 0);
        Instance instance = new Instance("", null, null, Arrays.asList(depot, customer1, customer2), 0, false, Arrays.asList(item1, item2, item3, item4, item5, item6));
        Demand demand1 = new Demand(1, 3);
        Demand demand2 = new Demand(2, 1);
        Demand demand3 = new Demand(2, 2);
        instance.getCustomers().get(1).getDemands().add(demand1);
        instance.getCustomers().get(1).getDemands().add(demand2);
        instance.getCustomers().get(2).getDemands().add(demand3);

        // Tour Creation
        Tour tour1 = new Tour(1, List.of(1, 2), Arrays.asList(0, 1, 2, 4, 5));
        Tour tour2 = new Tour(2, List.of(1), Arrays.asList(3));

        // Solution Creation
        Solution solution = new Solution(List.of(tour1, tour2));

        // Test
        Assertions.assertTrue(checkDispatchedItems(solution, instance, false));
    }

    @Test
    public void DemandFulfilled() {
        ItemType type1 = new ItemType(1, 0, 0, 0, 0, false, 0);
        ItemType type2 = new ItemType(2, 0, 0, 0, 0, false, 0);

        Item item1 = new Item(0, 1, Rotation.none, type1);
        Item item2 = new Item(1, 1, Rotation.none, type1);
        Item item3 = new Item(2, 1, Rotation.none, type1);

        Item item4 = new Item(3, 1, Rotation.none, type2);
        Item item5 = new Item(4, 1, Rotation.none, type2);

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
