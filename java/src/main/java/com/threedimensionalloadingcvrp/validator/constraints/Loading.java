package com.threedimensionalloadingcvrp.validator.constraints;

import com.threedimensionalloadingcvrp.validator.model.*;

import java.util.*;

import static java.lang.Math.max;
import static java.lang.Math.min;
import static java.lang.StrictMath.pow;

/**
 * Class with all Loading Constraints
 */
public class Loading {
    /**
     * Check solution concerning the feasibility of all loading constraints defined in the constraint set.
     *
     * @param solution the solution
     * @param cSet     the constraint set
     * @param instance the instance
     * @return the feasibility of the solution w.r.t. loading constraints
     */
    public static boolean checkLoadingConstraints(Solution solution, final ConstraintSet cSet, final Instance instance) {
		if (solution == null) throw new NullPointerException("Solution is null");
        if (cSet == null) throw new NullPointerException("ConstraintSet is null");
        if (instance == null) throw new NullPointerException("Instance is null");

        final int scaleFactor = getScaleFactor(instance.getVehicle().getH());

        for (Tour tour : solution.getTours()) {
            tour.setFeasible(true);
            // Necessary for some constraints beforehand
            getRelevantItems(tour, tour.getItem_ids().size(), instance);

            final int endPos = tour.getItem_ids().size();
            for (final int item_id : tour.getItem_ids()) {
                Item currentItem = instance.getItems().get(item_id);
                if (!checkVehicleWalls(currentItem, instance.getVehicle(), true)
                        || !checkMaxCoordinates(currentItem, true)
                        || !checkDimensions(currentItem, true)
                        || !checkOverlapping(currentItem, tour, endPos, instance, true)
                        || !checkAxleWeights(currentItem, tour, cSet.isAxle_weights(), instance, true)
                        || !checkBalancedLoading(currentItem, tour, cSet.isBalanced_loading(), cSet.getBalanced_part(), instance, true)
                        || !checkUnloadingSequence(currentItem, tour, endPos, cSet.getUSequence(), instance, true)
                        || !checkReachability(currentItem, tour, cSet.getLambda() * scaleFactor, cSet.isReachability(), instance, true)
                        || !checkVStability(currentItem, cSet.getVStability(), cSet.getAlpha(), instance, true)
                        || !checkStacking(currentItem, tour, endPos, cSet.getStacking(), instance, true)) {
                    System.err.println("Solution is NOT feasible according Loading Constraints.");
                    tour.setFeasible(false);
                    return false;
                }
            }
        }
        System.out.println("Solution is feasible according Loading Constraints.");
        return true;
    }

    /**
     * Check if an item overlaps with the vehicle walls.
     *
     * @param item    the item
     * @param vehicle the vehicle
     * @param msg     optional output messages
     * @return feasibility of item position.
     */
    public static boolean checkVehicleWalls(final Item item, final Vehicle vehicle, final boolean msg) {
        if (item.getMax().getX() > vehicle.getL() || item.getMax().getY() > vehicle.getW() || item.getMax().getZ() > vehicle.getH()) {
            if (msg) System.err.println("Overlapping of item " + item.getId() + " with Vehicle Walls.");
            return false;
        }
        return true;
    }

