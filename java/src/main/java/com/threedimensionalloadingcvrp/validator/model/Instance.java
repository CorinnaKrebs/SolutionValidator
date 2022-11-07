package com.threedimensionalloadingcvrp.validator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

/**
 * The type Instance.
 */
@Data
@AllArgsConstructor
public class Instance {
	/** the instance name */
	private String			name;

	/** the vehicle data, homogeneous fleet */
	private Vehicle		    vehicle;

	/** the list with all item types */
	private List<ItemType>  itemTypes;

	/** the list with all customers */
	private List<Customer>	customers;

	/** the maximal number of available vehicles */
	private int			    v_max;

	/** separation between 3L-CVRP (false) and 3L-VRPTW (true) */
	private boolean 		tw;

	/** Contains all items and their position */
	private List<Item>		items;

	/**
	 * Instantiates a new Instance.
	 *
	 * @param name      the instance name
	 * @param vehicle   the vehicle data, homogeneous fleet
	 * @param itemTypes the list with all item types
	 * @param customers the list with all customers
	 * @param v_max     the maximal number of available vehicles
	 * @param tw        the consideration of time windows
	 */
	public Instance(final String name, final Vehicle vehicle, final List<ItemType> itemTypes, final List<Customer> customers, final int v_max, final boolean tw) {
		this.name = name;
		this.vehicle = vehicle;
		this.itemTypes = itemTypes;
		this.customers = customers;
		this.v_max = v_max;
		this.tw = tw;
		this.items = new ArrayList<>();
	}
}
