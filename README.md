[![Java CI with Maven](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/maven.yml/badge.svg)](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/maven.yml)
[![CPP CI with MSBuild](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/msbuild.yml/badge.svg)](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/msbuild.yml)

# License
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

# Creators
Corinna Krebs

# Introduction
This solution validator checks the feasibility of a given solution with respect to various loading and routing constraints (see Section Constraints).
It handles the combination of two optimization problems -- the vehicle routing problem (VRP) combined with the container loading (CLP), also known as the 2L-CVRP, 2L-VRPTW, 3L-CVRP and 3L-VRPTW.

# Requirements
The tool is implemented in C++ and in Java. Therefore, it can be used with either of the two languages.
Only standard libraries are used.

## C++ Version
* Language level: min. C++11
* Software: e.g. MSBuild via [Visual Studio](https://visualstudio.microsoft.com/de/)
* Build files: [validator.sln](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Validator.sln), [validator.vcxproj](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Validator.vcxproj)

## Java Version
* Language level: min. Java 11
* Software: Java JRE, [Apache Maven Version 2.0](https://maven.apache.org/), IDE (e.g. [IntelliJ](https://www.jetbrains.com/de-de/idea/) or [Eclipse](https://www.eclipse.org/downloads/)
* Build files: [pom.xml](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/pom.xml)

# Usage
* Examples: C++: [Main.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Main.cpp), Java: [Main.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/Main.java)

## Code Example
To run the code, necessary data must be given. This includes three class objects: Instance, ConstraintSet and Solution.
These are then used to call the checkConstraints methods. The return values indicate whether the solution is feasible (true) or not (false).

```Java
// Necessary Data
Instance instance           = Read.readInstanceFile("../Input/Instances/Krebs_Ehmke_Koch_2020/001_n020_m200_bt3.txt");
ConstraintSet constraintSet = Read.readConstraintFile("../Input/Constraint_Sets/P1.txt");
Solution solution           = Read.readSolutionFile("../Input/PackPlan/001_n020_m200_bt3_P1_1.txt", instance);

// Check Constraints
bool feasibleRouting = checkRoutingConstraints(solution, constraintSet, instance);
bool feasibleLoading = checkLoadingConstraints(solution, constraintSet, instance);
```

The following shows how to provide the three necessary objects:

## Instance
Contains data concerning the vehicles, customers, items and demands.

### Option 1 -- Converted Instance:
Some Instances from Literature are already converted to the required data format (see [this repository](https://github.com/CorinnaKrebs/Instances)).

### Option 2 -- Use Constructor:
Alternativly, use the class constructor to provide the necessary input data.

## Constraint Set
Sets the included constraints and its parameters.

### Option 1 -- Constraint Set File:
Some constraint sets are already available (see [here](https://github.com/CorinnaKrebs/SolutionValidator/tree/master/Input/Constraint_Sets))
or create a new file. Please, be aware of the order of the constraints and the given values.

### Option 2 -- Use Class Constructor:
Alternativly, use the constructor in Class ConstraintSet to provide the necessary input data.

## Solution
Contains data concerning the included constraints and its parameters.

### Option 1 -- Solution File:
Either read an existing solution file or create the file by using the Write Class (see C++: [Write.cpp](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/cpp/Validator/Write.cpp) or Java: [Write.java](https://github.com/CorinnaKrebs/SolutionValidator/blob/master/java/src/main/java/com/threedimensionalloadingcvrp/validator/Write.java)

### Option 2 -- Use Class Constructor:
Alternativly, use the constructor in Class Solution to provide the necessary input data.

# Literature
Loading Constraints and formulas are described in

* Advanced Loading Constraints for 3D Vehicle Routing Problems <br>
by C. Krebs, J. Ehmke, H. Koch

* Axle Weights in Combined Vehicle Routing and Container Loading Problems <br>
by C. Krebs, J. Ehmke

* Vertical Stability Constraints in Combined Vehicle Routing and 3D Container Loading Problem <br>
by C. Krebs, J. Ehmke