    /**
     * Check if item position does not lead to an exceedence of the axle weights.
     *
     * @param item         the item
     * @param tour         the tour
     * @param axle_weights the consideration of axle weights
     * @param instance     the instance
     * @param msg          optional output messages
     * @return feasibility of item position.
     */
    public static boolean checkAxleWeights(Item item, Tour tour, final boolean axle_weights, final Instance instance, final boolean msg) {
        if (axle_weights) {
            final double g = 9.81;
            long item_T = 0;

            // Calculate Item's Force
            double item_F = item.getMass() * g;
            int length = !item.isRotated() ? item.getL() : item.getW();

            long F_RA = 0, F_FA = 0, F_TA = 0;

            // Calculate Item's Torque
            if (!instance.getVehicle().isAxle_trailer()) {
                double s = (double) instance.getVehicle().getL_FA_cargo() + item.getMin().getX() + 0.5 * length;
                item_T = (long) (s * item_F);

                // Calculate Axle Weights
                int l_truck_FA = instance.getVehicle().getWheelbase() - instance.getVehicle().getL_tractor_RA();
                F_RA = (long) ((tour.getSum_T() + item_T + instance.getVehicle().getMass_tractor() * g * l_truck_FA) / (double) instance.getVehicle().getWheelbase());
                F_FA = (long) (tour.getSum_F() + item_F + instance.getVehicle().getMass_tractor() * g) - F_RA;

            } else {
                // Calculate Item's Torque
                item_T = (long) (item_F * (instance.getVehicle().getL_cargo_TA() - item.getMin().getX() - (0.5 * length)));

                // Calculate Axle Weights
                long F_K = (long) ((1.0 / instance.getVehicle().getL_kingpin_TA()) * (tour.getSum_T() + item_T + instance.getVehicle().getMass_trailer() * g * instance.getVehicle().getL_trailer_TA()));

                F_TA = (long) (tour.getSum_F() + item_F + instance.getVehicle().getMass_trailer() * g - F_K);

                F_FA = (long) ((1.0 / instance.getVehicle().getWheelbase()) * (F_K * instance.getVehicle().getL_kingpin_RA() + instance.getVehicle().getMass_tractor() * g * instance.getVehicle().getL_tractor_RA()));

                F_RA = (long) (F_K + instance.getVehicle().getMass_tractor() * g - F_FA);
            }

            tour.setSum_F(tour.getSum_F() + item_F);
            tour.setSum_T(tour.getSum_T() + item_T);

            // Check Axle Weight Limits
            if (F_RA > instance.getVehicle().getLim_rearAxle() * g) {
                if (msg) System.err.println("Overloaded Rear Axle of Tour " + tour.getId());
                return false;
            }

            if (F_TA > instance.getVehicle().getLim_trailerAxle() * g) {
                if (msg) System.err.println("Overloaded Trailer Axle of Tour " + tour.getId());
                return false;
            }

            if (F_FA > instance.getVehicle().getLim_frontAxle() * g) {
                if (msg) System.err.println("Overloaded Front Axle of Tour " + tour.getId());
                return false;
            }
        }
        return true;
    }

    /**
     * Check if item position does not lead to an exceedence of the permissible load per vehicle half.
     * The load of one vehicle half does not exceed a certain percentage of the vehicle capacity.
     *
     * @param item             the item
     * @param tour             the tour
     * @param balanced_loading the consideration of balanced loading
     * @param balanced_part    the part of the vehicle capacity
     * @param instance         the instance
     * @param msg              optional output messages
     * @return feasibility of item position.
     */
    public static boolean checkBalancedLoading(Item item, Tour tour, final boolean balanced_loading, final float balanced_part, final Instance instance, final boolean msg) {
        if (balanced_loading) {
		    final double limit = instance.getVehicle().getD() * balanced_part;
            double W_half = instance.getVehicle().getW() * 0.5;

            // In Left Half
            if (item.getMax().getY() <= W_half) {
                tour.setMass_L(tour.getMass_L() + item.getMass());
            }

            // In Right Half
            else if (item.getMin().getY() > W_half) {
                tour.setMass_R(tour.getMass_R() + item.getMass());
            }
            // Distribute Mass
            else {
                double width = !item.isRotated() ? item.getW() : item.getL();
                double left_area = (W_half - item.getMin().getY()) * width;
                double part = left_area / ((double) item.getW() * item.getL());
                double mass_left = part * item.getMass();
                tour.setMass_L(tour.getMass_L() + mass_left);
                tour.setMass_R(tour.getMass_R() + item.getMass() - mass_left);
            }

            // Check the Limits
            if (tour.getMass_L() > limit) {
                if (msg) System.err.println("Overloaded Left Vehicle Half of Tour" + tour.getId());
                return false;
            }

            if (tour.getMass_R() > limit) {
                if (msg) System.err.println("Overloaded Right Vehicle Half of Tour " + tour.getId());
                return false;
            }
        }
        return true;
    }

