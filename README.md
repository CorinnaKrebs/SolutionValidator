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
* Software: Java JRE, [Apache Maven Version 2.0](https://maven.apache.org/), IDE (e.g. [IntelliJ](https://www.jetbrains.com/de-de/idea/) or [Eclipse](https://www.eclipse.org/downloads/)), Lombok
* Build files: [pom.xml](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/pom.xml)

# Usage
* Examples: 
  * C++: [Main.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Main.cpp)
  * Java: [Main.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/Main.java)

## Code Example
In general, the program consists of two main parts: 
* Provide Data (Instance, ConstraintSet and Solution)
* Use Check Methods

General hints are given below. For the concrete usage of the methods, please check the documentation of the code.

```Java
// Necessary Data
Instance instance           = Read.readInstanceFile("../Input/Instances/Krebs_Ehmke_Koch_2020/001_n020_m200_bt3.txt");
ConstraintSet constraintSet = Read.readConstraintFile("../Input/Constraint_Sets/P1.txt");
Solution solution           = Read.readSolutionFile("../Input/PackPlan/001_n020_m200_bt3_P1_1.txt", instance);

// Check Constraints
boolean feasibleRouting = checkRoutingConstraints(solution, constraintSet, instance);
boolean feasibleLoading = checkLoadingConstraints(solution, constraintSet, instance);
```


## Input Data
Provide necessary data via Instance, ConstraintSet and Solution objects.

### Instance
* Necessary Data: Information about vehicles, customers, items and demands
* Provide Data:
  * use converted Instances from Literature (see [this repository](https://github.com/CorinnaKrebs/Instances)) or
  * use Instance Class Constructor (see C++: [Instance.h](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Instance.h) or Java: [Instance.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/model/Instance.java)).

### Constraint Set
* Necessary Data: Included Loading Constraints and Parameters
* Provide Data: 
  * read Constraint File ([see examples](https://github.com/CorinnaKrebs/SolutionValidator/tree/master/Input/Constraint_Sets)) or
  * adapt an Example and read this new File or
  * use ConstraintSet Class Constructor (see C++: [ConstraintSet.h](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/ConstraintSet.h) or Java: [ConstraintSet.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/model/ConstraintSet.java)).

### Solution
* Necessary Data: Objective Values, run time, number of iterations, set of routes and packing plans
* Provide Data:
  * use Solution file (see [our repository](https://github.com/CorinnaKrebs/Results)) or
  * use Solution Class Constructor (see C++: [Solution.h](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Solution.h) or Java: [Solution.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/model/Solution.java)) or
  * convert Data via Write Class (see C++: [Write.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Write.cpp) or Java: [Write.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/Write.java)) and use the new file.

## Check Methods: 
* Necessary Data: Input Data is shown above. Provided as parameters to the methods. 
* Return Value: Feasibility of the solution (feasible: true; infeasible: false). 
* Check all Constraints defined in ConstraintSet: Use checkRoutingConstraints and checkLoadingConstraints
* Check specific Constraint, see 
    * C++: [ConstraintsLoading.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/ConstraintsLoading.cpp) and [ConstraintsRouting.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/ConstraintsRouting.cpp)
    * Java: [Loading.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/constraints/Loading.java) and [Routing.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/constraints/Routing.java).

# Literature
Loading Constraints and formulas are described in

* Advanced Loading Constraints for 3D Vehicle Routing Problems <br>
by C. Krebs, J. Ehmke, H. Koch

* Axle Weights in Combined Vehicle Routing and Container Loading Problems <br>
by C. Krebs, J. Ehmke

* Vertical Stability Constraints in Combined Vehicle Routing and 3D Container Loading Problem <br>
by C. Krebs, J. Ehmke
