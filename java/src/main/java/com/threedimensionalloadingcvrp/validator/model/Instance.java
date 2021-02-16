package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
@AllArgsConstructor
public class Instance {
	public Instance(final String name, final Vehicle vehicle, final List<ItemType> itemTypes, final List<Customer> customers, final int v_max, final boolean tw) {
		this.name = name;
		this.vehicle = vehicle;
		this.itemTypes = itemTypes;
		this.customers = customers;
		this.v_max = v_max;
		this.tw = tw;
		this.items = new ArrayList<>();
	}

	private String			name;		// Instance Name
	private Vehicle		    vehicle;	// Description of Vehicle Type, Homogeneous Fleet
	private List<ItemType>  itemTypes;	// Contains all ItemTypes
    private List<Customer>	customers;	// Contains all customers
	private int			    v_max;		// Maximal Number of available Vehicles
	private boolean		    tw;			// Consideration of Time Windows
    private List<Item>		items;		// Contains all items and their position

}