    /**
     * Check if an item overlaps with any already placed item.
     *
     * @param item       the item
     * @param tour       the tour
     * @param tourEndPos the current position of the packing process
     * @param instance   the instance
     * @param msg        optional output message
     * @return feasibility of item position.
     */
    public static boolean checkOverlapping(Item item, Tour tour, final int tourEndPos, final Instance instance, final boolean msg) {
        for (int i = 0; i < tourEndPos; ++i) {
            int item_id = tour.getItem_ids().get(i);
            if (item_id == item.getId()) continue;
            Item other = instance.getItems().get(item_id);

            if (item.getMin().getX() < other.getMax().getX()
                    && item.getMin().getY() < other.getMax().getY()
                    && item.getMin().getZ() < other.getMax().getZ()
                    && other.getMin().getX() < item.getMax().getX()
                    && other.getMin().getY() < item.getMax().getY()
                    && other.getMin().getZ() < item.getMax().getZ()) {
                if (msg) System.err.println("Overlapping of item " + item.getId() + " and Item " + other.getId());
                return false;
            }
        }
        return true;
    }

    /**
     * Check if the dimension of an item are calculated correctly.
     *
     * @param item the item
     * @param msg  optional output message
     * @return correctness of item dimensions
     */
    public static boolean checkDimensions(final Item item, final boolean msg) {
        int l = item.getMax().getX() - item.getMin().getX();
        int w = item.getMax().getY() - item.getMin().getY();
        int h = item.getMax().getZ() - item.getMin().getZ();
        if (l + w + h != item.getL() + item.getW() + item.getH()) {
            if (msg) System.err.println("Dimensions of " + item.getId() + " not fitting.");
            return false;
        }
        return true;

    }

    /**
     * Check if the maximum corner point is not zero.
     *
     * @param item the item
     * @param msg  optional output message
     * @return correctness of maximum corner point
     */
    public static boolean checkMaxCoordinates(final Item item, final boolean msg) {
        if (item.getMax().equals(new Point(0, 0, 0))) {
            if (msg) System.err.println("Maximal Coordinates of item " + item.getId() + " is Origin.");
            return false;
        }
        return true;
    }

    /**
     * Check item position w.r.t. Stacking Constraints.
     *
     * @param item       the item
     * @param tour       the tour
     * @param tourEndPos the current position of the packing process
     * @param stacking   the considered stacking constraint
     * @param instance   the instance
     * @param msg        optional output message
     * @return feasibility of item position.
     */
    public static boolean checkStacking(Item item, Tour tour, int tourEndPos, final Stacking stacking, Instance instance, final boolean msg) {
        if (stacking == Stacking.LBSComplete || stacking == Stacking.LBSSimple) {
            final int area = item.getL() * item.getW();
            List<Double> list = new ArrayList<>(Collections.nCopies(area, 0.0));
            item.setLoad(list);
        }

        switch (stacking) {
            case Fragility: 	return checkFragility(item, tour, tourEndPos, instance, msg);
            case LBSSimple:     return checkLBSSimple(item, instance, msg);
            case LBSComplete:	return checkLBSComplete(item, instance, item.getMass(), msg);
            case none:		    return true;
        }
        return true;

    }

    /**
     * Check item position w.r.t. Vertical Stability Constraints.
     *
     * @param item       the item
     * @param vStability the considered vertical stability constraint
     * @param alpha      support parameter (part of base area)
     * @param instance   the instance
     * @param msg        optional output message
     * @return feasibility of item position.
     */
    public static boolean checkVStability(Item item, final VerticalStability vStability, final float alpha, Instance instance, final boolean msg) {
        // Item laying at the ground
        if (item.getMin().getZ() == 0) {
            item.setSupport(1.0);
            return true;
        }

        switch (vStability) {
            case MinimalSupport:        return checkMinimalSupport(item, alpha, instance, msg);
            case MultipleOverhanging:   return checkMultipleOverhanging(item, alpha, instance, msg);
            case TopOverhanging:        return checkTopOverhanging(item, alpha, instance, msg);
            case StaticsStability:      return checkStaticsStability(item, instance, msg);
            case none:                  return checkMinimalSupport(item, 0.1f, instance, msg);
        }
        return true;
    }

