#pragma once
namespace validator {

	enum class UnloadingSequence { none, LIFO, MLIFO };
	enum class VerticalStability { none, MinimalSupport, MultipleOverhanging, TopOverhanging, StaticStability, StaticStabilityRamos, StaticStabilityMack };
	enum class Stacking { none, Fragility, LBSSimple, LBSComplete };

	/**
	* The type Constraint set.
	*/
	class ConstraintSet {
	public:
		ConstraintSet(const bool rotation, const bool load_capacity, const UnloadingSequence& uSequence, const VerticalStability& vStability, const Stacking& stacking, const bool reachability, const bool axle_weights, const bool balanced_loading, 
			const float alpha, const unsigned int lambda, const float balanced_part, const bool time_windows, const bool split_delivery)
			: rotation(rotation), load_capacity(load_capacity), uSequence(uSequence), vStability(vStability), stacking(stacking), reachability(reachability), axle_weights(axle_weights), balanced_loading(balanced_loading), 
			alpha(alpha), lambda(lambda), balanced_part(balanced_part), time_windows(time_windows), split_delivery(split_delivery) {}


		// Loading Constraints
		/** Consideration of Rotation along length/width plane */
		bool rotation;

		/** Consideration of Item Masses and Vehicle Load Capacity */
		bool load_capacity;

		/** Type of Unloading Sequence Constraint */
		UnloadingSequence uSequence;

		/** Type of Vertical Stability Constraint */
		VerticalStability vStability;

		/** Type of Stacking Constraint */
		Stacking stacking;

		/** Consideration of Reachability */
		bool reachability;

		/** Consideration of Axle Weights */
		bool axle_weights;

		/** Consideration of Balanced Constraints */
		bool balanced_loading;


		// Parameters
		/** Vertical Support Parameter */
		const float		   alpha;

		/** Reachability Parameter */
		const unsigned int lambda;

		/** Balanced Loading Parameter */
		const float		   balanced_part;

		// Routing Constraints
		/** Consideration of AxTime Windows*/
		bool time_windows;

		/** Consideration of Split Deliveries */
		bool split_delivery;
	};
}