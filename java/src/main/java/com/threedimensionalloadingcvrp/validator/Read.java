package com.threedimensionalloadingcvrp.validator;

import com.threedimensionalloadingcvrp.validator.model.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.*;

import static java.lang.System.exit;

/**
 * The type Read.
 */
public class Read {
    /**
     * Read the instance file.
     * Get Customers, Items, ItemTypes and Vehicles.
     *
     * @param instancePath the path to the instance file
     * @return the instance
     */
    public static Instance readInstanceFile(final String instancePath) {
		Scanner scanner = null;
        try {
            scanner = new Scanner(new File(instancePath));
            scanner.useLocale(Locale.US);
            String name = "";
            int no_customers = 0, no_items = 0, no_types = 0, v_max = 0, intTW = 0;
            boolean tw = false;
            List<Customer>  customers = new ArrayList<>();
            List<ItemType>  itemtypes = new ArrayList<>();

            String var = scanner.next();
            if (var.equals("Name"))                 name = scanner.next();

            var = scanner.next();
            if (var.equals("Number_of_Customers"))  no_customers = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Number_of_Items"))      no_items = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Number_of_ItemTypes"))	no_types = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Number_of_Vehicles"))	v_max = scanner.nextInt();

            var = scanner.next();
            if (var.equals("TimeWindows"))	        intTW = scanner.nextInt();
            tw = intTW != 0;

            scanner.nextLine();
            scanner.nextLine();
            var = scanner.next();

            if (!var.equals("VEHICLE")) {
                throw new IOException("Wrong Line. Should be Header VEHICLE, current: " + var);
            }
            Vehicle vehicle = readVehicle(scanner);

            var = scanner.next();

            if (!var.equals("CUSTOMERS")) {
                throw new IOException("Wrong Line. Should be Header CUSTOMERS, current: " + var);
            }
            scanner.nextLine();
            scanner.nextLine();

            // Read Customers
            for (int i = 0; i <= no_customers; ++i) {
                Customer customer = readCustomer(scanner);
                customers.add(customer);
            }

            scanner.nextLine();
            scanner.nextLine();
            var = scanner.next();

            if (!var.equals("ITEMS")) {
                throw new IOException("Wrong Line. Should be Header ITEMS, current: " + var);
            }

            // Read ItemTypes
            scanner.nextLine();
            scanner.nextLine();
            itemtypes.add(new ItemType(0, 0, 0, 0, 0, false, 0));
            for (int i = 0; i < no_types; ++i) {
                ItemType type = readItemType(scanner);
                itemtypes.add(type);
            }

            scanner.nextLine();
            scanner.nextLine();
            var = scanner.next();
            if (!var.equals("DEMANDS")) {
                throw new IOException("Wrong Line. Should be Header DEMANDS, current: " + var);
            }

