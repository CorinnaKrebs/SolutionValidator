package com.threedimensionalloadingcvrp.validator.constraints;

import com.threedimensionalloadingcvrp.validator.model.*;
import java.util.Collections;
import java.util.HashSet;

import static java.lang.Math.pow;
import static java.lang.Math.sqrt;

public class Routing {
    public static boolean checkRoutingConstraints(Solution solution, final ConstraintSet cSet, Instance instance) {
        if (!checkTimeWindows(solution, instance)
                || !checkCapacities(solution, cSet.isLoad_capacity(), instance)
                || !checkDispatchedCustomers(solution, instance)
                || !checkDispatchedItems(solution, instance)
                || !checkUsedVehicles(solution, instance)
                || !checkCost(solution, instance)) {
            System.err.println("Solution is NOT feasible according Routing Constraints.");
            return false;
        }
        //System.out.println("Solution is feasible according Routing Constraints.");
        return true;
    }

    public static boolean checkCost(final Solution solution, final Instance instance) {
        double totalCost = 0;
        for (Tour tour : solution.getTours()) {
            tour.getCustomer_ids().removeAll(Collections.singleton(0));
            int idI = 0;
            for (int idJ : tour.getCustomer_ids()) {
                Customer customerI = instance.getCustomers().get(idI);
                Customer customerJ = instance.getCustomers().get(idJ);
                totalCost += calcEuclideanDistance(customerI.getX(), customerI.getY(), customerJ.getX(), customerJ.getY());
                idI = idJ;
            }
            Customer customerI = instance.getCustomers().get(idI);
            Customer customerJ = instance.getCustomers().get(0);
            totalCost += calcEuclideanDistance(customerI.getX(), customerI.getY(), customerJ.getX(), customerJ.getY());
        }
        double solCost = solution.getTotal_travel_distance();
        if (Math.max(totalCost * 0.999, solCost) != Math.min(totalCost * 1.003, solCost)) {
            System.err.println("Total Travel Distance is wrong");
            return false;
        }
        return true;
    }

    public static boolean checkTimeWindows(final Solution solution, final Instance instance) {
        if (instance.isTw()) {
            Customer depot = instance.getCustomers().get(0);
            for (Tour tour : solution.getTours()) {
                int customer_last_id = 0;
                double time = depot.getServiceTime();
                // Check Customers
                for (int customer_id : tour.getCustomer_ids()) {
                    time = calculateTime(customer_id, customer_last_id, time, instance);
                    if (time < 0) {
                        return false;
                    }
                    customer_last_id = customer_id;
                }

                // Check DueDate of Depot
                Customer customer_last = instance.getCustomers().get(customer_last_id);
                double distance = calcEuclideanDistance(customer_last.getX(), customer_last.getY(), depot.getX(), depot.getY());
                time += distance;
                if (time > depot.getDueDate()) {
                    System.err.println("Arrived at Depot after Due Date.");
                    return false;
                }
            }
        }
        return true;

    }

    public static double calculateTime (final int customer_id, final int customer_last_id, double time, final Instance instance) {
        final Customer customer_current = instance.getCustomers().get(customer_id);
        final Customer customer_last    = instance.getCustomers().get(customer_last_id);
        final double distance = calcEuclideanDistance(customer_last.getX(), customer_last.getY(), customer_current.getX(), customer_current.getY());
        time += distance;
        if (time < customer_current.getReadyTime()) {
            double waiting_time = customer_current.getReadyTime() - time;
            time += waiting_time;
        }
        if (time > customer_current.getDueDate()) {
            System.err.println("Arrived at Customer " + customer_id + "after Due Date.");
            return -1;
        }
        time += customer_current.getServiceTime();
        return time;
    }

    public static double calcEuclideanDistance(final float ax, final float ay, final float bx, final float by) {
        float x = ax - bx;
        float y = ay - by;
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    public static boolean checkCapacities(final Solution solution, final boolean load_capacity, final Instance instance) {
        for (final Tour tour : solution.getTours()) {
            double sumVolume = 0;
            double sumMass = 0;
            for (int customer_id : tour.getCustomer_ids()) {
			final Customer customer = instance.getCustomers().get(customer_id);
                sumVolume += customer.getDemandedVolume();
                sumMass += customer.getDemandedMass();
            }

            if (sumVolume > instance.getVehicle().getLoad_volume()) {
                System.err.println("Exceeding Volume in Tour " + tour.getId());
                return false;
            }
            if (sumMass > instance.getVehicle().getD() && load_capacity) {
                System.err.println("Exceeding Mass in Tour:  " + tour.getId());
                return false;
            }
        }
        return true;

    }
    public static boolean checkDispatchedCustomers(final Solution solution, final Instance instance) {
        // Constraint: Each customer is visited exactly once;
        // Constraint: Each route visits at least one customer;

        HashSet<Integer> dispatched_customers = new HashSet<>();
        for (Tour tour : solution.getTours()) {
            if (tour.getCustomer_ids().size() == 0
                    || Collections.frequency(tour.getCustomer_ids(), 0) == tour.getCustomer_ids().size()) {
                System.err.println("Tour " + tour.getId() + " without customers.");
                return false;
            }

            for (final int customer_id : tour.getCustomer_ids()) {
                if (dispatched_customers.contains(customer_id)) {
                    System.err.print("Customer " + customer_id + " dispatched several times.");
                    return false;
                }

                dispatched_customers.add(customer_id);
            }
        }

        if (dispatched_customers.size() < instance.getCustomers().size() - 1) {
            System.err.println("Not all Customers are dispatched.");
            return false;
        }

        return true;
    }
    public static boolean checkDispatchedItems(final Solution solution, final Instance instance) {
        for (Tour tour : solution.getTours()) {
            for (int customer_id : tour.getCustomer_ids()) {
                Customer customer = instance.getCustomers().get(customer_id);
                for (Demand demand : customer.getDemands()) {
                    int qt = (int) tour.getItem_ids().stream().filter(id -> {
                        Item item = instance.getItems().get(id);
                        return item.getType_id() == demand.getItemTypeId() && item.getCustomer_id() == customer_id;}
                        ).count();
                    if (qt != demand.getQuantity()) {
                        System.err.println("Customer " + customer.getId() + ": Demand is not fulfiled. Expected items of type " + demand.getItemTypeId() + ": " + demand.getQuantity() + ", dispatched: " + qt);
                        return false;
                    }
                }
            }
        }
        return true;
    }
    public static boolean checkUsedVehicles(final Solution solution, final Instance instance) {
        // Not an error since Constraints can lead to an exceedence
        if (solution.getTours().size() > instance.getV_max()) {
            System.err.println("Number of used vehicles increases number of available vehicles.");
        }
        return true;
    }
}
