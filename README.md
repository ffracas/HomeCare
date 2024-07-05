# HomeCare
## Introduction
The aim of the application is to solve the Home Health Care Routing and Scheduling Problem (HHCRSP) in everyday life.
Optimising the routing and scheduling of doctors and nurses in the context of home care is an important goal of today's health care systems, as home care is becoming a crucial part of the health care system due to the positive implications on patients' quality of life.
The HHCRSP is a complex, mathematically proven np-hard problem that requires the use of a variety of heuristics and meta-heuristics to find a solution.
The meta-heuristic used is ALNS (Adaptive Large Neighbourhood Search), a popular algorithm used in VRP that is very similar to this HHCRSP.
The operation implemented in ALNS are:
- Random Removal
- Related Removal
- Worst Removal
- Cluster Removal
- Greedy Repair
- Regret Repair
The heuristic algorithms used in ALNS are:
- Simulated annealing
- 2-otp
- 3-otp
- dinamic programming
The application core is a C++17 program.

## System Requirements
- **C++ compiler** compatible with C++17 standard or later.
- **Operating system** compatible with the C++ standard.
- **Library jsoncpp** ([GitHub Repository](https://github.com/open-source-parsers/jsoncpp?tab=readme-ov-file)) installed

# Installation
Follow these steps to install and use HomeCare:

1. Clone the Repository:

``` bash
git clone https://github.com/ffracas/HomeCare
cd HomeCare
```

2. Compile the Code to find a solution:
    
``` bash
cd src
make -B
```

Run the Application with default input file:

```bash
./HomeCare
```
You can also specify a JSON input file:
```bash
./HomeCare "<AbsoluteOrRelative_pathTo/InputFile.json>"
```

3. Compile the Code to validate a solution:

``` bash
cd src
make -B Validate
```
You can also run validation with a specific JSON input file and solution:
```bash
./Validate "<AbsoluteOrRelative_pathTo/InputFile.json>" "<AbsoluteOrRelative_pathTo/SolutionFile.json>"
```

