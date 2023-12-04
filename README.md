# HomeCare
## Introduction
Welcome to the documentation for the HomeCare application. This C++ application is designed to address the needs related to planning and optimizing travel for doctors and nurses in the context of home care.

## System Requirements
- C++ compiler compatible with C++11 standard or later.
- Operating system compatible with the C++ standard.

# Installation
Clone the Repository:

``` bash
git clone https://github.com/ffracas/HomeCare
cd HomeCare
```

Compile the Code:
    
``` 
cd src
g++ -std=c++11 main.cpp ../include/problemdef/problemdef.cpp ../include/params/params.cpp ../include/route/route.cpp ../include/arc/arc.cpp ../include/node/node.cpp -o HomeCare
```

Another option is:
``` 
cd src
make
```

Run the Application:

```
./HomeCare
```
