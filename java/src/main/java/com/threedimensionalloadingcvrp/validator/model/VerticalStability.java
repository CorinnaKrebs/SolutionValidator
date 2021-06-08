package com.threedimensionalloadingcvrp.validator.model;

/**
 * The enum Vertical stability.
 */
public enum VerticalStability {
    /** No Consideration of Vertical Stability. */
    none,

    /** Consideration of Minimal supporting Area Constraint. */
    MinimalSupport,

    /** Consideration of Multiple overhanging Constraint. */
    MultipleOverhanging,

    /** Consideration of Top overhanging Constraint. */
    TopOverhanging,

	/** Consideration of Static Stability Constraint. */
	StaticsStability,

    /** Consideration of Static Stability Constraint proposed by Ramos (2015). */
    StaticStabilityRamos,

    /** Consideration of Static Stability Constraint proposed by Mack (2004). */
    StaticStabilityMack,
}
