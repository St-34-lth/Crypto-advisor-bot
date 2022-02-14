#include "TradeInterface.h"



#include <cctype>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include <list>

#include <regex>
#pragma once


//* an enum class to be used for the available commands of the bot *//
enum class Commands  
{
          help,
          avg,
          predict,
          prod,
          min,
          max,
          marketTime,
          step,
          frl,
          invalid,
          Default= invalid
};


class TestBot
{
          
                   
         
          private: 
                              //* data members*//
                    TradeInterface& tradingInterface;
                    string username;
                    string currentTime;
                    string firstTime;
                    string latestTime;
                    string logFilepath;

                    map<string, vector<OrderBookEntry>> currentMarketEntries;
                    struct results
                    {
                                         /* data */
                              double number;
                              string text;
                              map<string, vector<OrderBookEntry>> totalBidEntries;
                              map<string, vector<OrderBookEntry>> totalAskEntries;
                              vector<double> yPoints;
                              vector<int> xPoints;
                              
                    };
                    
                                        //* available bot user commands * //
                    results avg(queue<string> &arguments);
                    results min(queue<string> &arguments);
                    results help(queue<Commands> &commands);
                    results max(queue<string> &arguments);
                    results prod();
                    results predict(queue<string> &arguments);
                    void time();
                    void step();
                    results frl(queue<string> &arguments);
                    

                                        //* command parser functions
                    void outputResult(TestBot::results &result, Commands &callerCmd);
                    string getUserCmd();
                    Commands stringToCommand(const string &str);
                    void parseCommands(queue<Commands> &commands, queue<string> &arguments);
                    void clearQueues(queue<Commands> &commands, queue<string> &arguments);

                    
                                        // * helper functions *//
                  
                    void unpack(map<string, vector<OrderBookEntry>> marketStats);
                    string retrieveAndPop(queue<string> &arguments);
                    bool validPeak(const string& peak);
                    bool validType(const string& orderType);
                    bool validProduct(const string& product);

                                        // *tradeit API handlers *//

                    void jumpToTimeFrame(int jumps);
                    vector<OrderBookEntry> getBids(string product, string timestamp);
                    vector<OrderBookEntry> getAsks(string product, string timestamp);
                    void getCurrentTime();
                    results getCurrentMarketStats();
                    string getTime();

          public :
                    TestBot(TradeInterface& _tradingInterface);
                    void init(string _username);
                    void run();
                    
          
                   
};
