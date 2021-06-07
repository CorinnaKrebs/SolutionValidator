# CI Status
[![Java CI with Maven](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/maven.yml/badge.svg)](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/maven.yml)
[![CPP CI with MSBuild](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/msbuild.yml/badge.svg)](https://github.com/CorinnaKrebs/SolutionValidator/actions/workflows/msbuild.yml)

# Creators
Corinna Krebs

# Introduction
This solution validator checks the feasibility of a given solution with respect to various loading and routing constraints.
As optimization problems, the combination of the vehicle routing problem with container loading are possible (2L-CVRP, 2L-VRPTW, 3L-CVRP and 3L-VRPTW).

# Requirements
The tool is implemented in C++ and in Java. Therefore, it can be used with either of the two languages.
Only standard libraries are used.

## C++ version
Build files: visual studio files (validator.sln and validator.vcxproj)

## Java Version
Build files: pom.xml
Therefore, Apache Maven Version 2.0 is required.

# Usage
Loading Constraints and formulas are described in

* Advanced Loading Constraints for 3D Vehicle Routing Problems <br>
by C. Krebs, J. Ehmke, H. Koch

* Axle Weights in Combined Vehicle Routing and Container Loading Problems <br>
by C. Krebs, J. Ehmke



