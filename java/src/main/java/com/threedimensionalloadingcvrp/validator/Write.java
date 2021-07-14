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
     * Write a packing plan to file.
     *
     * @param path         the path of the output file.
     * @param solution     the solution
     * @param instance     the instance
     * @param constraintNo the constraint number (e.g. P1)
     * @param seed         the seed, the test number
     * @throws IOException the exception
     */
    public static void WritePackingPlan(final String path, final Solution solution, final Instance instance, final int constraintNo, final int seed) throws IOException {
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
            int noItems = 0;
            for (int customerId : tour.getCustomer_ids()) {
                Customer customer = instance.getCustomers().get(customerId);
                noItems += customer.getItem_ids().size();
            }
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
                    int rotationFlag = item.isRotated() ? 1 : 0;
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
