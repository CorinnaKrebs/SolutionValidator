package com.threedimensionalloadingcvrp.validator;

import com.threedimensionalloadingcvrp.validator.model.*;

import static com.threedimensionalloadingcvrp.validator.constraints.Loading.checkLoadingConstraints;
import static com.threedimensionalloadingcvrp.validator.constraints.Routing.checkRoutingConstraints;


public class Main {
    public static void main(String[] args) {
        Instance instance = Read.readInstanceFile("../Input/Instances/Krebs_Ehmke_Koch_2020/001_n020_m200_bt3.txt");
        ConstraintSet constraintSet = Read.readConstraintFile("../Input/Constraint_Sets/P1.txt");
        Solution solution = Read.readSolutionFile("../Input/PackPlan/001_n020_m200_bt3_P1_1.txt", instance);
        if (checkRoutingConstraints(solution, constraintSet, instance) && checkLoadingConstraints(solution, constraintSet, instance)) {
            System.out.println("All Constraints checked. Solution is feasible.");
            System.exit(1);
        }
        System.err.println("Solution is not feasible. Please check error hints above.");
        System.exit(-1);
    }
}
