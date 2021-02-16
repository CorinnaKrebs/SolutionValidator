package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Getter;

@AllArgsConstructor
@Getter
public class Point {
    private int x;
    private int y;
    private int z;

    public boolean equals(Point other) {
        return (this.x == other.x) && (this.y == other.y) && (this.z == other.z);
    }
}
