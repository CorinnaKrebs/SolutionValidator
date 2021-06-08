#pragma once

namespace validator {
	/**
	 * The type Vehicle.
	 */
	class Vehicle {

	public:
		// Constructor
		/**
		 * Instantiates a new Vehicle without Trailer.
		 *
		 * @param l             the length of the cargo space
		 * @param w             the width of the cargo space
		 * @param h             the height of the cargo space
		 * @param D             the Maximum Load Capacity (Mass)
		 * @param load_volume   the volume of the cargo space
		 * @param wheelbase     the wheelbase (Distance between Axles)
		 * @param lim_frontAxle the limit for front axle
		 * @param lim_rearAxle  the limit for rear axle
		 * @param l_f			the distance between front axle and cargo space
		 */
		Vehicle(const unsigned l, const unsigned w, const unsigned h, const double D, const unsigned int load_volume, const unsigned int wheelbase, const double lim_frontAxle, const double lim_rearAxle, const int l_f, const int l_tractor_RA, const double mass_tractor)
			: l(l), w(w), h(h), D(D), load_volume(load_volume), wheelbase(wheelbase), lim_frontAxle(lim_frontAxle), lim_rearAxle(lim_rearAxle), l_FA_cargo(l_f),
			axle_trailer(false), lim_trailerAxle(0), l_kingpin_RA(0), l_kingpin_TA(0), l_cargo_TA(0), l_tractor_RA(l_tractor_RA), l_trailer_TA(0), mass_tractor(mass_tractor), mass_trailer(0) {}

		/**
		 * Instantiates a new Vehicle without Trailer and with Vehicle Mass.
		 *
		 * @param l             the length of the cargo space
		 * @param w             the width of the cargo space
		 * @param h             the height of the cargo space
		 * @param D             the Maximum Load Capacity (Mass)
		 * @param load_volume   the volume of the cargo space
		 * @param wheelbase     the wheelbase (Distance between Axles)
		 * @param lim_frontAxle the limit for front axle
		 * @param lim_rearAxle  the limit for rear axle
		 * @param l_f		    the distance between front axle and cargo space
		 */
		Vehicle(const unsigned l, const unsigned w, const unsigned h, const double D, const unsigned int load_volume, const unsigned int wheelbase, const double lim_frontAxle, const double lim_rearAxle, const int l_f)
			: Vehicle(l, w, h, D, load_volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f, 0, 0) {}

		
		/**
		* Instantiates a new Semi-Truck Trailer.
		*
		* @param l               the length of the cargo space
		* @param w               the width of the cargo space
		* @param h               the height of the cargo space
		* @param D               the Maximum Load Capacity (Mass)
		* @param load_volume     the volume of the cargo space
		* @param wheelbase       the wheelbase (Distance between Axles)
		* @param lim_frontAxle   the limit for front axle
		* @param lim_rearAxle    the limit for rear axle
		* @param lim_trailerAxle the limit for trailer axle
		* @param l_kingpin_RA    the distance between kingpin and rear axle
		* @param l_kingpin_TA    the distance between kingpin and trailer axle
		* @param l_cargo_TA      the distance between cargo space and trailer axle
		* @param l_tractor_RA    the distance between center of mass of truck and trailer axle
		* @param l_trailer_TA    the distance between center of mass of trailer and trailer axle
		* @param mass_tractor    the mass of the truck
		* @param mass_trailer    the mass of the trailer
		*/
		Vehicle(const unsigned l, const unsigned w, const unsigned h, const double D, const unsigned int load_volume, const unsigned int wheelbase, const double lim_frontAxle, const double lim_rearAxle,
			const double lim_trailerAxle, const unsigned int l_kingpin_RA, const unsigned int l_kingpin_TA, const unsigned int l_cargo_TA, const unsigned int l_tractor_RA,
			const unsigned int l_trailer_TA, const double mass_tractor, const double mass_trailer)
			: l(l), w(w), h(h), D(D), load_volume(load_volume), wheelbase(wheelbase), lim_frontAxle(lim_frontAxle), lim_rearAxle(lim_rearAxle), l_FA_cargo(0),
			axle_trailer(true), lim_trailerAxle(lim_trailerAxle), l_kingpin_RA(l_kingpin_RA), l_kingpin_TA(l_kingpin_TA), l_cargo_TA(l_cargo_TA), l_tractor_RA(l_tractor_RA), l_trailer_TA(l_trailer_TA),
			mass_tractor(mass_tractor), mass_trailer(mass_trailer) {}

		/** Length of Cargo Space */
		const unsigned int	 l;

		/** Width of Cargo Space */
		const unsigned int	 w;

		/** Height of Cargo Space */
		const unsigned int	 h;

		/** The Maximum Load Capacity (Mass) */
		const double		 D;

		/** Volume of Cargo Space */
		const unsigned int	 load_volume;

		// Axle Weight Consideration
		/** Wheelbase (Distance between Axles) */
		const unsigned int	 wheelbase;

		/** Limit for Front Axle */
		const double		 lim_frontAxle;

		/** Limit for Rear Axle */
		const double		 lim_rearAxle;

		/** Distance between Front Axle and Cargo Space */
		const int			 l_FA_cargo;

		// Additional for Semi-Truck Trailer
		/** Consideration of Axle Weights for Trailer */
		const bool			 axle_trailer;

		/** Limit for Trailer Axle */
		const double		 lim_trailerAxle;

		/** Distance between Kingpin and Rear Axle */
		const unsigned int	 l_kingpin_RA;

		/** Distance between Kingpin and Trailer Axle */
		const unsigned int   l_kingpin_TA;

		/** Distance between Cargo Space and Trailer Axle */
		const unsigned int   l_cargo_TA;

		/** Distance between Center of Mass of Tractor and Rear Axle */
		const unsigned int	 l_tractor_RA;

		/** Distance between Center of Mass of Trailer and Trailer Axle */
		const unsigned int   l_trailer_TA;

		/** Mass Tractor */
		const double		 mass_tractor;

		/** Mass Trailer */
		const double		 mass_trailer;

	};

}