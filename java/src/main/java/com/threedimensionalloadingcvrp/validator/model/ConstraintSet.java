package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

/**
 * The type Constraint set.
 */
@Data
@AllArgsConstructor
public class ConstraintSet {

    // Loading Constraints
	/** Consideration of Rotation along length/width plane */
	private boolean rotation;

	/** Consideration of Item Masses and Vehicle Load Capacity */
	private boolean load_capacity;

	/** Type of Unloading Sequence Constraint */
	private UnloadingSequence uSequence;

	/** Type of Vertical Stability Constraint */
	private VerticalStability vStability;

	/** Type of Stacking Constraint */
	private Stacking stacking;

	/** Consideration of Reachability */
	private boolean reachability;

	/** Consideration of Axle Weights */
	private boolean axle_weights;

	/** Consideration of Balanced Constraints */
	private boolean balanced_loading;

	// Loading Parameters
	/** Vertical Support Parameter */
	private float 	alpha;

	/** Reachability Parameter */
	private int 	lambda;

	/** Balanced Loading Parameter */
	private float	balanced_part;

	// Routing Constraints
	/** Consideration of Split Deliveries */
	private boolean split;

	/** Consideration of Time Windows */
	private boolean timeWindows;
}
