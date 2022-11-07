package com.threedimensionalloadingcvrp.validator.constraints;

import com.threedimensionalloadingcvrp.validator.model.*;
import java.util.Collections;
import java.util.HashSet;

import static java.lang.Math.pow;
import static java.lang.Math.sqrt;

/**
 * Class with all Routing Constraints.
 */
public class Routing {
    /**
     * Check solution concerning the feasibility of all routing constraints defined in the constraint set.
     *
     * @param solution the solution
     * @param cSet     the constraint set
     * @param instance the instance
     * @param msg      optional output messages
     * @return the feasibility of the solution w.r.t. routing constraints
     */
    public static boolean checkRoutingConstraints(Solution solution, final ConstraintSet cSet, Instance instance, final boolean msg) {
        if (solution == null) throw new NullPointerException("Solution is null");
        if (cSet == null) throw new NullPointerException("ConstraintSet is null");
        if (instance == null) throw new NullPointerException("Instance is null");

        if (!checkTimeWindows(solution, instance, cSet.isTimeWindows(), msg)
                || !checkCapacities(solution, cSet.isLoad_capacity(), instance, msg)
                || !checkDispatchedCustomers(solution, cSet.isSplit(), instance, msg)
                || !checkDispatchedItems(solution, instance, msg)
                || !checkUsedVehicles(solution, instance, msg)
                || !checkCost(solution, instance, msg)) {
            if (msg) System.err.println("Solution is NOT feasible according Routing Constraints.");
            return false;
        }
        if (msg) System.out.println("Solution is feasible according Routing Constraints.");
        return true;
    }

