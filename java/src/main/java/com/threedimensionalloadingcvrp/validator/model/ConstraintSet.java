package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class ConstraintSet {

    // Constraints
	private boolean rotation;				// Consideration of Rotation along length/width plane
	private boolean load_capacity;			// Consideration of Item Masses and Vehicle Load Capacity
	private UnloadingSequence uSequence;	// Type of Unloading Sequence Constraint
	private VerticalStability vStability; 	// Type of Vertical Stability Constraint
	private Stacking stacking;				// Type of Stacking Constraint
	private boolean reachability;			// Consideration of Reachability
	private boolean axle_weights;			// Consideration of Axle Weights
	private boolean balanced_loading;		// Consideration of Balanced Constraints.Loading

	// Parameters
	private float 	alpha;					// Vertical Support Parameter
	private int 	lambda;					// Reachability Parameter
	private float	balanced_part;			// Balanced Constraints.Loading Parameter
}
