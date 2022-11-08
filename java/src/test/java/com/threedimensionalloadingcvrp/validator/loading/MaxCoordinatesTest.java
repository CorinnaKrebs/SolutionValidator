package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkMaxCoordinates;

public class MaxCoordinatesTest {
    private Item item;

    @BeforeEach
    public void initialization() {
        int dim = 5;
        item = new Item(1, 1, Rotation.none, new ItemType(1, dim, dim, dim, 0, false, 0));
    }

    @Test
    public void FalseMax() {
        item.setPosition(new Point(0, 0, 0));
        item.setMax(new Point(0, 0, 0));
        Assertions.assertFalse(checkMaxCoordinates(item, true));
    }

    @Test
    public void CorrectCoordinates() {
        item.setPosition(new Point(0, 0, 0));
        Assertions.assertTrue(checkMaxCoordinates(item, true));
    }
}