    /**
     * Check item position w.r.t. Unloading Sequence Constraints.
     *
     * @param item       the item
     * @param tour       the tour
     * @param tourEndPos the current position of the packing process
     * @param uSequence  the considered Unloading Sequence constraint
     * @param instance   the instance
     * @param msg        optional output message
     * @return feasibility of item position.
     */
    public static boolean checkUnloadingSequence(Item item, Tour tour, int tourEndPos, final UnloadingSequence uSequence, Instance instance, final boolean msg) {
        if (uSequence != UnloadingSequence.none) {
            for (int i = 0; i < tourEndPos; ++i) {
                final int item_id = tour.getItem_ids().get(i);
                Item other = instance.getItems().get(item_id);
                if (item.getCustomer_id() == other.getCustomer_id()) {
                    continue;
                }
                // Get Positions of Customers in Tour
                int posCurrentCustomer = tour.getCustomer_ids().indexOf(item.getCustomer_id());
                int posOtherCustomer = tour.getCustomer_ids().indexOf(other.getCustomer_id());
                // Other Customer is Delivered After Current
                if (posOtherCustomer > posCurrentCustomer) {
                    if (item.isBehind(other)) {
                        if (msg) System.err.println("Unloading Sequence for Item " + item.getId() + " not respected due to Item " + other.getId());
                        return false;
                    }
                    if ((uSequence == UnloadingSequence.LIFO && item.isBelow(other, false)) ||
                            (uSequence == UnloadingSequence.MLIFO && item.isBelow(other, true))) {
                        if (msg) System.err.println("Unloading Sequence for Item " + item.getId() + " not respected due to Item " + other.getId());
                        return false;
                    }
                }
            }
        }
        return true;

    }

    /**
     * Check item position w.r.t. Fragility Constraint.
     * Non-Fragile Items cannot be stacked on fragile items.
     * Fragile Item can be stacked on top of fragile items.
     *
     * @param item       the item
     * @param tour       the tour
     * @param tourEndPos the current position of the packing process
     * @param instance   the instance
     * @param msg        optional output message
     * @return feasibility of item position.
     */
    public static boolean checkFragility(Item item, Tour tour, int tourEndPos, Instance instance, final boolean msg) {
        for (int i = 0; i < tourEndPos; ++i) {
            final int item_id = tour.getItem_ids().get(i);
            Item other = instance.getItems().get(item_id);
            if (other.isBelow(item, true) && other.isFragile() && !item.isFragile()) {
                if (msg) System.err.println("Item " + item.getId() + " is placed on top of fragile item " + other.getId());
                return false;
            }
        }
        return true;
    }


    /**
     * Check item position w.r.t. Minimal Supporting Area Constraint.
     * Each item has a supporting area of at least a percentage alpha of its base area;
     *
     * @param item     the item
     * @param alpha    support parameter (part of base area)
     * @param instance the instance
     * @param msg      optional output message
     * @return feasibility of item position.
     */
    public static boolean checkMinimalSupport(Item item, final float alpha, Instance instance, final boolean msg) {
        item.setSupport(calcSupport(item, instance));

        // Check Support
        if (item.getSupport() < alpha * 0.99) {
            if (msg) System.err.println("Item " + item.getId() + " not enough supported.");
            return false;
        }
        return true;
    }

