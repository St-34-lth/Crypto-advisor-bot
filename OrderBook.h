#include <iostream>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#pragma once

#include <vector>
#include <string>

class OrderBook 
{
          public: 
                    OrderBook(string filename);
                    
                    vector<string> getKnownProducts();
                    vector<OrderBookEntry> getOrders(OrderBookType type, string product, string timestamp);
                    void insertOrder(OrderBookEntry& order); 
                    static double getHighPrice(vector<OrderBookEntry>& orders);
                    static double getLowPrice(vector<OrderBookEntry>& orders);
                    string getEarliestTime();
                    string getNextTime(string timestmap);
                    vector<OrderBookEntry> matchAsksToBids(string product, string timestamp);
                              
                              
                              //* Own methods *//
                    
                    string getLatestTime();
                    string getPreviousTime(string timestamp);
                    int availableTimeframes(int timeframes, string timestamp);

          private:
                   
                    vector <OrderBookEntry> orders; 
};