# C-Boolean-Satisfiability-Problem-Solver

# Description:

This project uses several heuristic search techniques to find possibly optimal truth assignments for variables in Boolean formulas.
The formulas are in conjunctive normal form (ANDs of ORs). The fitness of an assignment is the number of clauses (ORs) that the assignment satisfies. 

# Running Instructions:

This program runs on Linux 4.4.0-34-generic x86_64 (DMB 005)

Compile: g++ main.cpp

All files must be in the same folder

The test folder "sat" must be in the same folder as the program

Run: ./a.out

To run on many instances: compile and run ./hw3

To test the hill-climbing algorithm, uncomment line 78

# Data Structure:

I am using a vector to represent the formula where each element is a vector of integers representing the clause, and each integer represents the variable index.

# DPLL:

If the formula has no clauses return true. If it has an empty clause, return false. If it contains a unit clause, remove clauses where the literal is positive, and remove the literal from the clauses where it’s negated. If there are no unit clauses, choose the literal that appears in most clauses, set it to true, call DPLL recursively, and if it yields false, set the chosen literal to false and call DPLL recursively.

# walkSAT:

Create an array to store the values of variables (0 or 1). Populate the array randomly.
Substitute the array values in the clauses and evaluate the formula. Store the unsatisfied clauses in a vector. 
As long as there are unsatisfied clauses do the following:
-	With probability p = 0.5:
o	Randomly select a bit and flip it.
-	With probability 1-p:
o	Select a vector element at random
o	Select the variable that appears in most clauses, and flip it
-	Substitute the array values in the clauses
-	Store the unsatisfied clauses in a vector

Note: I am not allowing picking the same clause twice in a row which sometimes slows the program down.


# Hill Climbing:

Choose a random assignment. Check if it satisfies the formula. If not, flip the bit with the highest fitness, that is the bit that appears in most clauses. If we reach a plateau, in which all the bits yield the same or a lower fit fitness, choose a new random assignment.

# Sample Output:

Satisfiable
runtime: 5 ms

No solution, Best assignment found: 0 0 0 1 0 0 1 0 1 0 
runtime: 214 ms

