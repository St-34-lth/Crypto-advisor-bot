#include "Interface.h"
#include "tradeit-io.h"
#pragma once

//* An API for the tradeit simulator *//
//* Provides an interface for the simulator's functionalities to the bot. *//
// * Inherits from the virtual class Interface to demonstrate coding against an interface and improve modularity of the bot. *//
class TradeInterface: public Interface 
{
          
          private:
                    TradeIt& instance;

          public:
                    TradeInterface(TradeIt& _app)
                    : instance(_app){};

                    void init()
                    {
                              instance.init();
                    }
                    vector<OrderBookEntry> getAsks(string product, string timestamp)
                    {

                              return instance.getAsks(product, timestamp);
                    };
                    vector<OrderBookEntry> getBids(string product, string timestamp)
                    {
                              return instance.getBids(product, timestamp);
                    };
                    string getCurrentTime()
                    {

                              return instance.getCurrentTime();
                    };
                    string getEarliestTime()
                    {
                              return instance.getEarliestTime();
                    };
                    string getLatestTime()
                    {
                              return instance.getLatestTime();
                    };
                    vector<map<string, vector<OrderBookEntry>>> getCurrentMarketStats()
                    {
                              return instance.getCurrentMarketStats();
                    };
                    vector<string> getKnownProducts()
                    {
                              return instance.getKnownProducts();
                    };
                    void jumpToTimeFrame(int jumps)
                    {
                              instance.jumpToTimeFrame(jumps);
                    };
                    int getNumberOfJumpsMade()
                    {
                              return instance.getNumberOfJumpsMade();
                    };
                    void nextTimeStep()
                    {
                              instance.gotoNextTimeFrame();
                    };
                    double averageFor(int prevSteps, string type, string product)
                    {
                              return instance.averageFor(prevSteps, type, product);
                    };
                    double currentLow(string product, string type, string timestamp)
                    {
                              return instance.currentLow(product, type, timestamp);
                    };
                    double currentHigh(string product, string type, string timestamp)
                    {
                              return instance.currentHigh(product, type, timestamp);
                    };
};