# CI Status
[![Java CI with Maven](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/maven.yml/badge.svg)](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/maven.yml)
[![CPP CI with MSBuild](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/msbuild.yml/badge.svg)](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/msbuild.yml)

# License
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

# Creators
Corinna Krebs

# Introduction
This solution validator checks the feasibility of a given solution with respect to various loading and routing constraints (see Code Documentation or Literature).
It handles the combination of two optimization problems — the vehicle routing problem (VRP) combined with the container loading (CLP), also known as the 2L-CVRP, 2L-VRPTW, 3L-CVRP and 3L-VRPTW.

# Requirements
The tool is implemented in C++ and in Java. Therefore, it can be used with either of the two languages.
Only standard libraries are used.

## C++ Version
* Language level: min. C++11
* Software: e.g. MSBuild via [Visual Studio](https://visualstudio.microsoft.com/de/)
* Build files: [validator.sln](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Validator.sln), [validator.vcxproj](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Validator.vcxproj)

## Java Version
* Language level: min. Java 11
* Software: Java JRE, [Apache Maven Version 2.0](https://maven.apache.org/), IDE (e.g. [IntelliJ](https://www.jetbrains.com/de-de/idea/) or [Eclipse](https://www.eclipse.org/downloads/))
* Build files: [pom.xml](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/pom.xml)

# Usage
* Examples: 
  * C++: [Main.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Main.cpp)
  * Java: [Main.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/Main.java)

## Code Example

```Java
// Necessary Data
Instance instance           = Read.readInstanceFile("../Input/Instances/Krebs_Ehmke_Koch_2020/001_n020_m200_bt3.txt");
ConstraintSet constraintSet = Read.readConstraintFile("../Input/Constraint_Sets/P1.txt");
Solution solution           = Read.readSolutionFile("../Input/PackPlan/001_n020_m200_bt3_P1_1.txt", instance);

// Check Constraints
boolean feasibleRouting = checkRoutingConstraints(solution, constraintSet, instance);
boolean feasibleLoading = checkLoadingConstraints(solution, constraintSet, instance);
```

### Input Data: 
Provide necessary data via Instance, ConstraintSet and Solution objects.

### Check Methods: 
* Input Data is provided as parameters to the methods. 
* Return Value: Feasibility of the solution (feasible: true; infeasible: false). 
* Check all Constraints defined in ConstraintSet: Use checkRoutingConstraints and checkLoadingConstraints
* Check specific Constraint: 
    * C++: [ConstraintsLoading.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/ConstraintsLoading.cpp) and [ConstraintsRouting.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/ConstraintsRouting.cpp)
    * Java: [Loading.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/constraints/Loading.java) and [Routing.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/constraints/Routing.java)

## Input Data

### Instance
* Data: Information about vehicles, customers, items and demands
* Usage of Converted Instances from Literature (see [this repository](https://github.com/CorinnaKrebs/Instances))
* Usage of Instance Class Constructor 

### Constraint Set
* Data: Included Loading Constraints and Parameters
* Read Constraint File ([see examples](https://github.com/CorinnaKrebs/SolutionValidator/tree/master/Input/Constraint_Sets))
* Adapt Example and Read new File
* Usage of ConstraintSet Class Constructor

### Solution
* Data: Objective Values, run time, number of iterations, set of routes and packing plans
* Usage of Solution file 
* Usage of Solution Constructor 
* Convert Data via Write Class (see C++: [Write.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Write.cpp) or Java: [Write.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/Write.java))

# Literature
Loading Constraints and formulas are described in

* Advanced Loading Constraints for 3D Vehicle Routing Problems <br>
by C. Krebs, J. Ehmke, H. Koch

* Axle Weights in Combined Vehicle Routing and Container Loading Problems <br>
by C. Krebs, J. Ehmke

* Vertical Stability Constraints in Combined Vehicle Routing and 3D Container Loading Problem <br>
by C. Krebs, J. Ehmke