    /**
     * Check item position w.r.t. Top Overhanging Constraint.
     * All Items of a stack must be fully supported except the topmost item,
     * which is allowed to overhanging respecting the Minimal Supporting Area Constraint.
     *
     * @param item     the item
     * @param alpha    support parameter (part of base area)
     * @param instance the instance
     * @param msg      optional output message
     * @return feasibility of item position.
     */
    public static boolean checkTopOverhanging(Item item, final float alpha, Instance instance, final boolean msg) {
        if (item.getSupport() == 0) {
            item.setSupport(calcSupport(item, instance));
        }

        // Last Item of Stack
        if (item.getItemsAbove().size() == 0 && item.getSupport() < alpha * 0.99) {
            if (msg) System.err.println("Item " + item.getId() + " not enough supported.");
            return false;
        }

        // Other item can hang over current item
        for (int otherId : item.getItemsAbove()) {
            Item other = instance.getItems().get(otherId);
            if (other.isAbove(item, true) && item.getSupport() < 0.99) {
                if (msg) System.err.println("Item " + item.getId() + " not enough supported.");
                return false;
            }
        }
        return true;
    }

    /**
     * Check item position w.r.t. Multiple Overhanging Constraint.
     * Each item has a supporting area of at least a percentage alpha of its base area at any height.
     * Therefore check the Minimal Supporting Area at each level.
     *
     * @param item     the item
     * @param alpha    support parameter (part of base area)
     * @param instance the instance
     * @param msg      optional output message
     * @return feasibility of item position.
     */
    public static boolean checkMultipleOverhanging(Item item, final float alpha, Instance instance, final boolean msg) {
        double min_support = 1.0;
        // Item a is one level
        for (int a_id : item.getItemsBelow()) {
            Item aBelow = instance.getItems().get(a_id);
            double supportArea = calcSupportArea(item, aBelow);

            // Search for Items b, which in the same level of item a
            for (int b_id : item.getItemsBelow()) {
                if (a_id == b_id) continue;
                Item bBelow = instance.getItems().get(b_id);

                // If item b lies in the same level of item a
                if (bBelow.getMax().getZ() >= aBelow.getMax().getZ() && bBelow.getMin().getZ() < aBelow.getMax().getZ())
                    supportArea += calcSupportArea(bBelow, item);
            }
            double support = supportArea / ((double) item.getL() * item.getW());
            min_support = min(support, min_support);
        }
        if (min_support < alpha * 0.99) {
            if (msg) System.err.println("Item " + item.getId() + " not enough supported.");
            return false;
        }
        return true;
    }

	/**
     * Check item position w.r.t. Static Stability Constraint.
     * Each item has a center of gravity, which must be within an item at each level.
     *
     * @param item     the item
     * @param instance the instance
     * @param msg      optional output message
     * @return feasibility of item position.
     */
	public static boolean checkStaticsStability(Item item, Instance instance, final boolean msg) {
        final float x = (float) (0.5 * (item.getMax().getX() - item.getMin().getX()) + item.getMin().getX());
	    final float y = (float) (0.5 * (item.getMax().getY() - item.getMin().getY()) + item.getMin().getY());


        // Item a is one level
        for (int a_id : item.getItemsBelow()) {
            Item aBelow = instance.getItems().get(a_id);
            int minX = aBelow.getMin().getX();
            int maxX = aBelow.getMax().getX();
            int minY = aBelow.getMin().getY();
            int maxY = aBelow.getMax().getY();

            // Search for Items b, which in the same level of item a
            for (int b_id : item.getItemsBelow()) {
                if (a_id == b_id) continue;
                Item bBelow = instance.getItems().get(b_id);

                // If item b lies in the same level of item a
                if (bBelow.getMax().getZ() >= aBelow.getMax().getZ() && bBelow.getMin().getZ() < aBelow.getMax().getZ()) {
                    minX = min(minX, bBelow.getMin().getX());
                    maxX = max(maxX, bBelow.getMax().getX());
                    minY = min(minY, bBelow.getMin().getY());
                    maxY = max(maxY, bBelow.getMax().getY());
                }
            }
            if (x <= minX || x >= maxX || y <= minY || y >= maxY) {
                if (msg) System.err.println("Item " + item.getId() + " not enough supported.");
                return false;
            }
        }
        return true;
    }

