#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#pragma once
#include "OrderBookEntry.h"
using namespace std;


class CSVReader
{
          
          public:
                    CSVReader(); // default constructor declared
                    
                    static vector<OrderBookEntry> readCSV(string csvFilename);

                    static vector<string> tokenize(const string& line,const char& separator);

                    static OrderBookEntry stringsToOBE(
                        const std::string &priceString,
                        const std::string &amountString,
                        const std::string &timestamp,
                        const std::string &product,
                        const OrderBookType &orderType);

          private :

                    static OrderBookEntry stringsToOBE(const vector<string> tokens);
                    
                    string record;
                    char separator; 

};



