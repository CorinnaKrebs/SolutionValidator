package com.threedimensionalloadingcvrp.validator;

import com.threedimensionalloadingcvrp.validator.model.*;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Locale;

/**
 * The Write Class.
 */
public class Write {

    /**
     * Write an instance to file.
     *
     * @param path         the path of the output file.
     * @param instance     the instance
     * @throws IOException the exception
     */
    public static void instanceFile(final String path, final Instance instance) throws IOException {
        FileWriter writer = new FileWriter(path + instance.getName() + ".txt");
        writer.write("Name\t\t\t\t" + instance.getName() + "\n");
        writer.write("Number_of_Customers\t\t" + (instance.getCustomers().size() - 1) + "\n");
        writer.write("Number_of_Items\t\t\t" + (instance.getItems().size() - 1) + "\n");
        writer.write("Number_of_ItemTypes\t\t" + (instance.getItemTypes().size() - 1) + "\n");
        writer.write("Number_of_Vehicles\t\t" + instance.getV_max() + "\n");
        writer.write("TimeWindows\t\t\t" + (instance.isTw() ? 1 : 0) + "\n" );

        writer.write("\n");

        final Vehicle vehicle = instance.getVehicle();
        writer.write("VEHICLE\n");
        writer.write("Mass_Capacity\t\t\t"   + String.format(Locale.ENGLISH, "%.0f", vehicle.getD()) + "\n");
        writer.write("CargoSpace_Length\t\t" + vehicle.getL() + "\n");
        writer.write("CargoSpace_Width\t\t"  + vehicle.getW() + "\n");
        writer.write("CargoSpace_Height\t\t" + vehicle.getH() + "\n");
        writer.write("Wheelbase\t\t\t"       + vehicle.getWheelbase() + "\n");
        writer.write("Max_Mass_FrontAxle\t\t" + String.format(Locale.ENGLISH, "%.0f", vehicle.getLim_frontAxle()) + "\n");
        writer.write("Max_Mass_RearAxle\t\t"  + String.format(Locale.ENGLISH, "%.0f", vehicle.getLim_rearAxle()) + "\n");
        writer.write("Distance_FrontAxle_CargoSpace\t" + vehicle.getL_FA_cargo() + "\n");

        writer.write("\n");

        writer.write("CUSTOMERS\n");
        writer.write("i\t\tx\t\ty\t\tDemand\t\tReadyTime\tDueDate\t\tServiceTime\tDemandedMass\tDemandedVolume\n");
        for (int i = 0; i < instance.getCustomers().size(); ++i) {
            final Customer customer = instance.getCustomers().get(i);
            writer.write(i + "\t\t");
            writer.write(String.format(Locale.ENGLISH, "%.0f", customer.getX()) + "\t\t");
            writer.write(String.format(Locale.ENGLISH, "%.0f", customer.getY()) + "\t\t");
            writer.write(customer.getDemand() + "\t\t");
            writer.write(customer.getReadyTime() + "\t\t");
            writer.write(customer.getDueDate() + "\t\t");
            writer.write(customer.getServiceTime() +"\t\t");
            writer.write(String.format(Locale.ENGLISH, "%.2f", customer.getDemandedMass()) + "\t\t");
            writer.write(String.format(Locale.ENGLISH, "%.0f", customer.getDemandedVolume()) + "\n");
        }

        writer.write("\n");

        writer.write("ITEMS\n");
        writer.write("Type\t\tLength\t\tWidth\t\tHeight\t\tMass\t\tFragility\t\tLoadBearingStrength\n");
        for (int i = 1; i < instance.getItemTypes().size(); ++i) {
            final ItemType type = instance.getItemTypes().get(i);
            writer.write("Bt" + i + "\t\t" + type.getL() + "\t\t" + type.getW() + "\t\t" + type.getH() + "\t\t");
            writer.write(String.format(Locale.ENGLISH, "%.2f", type.getMass()) + "\t\t");
            writer.write((type.isFragile() ? 1 : 0) + "\t\t\t");
            writer.write(String.format(Locale.ENGLISH, "%.6f", type.getLbs()) + "\n");
        }

        writer.write("\n");
        writer.write("DEMANDS PER CUSTOMER\n");
        writer.write("i\tType Quantity\n");
        for (int i = 1; i < instance.getCustomers().size(); ++i) {
            final Customer customer = instance.getCustomers().get(i);
            writer.write(i + "\t");
            for (int j = 0; j < customer.getDemands().size(); ++j) {
                final Demand demand = customer.getDemands().get(j);
                writer.write("Bt" + demand.getItemTypeId() + " " + demand.getQuantity() + "  ");
            }
            writer.write("\n");
        }
        writer.close();
    }

