package com.threedimensionalloadingcvrp.validator.model;

import lombok.Getter;

@Getter
public class Vehicle {
    private int	l;				     // Length of Cargo Space
	private int	w;				     // Width  of Cargo Space
	private int	h;				     // Height of Cargo Space

	// Axle Weight Consideration
	private double D;				 // Load Capacity
	private int	   load_volume;	     // Volume of Cargo Space
	private int    wheelbase;		 // Wheelbase (Distance Axles)
	private double lim_frontAxle;	 // Limit for Front Axle
	private double lim_rearAxle;	 // Limit for Rear Axle
	private int	   l_FA_cargo;		 // Distance Front Axle - Cargo Space

	// Additional for Semi-Truck Trailer
	private boolean	axle_trailer;	 // Consideration of Axle Weights for Trailer
	private double	lim_trailerAxle; // Limit for Trailer Axle
	private int	 	l_kingpin_RA;    // Distance Kingpin - Rear Axle
	private int   	l_kingpin_TA;    // Distance Kinpin  - Trailer Axle
	private int   	l_cargo_TA;      // Distance Cargo Spcae - Trailer Axle
	private int 	l_tractor_RA;	 // Distance Center of Mass of Tractor - Rear Axle
	private int   	l_trailer_TA;    // Distance Center of Mass of Trailer - Trailer Axle
	private double  mass_tractor;    // Mass Tractor
	private double	mass_trailer;    // Mass Trailer

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

	public Vehicle(final int l, final int w, final int h, final double d, final int load_volume, final int wheelbase, final double lim_frontAxle, final double lim_rearAxle, final int l_FA_cargo, final int l_tractor_RA, final double mass_tractor) {
		this(l, w, h, d, load_volume, wheelbase, lim_frontAxle, lim_rearAxle, l_FA_cargo);
		this.l_tractor_RA = l_tractor_RA;
		this.mass_tractor = mass_tractor;
	}


	public Vehicle(final int l, final int w, final int h, final double d, final int load_volume, final int wheelbase, final double lim_frontAxle, final double lim_rearAxle, final double lim_trailerAxle, final int l_kingpin_RA, final int l_kingpin_TA, final int l_cargo_TA, final int l_truck_RA, final int l_trailer_TA, final double mass_truck, final double mass_trailer) {
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
		this.mass_tractor = mass_truck;
		this.mass_trailer = mass_trailer;
		this.axle_trailer = true;
		this.l_FA_cargo = 0;
	}

}
