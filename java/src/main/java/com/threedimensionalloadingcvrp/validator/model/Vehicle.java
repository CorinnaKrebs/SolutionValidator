package com.threedimensionalloadingcvrp.validator.model;

import lombok.Getter;

/**
 * The type Vehicle.
 */
@Getter
public class Vehicle {
	/** Length of Cargo Space */
    private int	l;

	/** Width of Cargo Space */
	private int	w;

	/** Height of Cargo Space */
	private int	h;

	// Axle Weight Consideration
	/** The Maximum Load Capacity (Mass) */
	private double D;

	/** Volume of Cargo Space */
	private int	   load_volume;

	/** Wheelbase (Distance between Axles) */
	private int    wheelbase;

	/** Limit for Front Axle */
	private double lim_frontAxle;

	/** Limit for Rear Axle */
	private double lim_rearAxle;

	/** Distance between Front Axle and Cargo Space */
	private int	   l_FA_cargo;

	// Additional for Semi-Truck Trailer
	/** Consideration of Axle Weights for Trailer */
	private boolean	axle_trailer;

	/** Limit for Trailer Axle */
	private double	lim_trailerAxle;

	/** Distance between Kingpin and Rear Axle */
	private int	 	l_kingpin_RA;

	/** Distance between Kingpin and Trailer Axle */
	private int   	l_kingpin_TA;

	/** Distance between Cargo Space and Trailer Axle */
	private int   	l_cargo_TA;

	/** Distance between Center of Mass of Tractor and Rear Axle */
	private int 	l_tractor_RA;

	/** Distance between Center of Mass of Trailer and Trailer Axle */
	private int   	l_trailer_TA;

	/** Mass Tractor */
	private double  mass_tractor;

	/** Mass Trailer */
	private double	mass_trailer;

	/**
	 * Instantiates a new Vehicle without Trailer.
	 *
	 * @param l             the length of the cargo space
	 * @param w             the width of the cargo space
	 * @param h             the height of the cargo space
	 * @param d             the Maximum Load Capacity (Mass)
	 * @param load_volume   the volume of the cargo space
	 * @param wheelbase     the wheelbase (Distance between Axles)
	 * @param lim_frontAxle the limit for front axle
	 * @param lim_rearAxle  the limit for rear axle
	 * @param l_FA_cargo    the distance between front axle and cargo space
	 */
	public Vehicle(final int l, final int w, final int h, final double d, final int load_volume, final int wheelbase, final double lim_frontAxle, final double lim_rearAxle, final int l_FA_cargo) {
		this.l = l;
		this.w = w;
		this.h = h;
		this.D = d;
		this.load_volume = load_volume;
		this.wheelbase = wheelbase;
		this.lim_frontAxle = lim_frontAxle;
		this.lim_rearAxle = lim_rearAxle;
		this.l_FA_cargo = l_FA_cargo;
		this.axle_trailer = false;
		this.lim_trailerAxle = 0;
		this.l_kingpin_RA = 0;
		this.l_kingpin_TA = 0;
		this.l_kingpin_TA = 0;
		this.l_cargo_TA = 0;
		this.l_tractor_RA = 0;
		this.l_trailer_TA = 0;
		this.mass_tractor = 0;
		this.mass_trailer = 0;
	}

	/**
	 * Instantiates a new Vehicle without Trailer and with Vehicle Mass.
	 *
	 * @param l             the length of the cargo space
	 * @param w             the width of the cargo space
	 * @param h             the height of the cargo space
	 * @param d             the Maximum Load Capacity (Mass)
	 * @param load_volume   the volume of the cargo space
	 * @param wheelbase     the wheelbase (Distance between Axles)
	 * @param lim_frontAxle the limit for front axle
	 * @param lim_rearAxle  the limit for rear axle
	 * @param l_FA_cargo    the distance between front axle and cargo space
	 * @param l_tractor_RA  the distance between center of mass of tractor and rear axle.
	 * @param mass_tractor  the mass of the tractor
	 */
	public Vehicle(final int l, final int w, final int h, final double d, final int load_volume, final int wheelbase, final double lim_frontAxle, final double lim_rearAxle, final int l_FA_cargo, final int l_tractor_RA, final double mass_tractor) {
		this(l, w, h, d, load_volume, wheelbase, lim_frontAxle, lim_rearAxle, l_FA_cargo);
		this.l_tractor_RA = l_tractor_RA;
		this.mass_tractor = mass_tractor;
	}


	/**
	 * Instantiates a new Semi-Truck Trailer.
	 *
	 * @param l               the length of the cargo space
	 * @param w               the width of the cargo space
	 * @param h               the height of the cargo space
	 * @param d               the Maximum Load Capacity (Mass)
	 * @param load_volume     the volume of the cargo space
	 * @param wheelbase       the wheelbase (Distance between Axles)
	 * @param lim_frontAxle   the limit for front axle
	 * @param lim_rearAxle    the limit for rear axle
	 * @param lim_trailerAxle the limit for trailer axle
	 * @param l_kingpin_RA    the distance between kingpin and rear axle
	 * @param l_kingpin_TA    the distance between kingpin and trailer axle
	 * @param l_cargo_TA      the distance between cargo space and trailer axle
	 * @param l_truck_RA      the distance between center of mass of truck and trailer axle
	 * @param l_trailer_TA    the distance between center of mass of trailer and trailer axle
	 * @param mass_tractor    the mass of the truck
	 * @param mass_trailer    the mass of the trailer
	 */
	public Vehicle(final int l, final int w, final int h, final double d, final int load_volume, final int wheelbase, final double lim_frontAxle, final double lim_rearAxle, final double lim_trailerAxle, final int l_kingpin_RA, final int l_kingpin_TA, final int l_cargo_TA, final int l_truck_RA, final int l_trailer_TA, final double mass_tractor, final double mass_trailer) {
		this.l = l;
		this.w = w;
		this.h = h;
		this.D = d;
		this.load_volume = load_volume;
		this.wheelbase = wheelbase;
		this.lim_frontAxle = lim_frontAxle;
		this.lim_rearAxle = lim_rearAxle;
		this.lim_trailerAxle = lim_trailerAxle;
		this.l_kingpin_RA = l_kingpin_RA;
		this.l_kingpin_TA = l_kingpin_TA;
		this.l_cargo_TA = l_cargo_TA;
		this.l_tractor_RA = l_truck_RA;
		this.l_trailer_TA = l_trailer_TA;
		this.mass_tractor = mass_tractor;
		this.mass_trailer = mass_trailer;
		this.axle_trailer = true;
		this.l_FA_cargo = 0;
	}

}
