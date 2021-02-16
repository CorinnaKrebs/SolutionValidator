package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@AllArgsConstructor
@Data
public class Demand {
    int itemTypeId;     // ItemType Id
    int quantity;       // Demanded Quantity of ItemType
}