    /**
     * Write a constraint set to file.
     *
     * @param path              the path of the output file.
     * @param constraintSet     the constraint set
     * @param constraintNumber  the number of the constraint set
     * @throws IOException      the exception
     */
    public static void constraintFile(final String path, final ConstraintSet constraintSet, final int constraintNumber) throws IOException {
        FileWriter writer = new FileWriter(path + "P" + constraintNumber + ".txt");

        writer.write("// Parameter for the Constraints\n");
        writer.write("alpha\t\t"  + constraintSet.getAlpha() + "\t\t// Support Parameter\t\t\tin %\n");
        writer.write("lambda\t\t" + constraintSet.getLambda() + "\t\t// distance for reachability\t\tin dm\n");
        writer.write("balanced_part\t" + constraintSet.getBalanced_part() + "\t\t// Part of balanced loading\t\tin %\n");

        writer.write("\n");
        writer.write("// (De-)activation of Constraints\n");

        writer.write("rotation\t\t" + (constraintSet.isRotation() ? 1 : 0) + "\t// Rotation of Items\t\t(0: n, 1: y)\n");
        writer.write("capacity\t\t" + (constraintSet.isLoad_capacity() ? 1 : 0) + "\t// Load Capacity\t\t(0: n, 1: y)\n");
        writer.write("unloading_sequence\t" + constraintSet.getUSequence().ordinal() + "\t// Unloading Sequence\t\t(0: n; 1: lifo,\t2: mlifo)\n");
        writer.write("vertical_stability\t" + constraintSet.getVStability().ordinal() + "\t// Vertical Stability\t\t(0: n, 1: minimal support area,\t2: robust stability (multiple overhanging),\t3: robust stability 2 (top overhanging))\n");
        writer.write("stacking\t\t" + constraintSet.getStacking().ordinal() + "\t// Stacking Constraints\t\t(0: n, 1: fragility,\t2: LBS Simplified,\t3: LBS Complete)\n");
        writer.write("reachability\t\t" + (constraintSet.isReachability() ? 1 : 0) + "\t// Reachability\t\t\t(0: n, 1: y)\n");
        writer.write("axle_weights\t\t" + (constraintSet.isAxle_weights() ? 1 : 0) + "\t// Axle Weight Constraint\t(0: n, 1: y)\n");
        writer.write("balancing\t\t" + (constraintSet.isBalanced_loading() ? 1 : 0) + "\t// Balanced Loading\t\t(0: n, 1: y)\n");

        writer.close();
    }

    /**
     * Write a packing plan (aka solution) to file.
     *
     * @param path         the path of the output file.
     * @param solution     the solution
     * @param instance     the instance
     * @param constraintNo the constraint number (e.g. P1)
     * @param seed         the seed, the test number
     * @throws IOException the exception
     */
    public static void solutionFile(final String path, final Solution solution, final Instance instance, final int constraintNo, final int seed) throws IOException {
        FileWriter writer = new FileWriter(path + instance.getName() + "_P" + constraintNo + "_" + seed + ".txt");

        writer.write("Name:\t\t\t\t" + instance.getName() + "\n");
        String problem = instance.isTw() ? "3L-VRPTW" : "3L-CVRP";
        writer.write("Problem:\t\t\t" + problem + "\n");

        writer.write("Number_of_used_Vehicles:\t" + solution.getTours().size() + "\n");
        writer.write("Total_Travel_Distance:\t\t" + String.format(Locale.ENGLISH, "%.2f", solution.getTotal_travel_distance()) + "\n");
        writer.write("Calculation_Time:\t\t" + String.format(Locale.ENGLISH,"%.2f", solution.getCalculation_time()) + "\n");
        writer.write("Total_Iterations:\t\t" + solution.getTotal_iterations() + "\n");
        writer.write("ConstraintSet:\t\t\t" + "P" + constraintNo + "\n");

        writer.write("\n");

        String breakLine = "-------------------------------------------------------------------------------------------------------------------\n";

        int id = 1;
        for (final Tour tour : solution.getTours()) {
            int noItems = tour.getItem_ids().size();
            writer.write(breakLine);
            writer.write("Tour_Id:\t\t\t" + id++ + "\n");
            writer.write("No_of_Customers:\t\t" + tour.getCustomer_ids().size() + "\n");
            writer.write("No_of_Items:\t\t\t" + noItems + "\n");
            writer.write("Customer_Sequence:\t\t");
            for (int customerId : tour.getCustomer_ids()) {
                writer.write(customerId + " ");
            }
            writer.write( "\n\n");

            if (tour.getCustomer_ids().size() > 1) {
                writer.write("CustId\tId\tTypeId\tRotated\tx\ty\tz\tLength\tWidth\tHeight\tmass\tFragility\tLoadBearingStrength" + "\n");
                for (int itemId : tour.getItem_ids()) {
                    Item item = instance.getItems().get(itemId);
                    int fragFlag = item.isFragile() ? 1 : 0;
                    int rotationFlag = item.getRotation().ordinal();
                    String itemMass = (int) item.getMass() == item.getMass() ? String.format("%.0f", item.getMass()) : String.format(Locale.ENGLISH, "%.2f", item.getMass());

                    writer.write(item.getCustomer_id() + "\t" + itemId + "\t" + item.getType_id() + "\t" + rotationFlag + "\t" + item.getMin().getX() + "\t" + item.getMin().getY() + "\t" + item.getMin().getZ() + "\t" + item.getL() + "\t" + item.getW() + "\t" + item.getH() + "\t");
                    writer.write(itemMass + "\t" + fragFlag + "\t\t" + String.format(Locale.ENGLISH,"%.6f", item.getLbs()) + "\n");
                }
                writer.write("\n\n");
            }
        }
        writer.close();
    }
}
