# HomeCare
## Introduction
Welcome to the documentation for the HomeCare application. This C++ application is designed to address the needs related to planning and optimizing travel for doctors and nurses in the context of home care.


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

