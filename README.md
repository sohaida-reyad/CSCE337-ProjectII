# CSCE337-ProjectII

Our Static Timing Analyzer is an application that accepts a gate-level netlist (GLN), 
a library file (liberty), net capacitance file, and timing constraints file, then it 
parses those files into internal data structures that will be integrated to perform 
analysis. The gate-level netlist is parsed into a directed acyclic graph (DAG) from 
which the different timing paths of the given logic will be deduced. Each path is 
then analyzed to obtained the path’s arrival time, required time, and slack, which 
all will be saved in a report file.

This project does the following:

1- Read the verilog gate level netlist file, net capacitance file, and timing constraints file. 
2- Read and Parse Liberty file
3- Convert gate level netlist to DAG
4- Identify all 4 types of timing paths. 
5- Perform STA by calculating arrival time and required times and calculating the slacks
6- Generate timing reports
