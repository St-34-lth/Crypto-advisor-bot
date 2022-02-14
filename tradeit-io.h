#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "CSVReader.h"

#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <limits>
#include <numeric> 
#include <algorithm>

#pragma once

class TradeIt
{
          public:
                    
                    TradeIt();
                              //* Startup and main loop *//
                    void init();
                    void run();
                              //* user interaction */
                    void enterAsk(OrderBookEntry askOrder);
                    void enterBid(OrderBookEntry bidOrder);
                    vector<OrderBookEntry> getAsks(string product, string timestamp);
                    vector<OrderBookEntry> getBids(string product, string timestamp);
                    vector<map<string, vector<OrderBookEntry>>> getCurrentMarketStats();
                              //*helpers */
                    double currentHigh(string product, string type, string timestamp = "");
                    double currentLow(string product, string type, string timestamp = "") ;
                    void executeTrades(string product,string user);
                              //* wallet functionality *//
                    void createWallet(string const &username);
                    void fundWallet(string fundType, double amount, string username);
                    void showWallet();
                    void gotoNextTimeFrame();
                    string getWalletStats(string const &username);

                    
                    
                    //*own and modified methods*//
                    void jumpToTimeFrame(int jumps = 0);
                    int getNumberOfJumpsMade();
                    string getEarliestTime();
                    string getLatestTime();
                    vector<string> getKnownProducts();
                    double averageEach(string type, string product, string timestamp);
                    double averageFor(int steps, string type, string product);
                    string getCurrentTime();
                   
          private :

                    void printHelp();
                    void printMarketStats();
                    void enterAsk();
                    void enterBid();
                    void printMenu();
                    int getUserOption();
                    void processUserOption(int userOption);
                    void matchingEngine(vector<OrderBookEntry> asksList, vector<OrderBookEntry> bidsList);

                    map<string, Wallet> userWallets;
                    void addWallet(Wallet userWallet);
                    int jumpsMade;
                    Wallet exchangeWallet{};
                    string currentTime;
                    map<string, vector<OrderBookEntry>> marketStats;
                    OrderBook orderBook{"20200601.csv"};
};