    /**
     * Check item position w.r.t. LBS Simple Constraint.
     * The Load of an item is distributed to all indirect and direct
     * supporting items underneath the item base area.
     *
     * @param item     the item
     * @param instance the instance
     * @param msg      optional output message
     * @return feasibility of item position.
     */
    public static boolean checkLBSSimple(Item item, Instance instance, final boolean msg) {
        for (int below_id : item.getItemsBelow()) {
            Item below = instance.getItems().get(below_id);

            // Call Calc_LBS with mass of above item
            if (!checkLBS(below, item, item.getMass(), instance, Stacking.LBSSimple, msg)) {
                if (msg) System.err.println("Load exceeded for item " + below.getId());
                return false;
            }
        }

        return true;
    }

    /**
     * Check item position w.r.t. LBS Complete Constraint.
     * The load of an item is recursively distributed to the direct underlying items.
     *
     * @param item     the item
     * @param instance the instance
     * @param load     the load
     * @param msg      optional output message
     * @return feasibility of item position.
     */
    public static boolean checkLBSComplete(Item item, Instance instance, double load, final boolean msg) {
        for (int below_id : item.getItemsBelow()) {
            Item below = instance.getItems().get(below_id);

            // Consider items with direct support
            if (below.getMax().getZ() != item.getMin().getZ()) continue;

            // Call CalcLBS with adapted load
            if (!checkLBS(below, item, load, instance, Stacking.LBSComplete, msg)) {
                if (msg) System.err.println("Load exceeded for item " + below.getId());
                return false;
            }
        }
        return true;
    }

    /**
     * Check item position w.r.t. LBS (Complete or Simple) Constraint.
     *
     * @param below    the item below
     * @param above    the item above
     * @param weight   the weight to be distributed
     * @param instance the instance
     * @param stacking the stacking constraint (LBS)
     * @param msg      optional output message
     * @return feasibility of item position.
     */
    public static boolean checkLBS(Item below, Item above, final double weight, final Instance instance, final Stacking stacking, final boolean msg) {
        if (above.getSupport() == 0) {
            above.setSupport(calcSupport(above, instance));
        }

        // Get Support Area between above Item and below Item
        int minY = max(above.getMin().getY(), below.getMin().getY()) - below.getMin().getY();
        int minX = max(above.getMin().getX(), below.getMin().getX()) - below.getMin().getX();
        int maxY = min(above.getMax().getY(), below.getMax().getY()) - below.getMin().getY();
        int maxX = min(above.getMax().getX(), below.getMax().getX()) - below.getMin().getX();
        int supportArea = (maxY - minY) * (maxX - minX);

        // Calculate part of support by above Item
        double support_below = (double) supportArea / ((double)above.getL() * above.getW());
        support_below /= above.getSupport();

        // Calculate part of load
        double load_part   = weight * support_below;
        double loadPerArea = load_part / (double) supportArea;

        // Check if Load per Area Unit exceeds in general the limit
        if (loadPerArea > below.getLbs()) return false;

        // Save total load
        below.setSumLoad(below.getSumLoad() + load_part);

        // Check Load per Area Unit
        for (int a = minX; a < maxX; ++a) { // Row
            for (int b = minY; b < maxY; ++b) { // Column
                // Determine Square
                int square = b + (below.getMax().getY() - below.getMin().getY()) * a;

                // Save Load for the Square
                double load = below.getLoad().get(square);
                load += loadPerArea;
                below.getLoad().set(square, load);

                // Check Load
                if (load > below.getLbs())
                    return false;
            }
        }

        // Recursive Call
        if (stacking == Stacking.LBSComplete) {
            return checkLBSComplete(below, instance, load_part, msg);
        }
        return true;
    }

    /**
     * Calculate the support area units between two items.
     *
     * @param current the current item
     * @param other   the other item
     * @return the supporting area units
     */
    public static int calcSupportArea(final Item current, final Item other) {
        int minX = max(current.getMin().getX(), other.getMin().getX());
        int minY = max(current.getMin().getY(), other.getMin().getY());
        int maxX = min(current.getMax().getX(), other.getMax().getX());
        int maxY = min(current.getMax().getY(), other.getMax().getY());
        int area = (maxX - minX) * (maxY - minY);

        return max(area, 0);
    }