            // Read Demands
            scanner.nextLine();
            scanner.nextLine();
            for (int i = 1; i <= no_customers; ++i) {
                Customer customer = customers.get(i);
		        final int totalDemand = customers.get(i).getDemand();
                int id = scanner.nextInt();
                if (customer.getId() != id) {
					throw new IOException("Wrong Line. Expected: " + customer.getId() + ", read: " + id);
                }
                for (int j = 0; j < totalDemand;) {
                    Demand demand = readDemand(scanner);
                    customer.getDemands().add(demand);
                    j += demand.getQuantity();
                }
            }
            Instance instance = new Instance(name, vehicle, itemtypes, customers, v_max, tw);
            List<Item> items = new ArrayList<>(Collections.nCopies(no_items + 1, new Item(0, 0, false, new ItemType(0, 0, 0, 0, 0, false, 0))));
            instance.setItems(items);
            return instance;


        } catch (IOException ioe) {
            ioe.printStackTrace();
            return null;
        }
    }

    /**
     * Read the Vehicle.
     * Get Vehicle Data (e.g. Capacity, Length, Width, Height, Axle Weight Parameters)
     *
     * @param scanner to read the file
     * @return the Vehicle
     */
    private static Vehicle readVehicle(Scanner scanner) {
        int l = 0, w = 0, h = 0, wheelbase = 0, l_f = 0, l_tractor_RA = 0;
        double D = 0, lim_frontAxle = 0, lim_rearAxle = 0;
        double mass_tractor = 0;

        scanner.nextLine();

        String var = scanner.next();
        if (var.equals("Mass_Capacity"))			D = scanner.nextInt();

        var = scanner.next();
        if (var.equals("CargoSpace_Length"))		l = scanner.nextInt();

        var = scanner.next();
        if (var.equals("CargoSpace_Width"))		    w = scanner.nextInt();

        var = scanner.next();
        if (var.equals("CargoSpace_Height"))		h = scanner.nextInt();

        var = scanner.next();
        if (var.equals("Wheelbase"))				wheelbase = scanner.nextInt();

        var = scanner.next();
        if (var.equals("Max_Mass_FrontAxle"))	    lim_frontAxle = scanner.nextInt();

        var = scanner.next();
        if (var.equals("Max_Mass_RearAxle"))		lim_rearAxle = scanner.nextInt();

        var = scanner.next();

        // Vehicle without Trailer
        if (var.equals("Distance_FrontAxle_CargoSpace")) {
            l_f = scanner.nextInt();
            final int volume = l * w * h;

            var = scanner.nextLine();
            if (var.isEmpty())	                                return new Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f);

            if (var.equals("Distance_Mass_Truck_RearAxle"))		l_tractor_RA = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Mass_Truck"))						mass_tractor = scanner.nextDouble();

            return new Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f, l_tractor_RA, mass_tractor);


        // Semi-Truck Trailer
        } else {
            double lim_trailer = 0, mass_trailer = 0;
            int l_kingpin_RA = 0, l_kingpin_TA = 0, l_cargo_TA = 0, l_trailer_TA = 0;

            if (var.equals("Max_Mass_TrailerAxle"))				    lim_trailer = scanner.nextDouble();

            var = scanner.next();
            if (var.equals("Distance_Kingpin_RearAxle"))			l_kingpin_RA = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Distance_Kingpin_TrailerAxle"))		    l_kingpin_TA = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Distance_CargoSpace_TrailerAxle"))	    l_cargo_TA = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Distance_Mass_Tractor_RearAxle"))		l_tractor_RA = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Distance_Mass_Trailer_TrailerAxle"))	l_trailer_TA = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Mass_Tractor"))						    mass_tractor = scanner.nextDouble();

            var = scanner.next();
            if (var.equals("Mass_Trailer"))						    mass_trailer = scanner.nextDouble();

            int volume = l * w * h;
            return new Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, lim_trailer, l_kingpin_RA, l_kingpin_TA,
                    l_cargo_TA, l_tractor_RA, l_trailer_TA, mass_tractor, mass_trailer);

        }

    }

    /**
     * Read a Customer.
     * Get Customer Data (coordinates, total demand, demanded volume, demanded mass)
     *
     * @param scanner to read the file
     * @return the Customer
     */
    private static Customer readCustomer(Scanner scanner) {
        int  id             = scanner.nextInt();
        float x             = scanner.nextFloat();
        float y             = scanner.nextFloat();
        int demand          = scanner.nextInt();
        int readyTime       = scanner.nextInt();
        int dueDate         = scanner.nextInt();
        int serviceTime     = scanner.nextInt();
        float demandedMass  = scanner.nextFloat();
        int demandedVolume  = scanner.nextInt();

        return new Customer(id, x, y, demand, readyTime, dueDate, serviceTime, demandedMass, demandedVolume);
    }

    /**
     * Read an ItemType.
     * Get ItemType Data (e.g. Dimensions, Fragility Flag, LBS Values)
     *
     * @param scanner to read the file
     * @return the Customer
     */
    private static ItemType readItemType(Scanner scanner) {
        String btStr = scanner.next();
        int typeId  = Integer.parseInt(btStr.replace("Bt", ""));
        int l       = scanner.nextInt();
        int w       = scanner.nextInt();
        int h       = scanner.nextInt();
        float mass  = scanner.nextFloat();
        int intFrag = scanner.nextInt();
        boolean fragile = intFrag != 0;
        double lbs      = scanner.nextDouble();

        return new ItemType(typeId, l, w, h, mass, fragile, lbs);
    }

    /**
     * Read a Demand.
     * Get demanded ItemType and the corresponding quantity.
     *
     * @param scanner to read the file
     * @return the Demand
     */
    private static Demand readDemand(Scanner scanner) {
        String btStr = scanner.next();
        int typeId = Integer.parseInt(btStr.replace("Bt", ""));
        int quantity = scanner.nextInt();
        return new Demand(typeId, quantity);
    }


    /**
     * Read constraint file.
     *
     *
     * @param constraintPath the path to the constraint file.
     * @return the constraint set
     */
    public static ConstraintSet readConstraintFile(final String constraintPath, final boolean hasTimeWindows) {
        try {
            Scanner scanner = new Scanner(new File(constraintPath));
            scanner.useLocale(Locale.US);

            float alpha = 0, balanced_part = 0;
            int lambda = 0, intRot = 0, intLoad = 0, intSeq = 0, intStab = 0, intStack = 0, intReach = 0,
                    intAx = 0, intBal = 0, intTW = hasTimeWindows ? 1 : 0, intSplit = 0;
            scanner.nextLine();  // Skip first Line

            String var = scanner.next();
            if (var.equals("alpha"))					alpha = scanner.nextFloat();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("lambda"))				lambda = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("balanced_part"))			balanced_part = scanner.nextFloat();

            scanner.nextLine();
            scanner.nextLine();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("rotation"))				intRot = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("capacity"))				intLoad = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("unloading_sequence"))	intSeq = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("vertical_stability"))	intStab = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("stacking"))				intStack = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("reachability"))			intReach = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("axle_weights"))			intAx = scanner.nextInt();

            scanner.nextLine(); var = scanner.next();
            if (var.equals("balancing"))			intBal = scanner.nextInt();

            if (scanner.hasNextLine()) {
                scanner.nextLine();
                if (scanner.hasNext()) {
                    var = scanner.next();
                    if (var.equals("TimeWindows")) intTW = scanner.nextInt();
                }
            }

            if (scanner.hasNextLine()) {
                scanner.nextLine();
                if (scanner.hasNext()) {
                    var = scanner.next();
                    if (var.equals("SplitDelivery")) intSplit = scanner.nextInt();
                }
            }

            boolean rotation = intRot != 0;
            boolean load_capacity = intLoad != 0;
            UnloadingSequence uSequence = UnloadingSequence.values()[intSeq];
            VerticalStability vStability = VerticalStability.values()[intStab];
            Stacking stacking = Stacking.values()[intStack];
            boolean reachability = intReach != 0;
            boolean axle_weights = intAx != 0;
            boolean balanced_loading = intBal != 0;
            boolean timeWindows = intTW != 0;
            boolean split = intSplit != 0;


            return new ConstraintSet(rotation, load_capacity, uSequence, vStability,
                    stacking, reachability, axle_weights, balanced_loading, alpha,
                    lambda, balanced_part, timeWindows, split);

        }
        catch (FileNotFoundException nfe) {
            System.err.println("Could not open ConstraintSet file " + constraintPath);
            return null;
        }

    }

    /**
     * Read solution file.
     *
     * @param solutionPath the path to the solution file
     * @param instance     the corresponding instance
     * @return the solution
     */
    public static Solution readSolutionFile(String solutionPath, Instance instance) {
        try {
            String name = "", problem = "", cSet = "";
            int vUsed = 0, iterations = 0;
            float calculationTime = 0;
            double totalTravelDistance = 0;
            double totalTravelTime = 0;
            double unloadingEffort = 0;
            List<Tour> tours = new ArrayList<>();

            Scanner scanner = new Scanner(new File(solutionPath));
            scanner.useLocale(Locale.US);

            String var = scanner.next();
            if (var.equals("Name:"))          name = scanner.next();
            if (!name.equals(instance.getName())) {
				throw new IOException("Wrong Packing Plan found. Expected: " + instance.getName() + ", read: " + name);

            }

            var = scanner.next();
            if (var.equals("Problem:"))						problem = scanner.next();

            var = scanner.next();
            if (var.equals("Number_of_used_Vehicles:"))		vUsed = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Total_Travel_Distance:"))		totalTravelDistance = scanner.nextDouble();

            var = scanner.next();
            if (var.equals("Total_Unloading_Effort:")) {
                unloadingEffort = scanner.nextFloat();
                var = scanner.next();
                if (var.equals("Total_Travel_Time:"))		totalTravelTime = scanner.nextDouble();
            }

            var = scanner.next();
            if (var.equals("Calculation_Time:"))			calculationTime = scanner.nextFloat();

            var = scanner.next();
            if (var.equals("Total_Iterations:"))			iterations = scanner.nextInt();

            var = scanner.next();
            if (var.equals("ConstraintSet:"))				cSet = scanner.next();

            scanner.nextLine();
            scanner.nextLine();

            // Read Routes
            for (int r = 1; r <= vUsed; ++r) {
                scanner.nextLine();

                int id = 0, noItems = 0, noCustomers = 0, customerId = 0;
                List<Integer> customerIds = new ArrayList<>();

                do {
                    var = scanner.next();
                } while (scanner.hasNextLine() && !var.startsWith("Tour_Id"));

                id = scanner.nextInt();
                if (r != id) {
					throw new IOException("Wrong TourId found. Expected: " + r + ", read: " + id);
                }

                var = scanner.next();
                if (var.equals("No_of_Customers:"))		    noCustomers = scanner.nextInt();

                var = scanner.next();
                if (var.equals("No_of_Items:"))			    noItems = scanner.nextInt();

                var = scanner.next();
                if (var.equals("Customer_Sequence:")) {
                    for (int i = 0; i < noCustomers; ++i) {
                        customerId = scanner.nextInt();
                        customerIds.add(customerId);
                    }
                }
                if (noCustomers > 1) {
                    scanner.nextLine();
                    scanner.nextLine();
                    scanner.nextLine();

                    // Read Items
                    List<Integer> itemIds = new ArrayList<>();
                    for (int i = 0; i < noItems; ++i) {
                        Item item = readItem(scanner, instance);
                        itemIds.add(item.getId());
                        instance.getItems().set(item.getId(), item);
                        instance.getCustomers().get(item.getCustomer_id()).getItem_ids().add(item.getId());
                    }

                    tours.add(new Tour(r, customerIds, itemIds));
                    scanner.nextLine();
                    scanner.nextLine();

                } else {
                    instance.getCustomers().get(customerId).getDemands().clear();
                    tours.add(new Tour(r, customerIds, new ArrayList<>()));
                }
            }
            return new Solution(tours, totalTravelDistance, totalTravelTime, unloadingEffort,
                    true, iterations, calculationTime);

        } catch (FileNotFoundException nfe) {
            System.err.println("Could not open Packing Plan file " + solutionPath);
            return null;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Read an Item with its position.
     *
     * @param scanner      to read the file.
     * @param instance     the corresponding instance
     * @return the Item
     */
    private static Item readItem(Scanner scanner, Instance instance) {
        int customer_id = scanner.nextInt();
        int id = scanner.nextInt();
        int typeId = scanner.nextInt();
        int intRot = scanner.nextInt();
        boolean rotated = intRot != 0;
        int x = scanner.nextInt();
        int y = scanner.nextInt();
        int z = scanner.nextInt();
        scanner.nextLine();

        ItemType itemtype = instance.getItemTypes().get(typeId);

        Item item = new Item(id, customer_id, rotated, itemtype);
        item.setPosition(new Point(x, y, z));

        return item;
    }
}
