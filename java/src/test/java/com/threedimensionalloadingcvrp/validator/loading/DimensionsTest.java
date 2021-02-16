package com.threedimensionalloadingcvrp.validator.loading;

import com.threedimensionalloadingcvrp.validator.model.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkDimensions;

public class DimensionsTest {
    private Item item;

    @BeforeEach
    public void initialize() {
        final int dim = 5;
        item = new Item(1, 1, false, new ItemType(1, dim, dim, dim, 0, false, 0));
    }

    @Test
    public void WrongDimensions() {
        item.setPosition(new Point(0, 0, 0));
        item.setMax(new Point(1, 1, 1));
        Assertions.assertFalse(checkDimensions(item, true));
    }

    @Test
    public void CorrectDimensions() {
        item.setPosition(new Point(0, 0, 0));
        Assertions.assertTrue(checkDimensions(item, true));
    }
}