    /**
     * Calculate the total support of an item.
     *
     * @param item     the item
     * @param instance the instance
     * @return the total support.
     */
    public static double calcSupport(Item item, final Instance instance) {
        // On Bottom
        double support;
        if (item.getMin().getZ() == 0) {
            support = 1.0;
        }
        else {
            // Calculate Support
            double supportArea = 0;
            for (int item_id : item.getItemsBelow()) {
                Item other = instance.getItems().get(item_id);
                if (other.isBelow(item, true)) {
                    supportArea += calcSupportArea(item, other);
                }
            }
            support = supportArea / ((double)item.getL() * item.getW());
        }
        item.setSupport(support);
        return support;
    }

    /**
     * Check item position w.r.t. Reachability Constraint.
     *
     * @param item         the item
     * @param tour         the tour
     * @param lambda       the minimum dimension units
     * @param reachability the consideration of reachability constraint
     * @param instance     the instance
     * @param msg          optional output message
     * @return feasibility of item position.
     */
    public static boolean checkReachability(Item item, Tour tour, final int lambda, final boolean reachability, Instance instance, final boolean msg) {
        if (reachability) {
            // Determine max X
            int maxX = item.getMax().getX();
            for (int item_id : tour.getItem_ids()) {
                Item other = instance.getItems().get(item_id);

                // Get Positions of Customers in Tour
                int posCurrentCustomer = tour.getCustomer_ids().indexOf(item.getCustomer_id());
                int posOtherCustomer   = tour.getCustomer_ids().indexOf(other.getCustomer_id());

                if (posOtherCustomer > posCurrentCustomer
                        && item.getMax().getY() > other.getMin().getY() && item.getMin().getY() < other.getMax().getY()) {
                    maxX = max(maxX, other.getMax().getX());
                }
            }
            // Check Reachability
            if (maxX - item.getMax().getX() > lambda) {
                if (msg) System.err.println("Not reachable Item " + item.getId());
                return false;
            }
        }
        return true;
    }


    /**
     * Fills relevant sets of items.
     * For each item, add all indirect supporting items,
     * add all items above,
     * add all items in front and
     * add all items behind.
     *
     * @param tour       the tour
     * @param tourEndPos the current position of the packing process
     * @param instance   the instance
     */
    public static void getRelevantItems(Tour tour, int tourEndPos, Instance instance) {
        for (int i = 0; i < tourEndPos; ++i) {
            final int currentId = tour.getItem_ids().get(i);
            for (int j = 0; j < tourEndPos; ++j) {
                if (i == j) continue;
                final int otherId = tour.getItem_ids().get(j);
                Item item  = instance.getItems().get(currentId);
                Item other = instance.getItems().get(otherId);
                // Get direct supporting items
                if (other.isBelow(item, true)) {
                    item.getItemsBelow().add(otherId);
                    other.getItemsAbove().add(item.getId());
                }
                if (other.isBehind(item)) {
                    item.getItemsBehind().add(other.getId());
                    other.getItemsFront().add(item.getId());
                }
            }
        }

        for (int i = 0; i < tourEndPos; ++i) {
            final int currentId = tour.getItem_ids().get(i);
            Item current = instance.getItems().get(currentId);
            Iterator<Integer> iter = current.getItemsBelow().iterator();
            while (iter.hasNext()) {
                int directId = iter.next();
                Item direct = instance.getItems().get(directId);
                for (int indirectId : direct.getItemsBelow()) {
                    Item indirect = instance.getItems().get(indirectId);
                    if (indirect.isBelow(current, false)) {
                        if (!current.getItemsBelow().contains(indirectId)) {
                            current.getItemsBelow().add(indirectId);
                            indirect.getItemsAbove().add(currentId);
                            iter = current.getItemsBelow().iterator();
                        }
                    }
                }
            }
        }
    }


    /**
     * Gets the scale factor for Reachability Constraint
     * to rescale the lambda factor.
     *
     * @param vehicleH the vehicle height
     * @return the scale factor
     */
    public static int  getScaleFactor(final int vehicleH) {
        int digits = String.valueOf(vehicleH).length();
        return (int) pow(10, digits - 2);
    }
}
