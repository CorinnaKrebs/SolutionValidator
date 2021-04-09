package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * The 3D Point.
 */
@AllArgsConstructor
@Getter
public class Point {
    /** x-Position */
    private final int x;

    /** y-Position */
    private final int y;

    /** z-Position */
    private final int z;

    /**
     * Equals Operator.
     *
     * @param other another point
     * @return true if two points are equal.
     */
    public boolean equals(Point other) {
        return (this.x == other.x) && (this.y == other.y) && (this.z == other.z);
    }
}
