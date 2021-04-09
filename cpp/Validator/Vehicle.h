#pragma once

namespace validator {

	class Vehicle {

	public:
		// Constructor
		Vehicle(const unsigned l, const unsigned w, const unsigned h, const double D, const unsigned int load_volume, const unsigned int wheelbase, const double lim_frontAxle, const double lim_rearAxle, const int l_f, const int l_tractor_RA, const double mass_tractor)
			: l(l), w(w), h(h), D(D), load_volume(load_volume), wheelbase(wheelbase), lim_frontAxle(lim_frontAxle), lim_rearAxle(lim_rearAxle), l_FA_cargo(l_f),
			axle_trailer(false), lim_trailerAxle(0), l_kingpin_RA(0), l_kingpin_TA(0), l_cargo_TA(0), l_tractor_RA(l_tractor_RA), l_trailer_TA(0), mass_tractor(mass_tractor), mass_trailer(0) {}

		Vehicle(const unsigned l, const unsigned w, const unsigned h, const double D, const unsigned int load_volume, const unsigned int wheelbase, const double lim_frontAxle, const double lim_rearAxle, const int l_f)
			: Vehicle(l, w, h, D, load_volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f, 0, 0) {}

		Vehicle(const unsigned l, const unsigned w, const unsigned h, const double D, const unsigned int load_volume, const unsigned int wheelbase, const double lim_frontAxle, const double lim_rearAxle,
			const double lim_trailerAxle, const unsigned int l_kingpin_RA, const unsigned int l_kingpin_TA, const unsigned int l_cargo_TA, const unsigned int l_tractor_RA,
			const unsigned int l_trailer_TA, const double mass_tractor, const double mass_trailer)
			: l(l), w(w), h(h), D(D), load_volume(load_volume), wheelbase(wheelbase), lim_frontAxle(lim_frontAxle), lim_rearAxle(lim_rearAxle), l_FA_cargo(0),
			axle_trailer(true), lim_trailerAxle(lim_trailerAxle), l_kingpin_RA(l_kingpin_RA), l_kingpin_TA(l_kingpin_TA), l_cargo_TA(l_cargo_TA), l_tractor_RA(l_tractor_RA), l_trailer_TA(l_trailer_TA),
			mass_tractor(mass_tractor), mass_trailer(mass_trailer) {}

		const unsigned int	 l;				// Length of Cargo Space
		const unsigned int	 w;				// Width  of Cargo Space
		const unsigned int	 h;				// Height of Cargo Space

		const double		 D;				// Load Capacity
		const unsigned int	 load_volume;	// Volume of Cargo Space

		// Axle Weight Consideration
		const unsigned int	 wheelbase;		// Wheelbase (Distance Axles)
		const double		 lim_frontAxle;	// Limit for Front Axle
		const double		 lim_rearAxle;	// Limit for Rear Axle
		const int			 l_FA_cargo;	// Distance Front Axle - Cargo Space

		// Additional for Semi-Truck Trailer
		const bool			 axle_trailer;	// Consideration of Axle Weights for Trailer
		const double		 lim_trailerAxle;
		const unsigned int	 l_kingpin_RA;  // Distance Kingpin - Rear Axle
		const unsigned int   l_kingpin_TA;  // Distance Kinpin  - Trailer Axle
		const unsigned int   l_cargo_TA;    // Distance Cargo Spcae - Trailer Axle
		const unsigned int	 l_tractor_RA;	// Distance Center of Mass of Tractor - Rear Axle
		const unsigned int   l_trailer_TA;  // Distance Center of Mass of Trailer - Trailer Axle
		const double		 mass_tractor;  // Mass Trailer
		const double		 mass_trailer;  // Mass Trailer

	};

}