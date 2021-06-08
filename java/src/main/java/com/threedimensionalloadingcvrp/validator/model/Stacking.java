package com.threedimensionalloadingcvrp.validator.model;

/**
 * The enum Stacking.
 */
public enum Stacking {
    /** No Consideration of Stacking Constraints. */
    none,

    /** Consideration of the Fragility Flag. */
    Fragility,

    /** Consideration of the Load Bearing Strength as described in Bischoff et al. (2006). */
    LBSSimple,

    /** Consideration of the Load Bearing Strength according to Science of Statics. */
    LBSComplete
}