    /**
     * Check the calculation of the total travel distance.
     *
     * @param solution the solution
     * @param instance the instance
     * @param msg      optional output messages
     * @return feasibility of the calculation.
     */
    public static boolean checkCost(final Solution solution, final Instance instance, final boolean msg) {
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
            if (msg) System.err.println("Total Travel Distance is wrong");
            return false;
        }
        return true;
    }

    /**
     * Check if all time windows are met.
     *
     * @param solution the solution
     * @param instance the instance
     * @param msg      optional output messages
     * @return feasibility of time windows.
     */
    public static boolean checkTimeWindows(final Solution solution, final Instance instance, boolean tw, final boolean msg) {
        if (tw) {
            Customer depot = instance.getCustomers().get(0);
            for (Tour tour : solution.getTours()) {
                int customer_last_id = 0;
                double time = depot.getServiceTime();
                // Check Customers
                for (int customer_id : tour.getCustomer_ids()) {
                    time = calculateTime(customer_id, customer_last_id, time, instance, msg);
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
                    if (msg) System.err.println("Arrived at Depot after Due Date.");
                    return false;
                }
            }
        }
        return true;

    }

    /**
     * Calculate the current time after each customer.
     *
     * @param customer_id      the customer id
     * @param customer_last_id the customer last id
     * @param time             the current time before visiting the customer.
     * @param instance         the instance
     * @return the current time after visiting the customer.
     */
    public static double calculateTime(final int customer_id, final int customer_last_id, double time, final Instance instance, final boolean msg) {
        final Customer customer_current = instance.getCustomers().get(customer_id);
        final Customer customer_last = instance.getCustomers().get(customer_last_id);
        final double distance = calcEuclideanDistance(customer_last.getX(), customer_last.getY(), customer_current.getX(), customer_current.getY());
        time += distance;
        if (time < customer_current.getReadyTime()) {
            double waiting_time = customer_current.getReadyTime() - time;
            time += waiting_time;
        }
        if (time > customer_current.getDueDate()) {
            if (msg) System.err.println("Arrived at Customer " + customer_id + "after Due Date.");
            return -1;
        }
        time += customer_current.getServiceTime();
        return time;
    }

    /**
     * Calculate the euclidean distance between Point a and Point b.
     *
     * @param ax the x-coordinate of Point a.
     * @param ay the y-coordinate of Point a.
     * @param bx the x-coordinate of Point b.
     * @param by the y-coordinate of Point a.
     * @return the euclidean distance.
     */
    public static double calcEuclideanDistance(final float ax, final float ay, final float bx, final float by) {
        float x = ax - bx;
        float y = ay - by;
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    /**
     * Check if the vehicle dimensions are met.
     *
     * @param solution      the solution
     * @param load_capacity the consideration of the load capacity
     * @param instance      the instance
     * @param msg           optional output messages
     * @return feasibility of vehicle dimensions (Mass and Volume).
     */
    public static boolean checkCapacities(final Solution solution, final boolean load_capacity, final Instance instance, final boolean msg) {
        for (final Tour tour : solution.getTours()) {
            double sumVolume = 0;
            double sumMass = 0;
            for (int customer_id : tour.getCustomer_ids()) {
                final Customer customer = instance.getCustomers().get(customer_id);
                sumVolume += customer.getDemandedVolume();
                sumMass += customer.getDemandedMass();
            }

            if (sumVolume > instance.getVehicle().getLoad_volume()) {
                if (msg) System.err.println("Exceeding Volume in Tour " + tour.getId());
                return false;
            }
            if (sumMass > instance.getVehicle().getD() && load_capacity) {
                if (msg) System.err.println("Exceeding Mass in Tour:  " + tour.getId());
                return false;
            }
        }
        return true;

    }

    /**
     * Check if all customers are dispatched.
     *
     * @param solution the solution
     * @param split    split deliveries
     * @param instance the instance
     * @param msg      optional output messages
     * @return true if all customers are visited.
     */
    public static boolean checkDispatchedCustomers(final Solution solution, final boolean split, final Instance instance, final boolean msg) {
        // Without Split Deliveries: Each customer is visited exactly once;
        // Constraint: Each route visits at least one customer;

        HashSet<Integer> dispatched_customers = new HashSet<>();
        for (Tour tour : solution.getTours()) {
            if (tour.getCustomer_ids().size() == 0
                    || Collections.frequency(tour.getCustomer_ids(), 0) == tour.getCustomer_ids().size()) {
                if (msg) System.err.println("Tour " + tour.getId() + " without customers.");
                return false;
            }

            for (final int customer_id : tour.getCustomer_ids()) {
                if (dispatched_customers.contains(customer_id)) {
                    if (!split) {
                        if (msg) System.err.print("Customer " + customer_id + " dispatched several times.");
                        return false;
                    }
                } else {
                    dispatched_customers.add(customer_id);
                }
            }
        }

        if (dispatched_customers.size() < instance.getCustomers().size() - 1) {
            if (msg) System.err.println("Not all Customers are dispatched.");
            return false;
        }

        return true;
    }

    /**
     * Check if all items of all customers are dispatched.
     *
     * @param solution the solution
     * @param instance the instance
     * @param msg      optional output messages
     * @return true if all items of all customers are dispatched.
     */
    public static boolean checkDispatchedItems(final Solution solution, final Instance instance, final boolean msg) {
        for (Customer customer : instance.getCustomers()) {
            for (Demand demand : customer.getDemands()) {
                int qt = 0;
                for (Tour tour : solution.getTours()) {
                    qt += (int) tour.getItem_ids().stream().filter(id -> {
                                Item item = instance.getItems().get(id);
                                return item.getType_id() == demand.getItemTypeId() && item.getCustomer_id() == customer.getId();
                            }).count();
                }
                if (qt != demand.getQuantity()) {
                    if (msg) System.err.println("Customer " + customer.getId() + ": Demand is not fulfilled. Expected items of type " + demand.getItemTypeId() + ": " + demand.getQuantity() + ", dispatched: " + qt);
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Check if the solution exceeds the number of used vehicles.
     * Lazy-Constraint since solution can exceed the number of used vehicles.
     *
     * @param solution the solution
     * @param instance the instance
     * @param msg      optional output messages
     * @return true
     */
    public static boolean checkUsedVehicles(final Solution solution, final Instance instance, final boolean msg) {
        // Not an error since Constraints can lead to an exceedence
        if (solution.getTours().size() > instance.getV_max()) {
            if (msg) System.err.println("Number of used vehicles increases number of available vehicles.");
        }
        return true;
    }
}
