package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkVehicleWalls;

public class VehicleOverlappingTest {
    private final int dim = 5;
    @Test
    public void ItemOverlappingVehicle() {
        Item item = new Item(1, 1, Rotation.none, new ItemType(1, dim + 1, dim, dim, 0, false, 0));
        item.setPosition(new Point(0, 0, 0));

        Vehicle vehicle = new Vehicle(dim, dim, dim, 0, 0, 0, 0, 0, 0);
        Assertions.assertFalse(checkVehicleWalls(item, vehicle, true));
    }

    @Test
    public void ItemNotOverlapping() {
        Item item = new Item(1, 1, Rotation.none, new ItemType(1, dim, dim, dim, 0, false, 0));
        item.setPosition(new Point(0, 0, 0));

        Vehicle vehicle = new Vehicle(dim, dim, dim, 0, 0, 0, 0, 0, 0);
        Assertions.assertTrue(checkVehicleWalls(item, vehicle, true));
    }
}
