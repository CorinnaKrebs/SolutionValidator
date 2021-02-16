#pragma once

enum class UnloadingSequence { none, LIFO, MLIFO };
enum class VerticalStability { none, MinimalSupport, MultipleOverhanging, TopOverhanging};
enum class Stacking { none, Fragility, LBSSimple, LBSComplete };

class ConstraintSet {
public:
	ConstraintSet(const bool rotation, const bool load_capacity, const UnloadingSequence& uSequence, const VerticalStability& vStability, const Stacking& stacking, const bool reachability, const bool axle_weights, const bool balanced_loading, const float alpha, const unsigned int lambda, const float balanced_part) 
		: rotation(rotation), load_capacity(load_capacity), uSequence(uSequence), vStability(vStability), stacking(stacking), reachability(reachability), axle_weights(axle_weights), balanced_loading(balanced_loading), alpha(alpha), lambda(lambda), balanced_part(balanced_part) {}

	// Constraints
	const bool rotation;				// Consideration of Rotation along length/width plane
	const bool load_capacity;			// Consideration of Item Masses and Vehicle Load Capacity
	const UnloadingSequence uSequence;	// Type of Unloading Sequence Constraint
	const VerticalStability vStability; // Type of Vertical Stability Constraint
	const Stacking stacking;			// Type of Stacking Constraint
	const bool reachability;			// Consideration of Reachability
	const bool axle_weights;			// Consideration of Axle Weights
	const bool balanced_loading;		// Consideration of Balanced Loading


	// Parameters
	const float		   alpha;			// Vertical Support Parameter
	const unsigned int lambda;			// Reachability Parameter
	const float		   balanced_part;	// Balanced Loading Parameter
};