#include "CSVReader.h"
#include "OrderBookEntry.h"
#include <iostream>
#include <chrono>
#include <array>


CSVReader::CSVReader()
{

}

vector<OrderBookEntry> CSVReader::readCSV(string csvFilename)
{
          vector<OrderBookEntry> entries;
          ifstream csvFile {csvFilename};
          string line;
          
          if (csvFile.is_open())
          {
                    cout << "CSV opened" << endl;
                    // left this to measure performance between versions.
                    const chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();

                    while(getline(csvFile,line))
                    {

                             
                              try
                              {

                                        entries.push_back(stringsToOBE(CSVReader::tokenize(line, ',')));
                              }
                              catch(const exception& err)
                              { 
                                        
                                        continue;
                                        
                              };
                              
                    };

                    const chrono::time_point<chrono::system_clock> end  = chrono::system_clock::now();

                    
                    cout << " Completed in  " << chrono::duration_cast<chrono::microseconds>(end - now).count() << " microseconds.\n";
          }
          else
          {
                    cout << "Problem with file" << csvFilename << endl;
          };
         
          csvFile.close();
          std::cout << "CSVReader::readCSV read " << entries.size() << " entries." << '\n';
          return entries;
};

vector<string> CSVReader::tokenize(const string& str, const char &delim)
{

          //*Original version *//
          // vector<string> tokens;
          // int start, end;
          
          // start = str.find_first_not_of(delim);
          // string token;
          // do
          // {
                    
          //           end = str.find_first_of(delim, start);
          //           if (start == str.length() || start == end)
          //           {
          //                     break;
          //           };
                  
          //           if (end >= 0)
          //           {
          //                     token = str.substr(start, end - start);
          //           }
          //           else
          //           {
          //                     token = str.substr(start, str.length() - start);
          //           };
                    
                    
          //           tokens.push_back(token);
                    
                    
          //           start = end + 1;

          // } while (end > 0);
          // return tokens;

          // own version
          // you can see the optimizations here compared to above
          vector<string> tokens;
          const int numOftokens = 4;
          int next = 0;
          int prev;
          string token;
          try
          {
                    for (int i = 0; i <= numOftokens; ++i) //We already know the number of tokens so why not use it ?
                    {
                              prev = next;
                              next = i == 0 ? str.find(delim, next) : str.find(delim, next + 1);
                              
                              token = i == 0 ? str.substr(prev, next - prev) : str.substr(prev + 1, next - 1 - prev);
                              
                              tokens.push_back(token);
                    }
          }
          catch (const exception &e)
          {
                    cout << e.what() << endl;
          };
          return tokens;
};
          
OrderBookEntry CSVReader::stringsToOBE(const std::vector<std::string> tokens)
{
          double price, amount;

          if (tokens.size() != 5) 
          {
                    std::cout << "Bad line " << std::endl;
                    throw std::exception{};
          }
          
          try
          {
                    price = std::stod(tokens[3]);
                    amount = std::stod(tokens[4]);
          }
          catch (const std::exception &e)
          {
                    std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
                    std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
                    throw;
          }

          OrderBookEntry obe{price,
                             amount,
                             tokens[0],
                             tokens[1],
                             OrderBookEntry::stringToOrderBookType(tokens[2])};

          return obe;
};

// you can see the optimizations here
OrderBookEntry CSVReader::stringsToOBE(const std::string& priceString,
                                       const std::string& amountString,
                                       const std::string& timestamp,
                                       const std::string& product,
                                       const OrderBookType& orderType)
{
          double price, amount;
          try
          {
                    price = std::stod(priceString);
                    amount = std::stod(amountString);
          }
          catch (const std::exception &e)
          {
                    std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
                    std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
                    throw;
          }
          OrderBookEntry obe{price,
                             amount,
                             timestamp,
                             product,
                             orderType};

          return obe;
}
