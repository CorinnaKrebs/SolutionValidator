package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

/**
 * The Demand of one ItemType.
 */
@AllArgsConstructor
@Data
public class Demand {
    /** The ItemType id. */
    int itemTypeId;

    /** The Demanded Quantity of ItemType. */
    int quantity;
}
