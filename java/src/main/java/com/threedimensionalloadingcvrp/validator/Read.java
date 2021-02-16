package com.threedimensionalloadingcvrp.validator;

import com.threedimensionalloadingcvrp.validator.model.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

import static java.lang.System.exit;

public class Read {

    public static Instance readInstanceFile(final String instancePath) {
        try {
            Scanner scanner = new Scanner(new File(instancePath));
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
                System.err.println("Wrong Line. Should be Header VEHICLE, current: " + var);
                System.exit(-1);
            }
            Vehicle vehicle = readVehicle(scanner);

            var = scanner.next();

            if (!var.equals("CUSTOMERS")) {
                System.err.println("Wrong Line. Should be Header CUSTOMERS, current: " + var);
                System.exit(-1);
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
                System.err.println("Wrong Line. Should be Header ITEMS, current: " + var);
                System.exit(-1);
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
                System.err.println("Wrong Line. Should be Header DEMANDS, current: " + var);
                System.exit(-1);
            }

            // Read Demands
            scanner.nextLine();
            scanner.nextLine();
            for (int i = 1; i <= no_customers; ++i) {
                Customer customer = customers.get(i);
		        final int totalDemand = customers.get(i).getDemand();
                int id = scanner.nextInt();
                if (customer.getId() != id) {
                    System.err.println("Wrong Line. Expected: " + customer.getId() + ", read: " + id);
                    return null;
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


        } catch(FileNotFoundException fne) {
            System.err.println("Could not open Instance file " + instancePath);
            exit(-1);
        }
        return null;
    }

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
        if (var.equals("Distance_FrontAxle_CargoSpace")) {
            l_f = scanner.nextInt();
            final int volume = l * w * h;

            var = scanner.nextLine();
            if (var.isEmpty())	                                return new Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f);

            if (var.equals("Distance_Mass_Truck_RearAxle"))		l_tractor_RA = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Mass_Truck"))						mass_tractor = scanner.nextDouble();

            return new Vehicle(l, w, h, D, volume, wheelbase, lim_frontAxle, lim_rearAxle, l_f, l_tractor_RA, mass_tractor);


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

    private static Demand readDemand(Scanner scanner) {
        String btStr = scanner.next();
        int typeId = Integer.parseInt(btStr.replace("Bt", ""));
        int quantity = scanner.nextInt();
        return new Demand(typeId, quantity);
    }


    public static ConstraintSet readConstraintFile(final String constraintPath) {
        try {
            Scanner scanner = new Scanner(new File(constraintPath));
            scanner.useLocale(Locale.US);

            float alpha = 0, balanced_part = 0;
            int lambda = 0, intRot = 0, intLoad = 0, intSeq = 0, intStab = 0, intStack = 0, intReach = 0, intAx = 0, intBal = 0;

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

            boolean rotation = intRot != 0;
            boolean load_capacity = intLoad != 0;
            UnloadingSequence uSequence = UnloadingSequence.values()[intSeq];
            VerticalStability vStability = VerticalStability.values()[intStab];
            Stacking stacking = Stacking.values()[intStack];
            boolean reachability = intReach != 0;
            boolean axle_weights = intAx != 0;
            boolean balanced_loading = intBal != 0;


            return new ConstraintSet(rotation, load_capacity, uSequence, vStability,
                    stacking, reachability, axle_weights, balanced_loading, alpha, lambda, balanced_part);

        }
        catch (FileNotFoundException nfe) {
            System.err.println("Could not open ConstraintSet file " + constraintPath);
            System.exit(-1);
        }

        return null;
    }

    public static Solution readSolutionFile(String solutionPath, Instance instance) {
        try {
            String name = "", problem = "", cSet = "";
            int vUsed = 0, iterations = 0;
            float calculationTime = 0;
            double totalTravelDistance = 0;
            List<Tour> tours = new ArrayList<>();

            Scanner scanner = new Scanner(new File(solutionPath));
            scanner.useLocale(Locale.US);

            String var = scanner.next();
            if (var.equals("Name:"))          name = scanner.next();
            if (!name.equals(instance.getName())) {
                System.err.println("Wrong Packing Plan found. Expected: " + instance.getName() + ", read: " + name);
                System.exit(-1);
            }

            var = scanner.next();
            if (var.equals("Problem:"))						problem = scanner.next();

            var = scanner.next();
            if (var.equals("Number_of_used_Vehicles:"))		vUsed = scanner.nextInt();

            var = scanner.next();
            if (var.equals("Total_Travel_Distance:"))		totalTravelDistance = scanner.nextDouble();

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
                    System.err.println("Wrong TourId found. Expected: " + r + ", read: " + id);
                    System.exit(-1);
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
            return new Solution(tours, totalTravelDistance, true, iterations, calculationTime);

        } catch (FileNotFoundException nfe) {
            System.err.println("Could not open Packing Plan file " + solutionPath);
            System.exit(-1);
        }
        return null;
    }

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
