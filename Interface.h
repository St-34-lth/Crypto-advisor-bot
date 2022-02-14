#include "tradeit-io.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#pragma once

//An instance of the trade it app placed here so tha Trade interface will to access it in its scope.
//* A virtual class to provide and demonstrate composition for the Trade Interface class. *//
//* The Trade Interface inherits the below virtual methods, which ensures that they must be implemented. *//
// * On a sidenote, it may be used to inherit from the trade it class and grant access to both its own and tradeit's class methods but I chose not to do it for practicing cpp syntax etc ! *//
class Interface
{
          public:
                   
                    virtual ~Interface() = default;  
                    virtual vector<OrderBookEntry> getAsks(string product,string timestamp) = 0;
                    virtual vector<OrderBookEntry> getBids(string product,string timestamp) = 0; 
                    virtual string getCurrentTime() = 0;
                    virtual void init() = 0;
};