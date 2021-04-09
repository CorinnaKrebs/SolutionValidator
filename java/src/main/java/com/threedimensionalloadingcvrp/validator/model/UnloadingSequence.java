package com.threedimensionalloadingcvrp.validator.model;

/**
 * The enum Unloading sequence.
 */
public enum UnloadingSequence {
    /** No Consideration of the Unloading Sequence. */
    none,

    /** Consideration of LIFO (Last-In-First-Out). */
    LIFO,

    /** Consideration of MLIFO (Manual Last-In-First-Out). */
    MLIFO
}
