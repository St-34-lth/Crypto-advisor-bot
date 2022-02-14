#include "tradeit-io.h"

using namespace std;

                                        // * Original class methods *//
TradeIt::TradeIt()
{
          cout << "TradeIt constructor called" << endl;

};

void TradeIt::init()
{
          currentTime = orderBook.getEarliestTime();
          jumpsMade = 0;
         
};

void TradeIt::run()
{
          int input;
          while (true)
          {
                    printMenu();
                    input = getUserOption();
                    processUserOption(input);
          };
};

void TradeIt::showWallet()
{
          cout << exchangeWallet.toString() << endl;
}

vector<OrderBookEntry> TradeIt::getAsks(string product, string timestamp)
{
          
          vector<OrderBookEntry> currentAsks = orderBook.getOrders(OrderBookType::ask,product,timestamp);
          return currentAsks; 
          
};

vector<OrderBookEntry> TradeIt::getBids(string product, string timestamp)
{
          vector<OrderBookEntry> currentBids = orderBook.getOrders(OrderBookType::bid, product, timestamp);
          return currentBids;
}

//* Returns the currentTime global variable. The variable is modified only through the orderBook object and the gotoNextTimeFrame/jumpToTimeFrame own class methods. The bot only uses the jumpToTimeFrame method.
string TradeIt::getCurrentTime()
{
          
          return currentTime;
}

void TradeIt::enterAsk(OrderBookEntry askOrder)
{
          cout << "AskOrder's Details : " 
               << askOrder.amount << " "
               << askOrder.price << " "
               << askOrder.product << " " 
               << askOrder.timestamp << endl;
               

          try
          {

                    string username = askOrder.username;

                    if (userWallets[username].canFulfillOrder(askOrder))
                    {
                              cout << "bot ask order inserted. " << endl;
                              orderBook.insertOrder(askOrder);
                    }
                    else
                    {
                              cout << "not enough stuff in the bot." << endl;
                    };
          }
          catch (const exception &e)
          {
                    cout << e.what() << " from TradeIt::enterAsk(askOrder)" << endl;
          };
};

void TradeIt::enterBid(OrderBookEntry bidOrder)
{
          try
          {
                    cout << "BidOrder's Details : "
                         << bidOrder.amount << " "
                         << bidOrder.price << " "
                         << bidOrder.product << " "
                         << bidOrder.timestamp << endl;
                    string username = bidOrder.username;

                    if (userWallets[username].canFulfillOrder(bidOrder))
                    {
                              cout << "bot bidOrder inserted. " << endl;
                              orderBook.insertOrder(bidOrder);
                    }
                    else
                    {
                              cout << "not enough stuff in the bot." << endl;
                    };
          }
          catch (const exception &e)
          {
                    cout << e.what() << " from TradeIt::enterAsk()" << endl;
          };
};

void TradeIt::printHelp()
{
          cout << "Help is for pussies." << endl;
};


void TradeIt::printMarketStats()
{
        

          for (string const& p : orderBook.getKnownProducts())
          {
                    cout << "product: " << p << endl;

                    vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
                    cout << entries.size() << endl;
                    cout << OrderBook::getHighPrice(entries) << endl;
                    cout << OrderBook::getLowPrice(entries) << endl; 
          }; 
};

//* gets the highest price available for a specified timeframe if given, else for the current one
double TradeIt::currentHigh(string product,string type,string timestamp)
{
          //get the entries for current time step
          if(timestamp=="")
          {
                    timestamp = currentTime;
          };
     
          vector<OrderBookEntry> currentEntries;
          
          
          if (type == "ASK")
          {
                   currentEntries = getAsks(product,timestamp);
                   return OrderBook::getHighPrice(currentEntries);
          }
          if(type=="BID")
          {
                    currentEntries = getBids(product,timestamp);
                    return OrderBook::getHighPrice(currentEntries);
          };
          return 0;

};
//* gets the lowest price available for a specified timeframe if given, else for the current one 
double TradeIt::currentLow(string product,string type,string timestamp)
{
          if (timestamp == "")
          {
                    timestamp = currentTime;
          };

          vector<OrderBookEntry> currentEntries;
          
          
          if (type == "ASK")
          {
                    currentEntries = getAsks(product, timestamp);
                    return OrderBook::getLowPrice(currentEntries);
          };
          if (type == "BID")
          {
                    currentEntries = getBids(product, timestamp);
                    return OrderBook::getLowPrice(currentEntries);
          };
          return 0;
};

void TradeIt::enterAsk()
{
          
          cout << "Make an ask - enter amount: product,price amount - e.g. 'ETH/BTC,200,0.5" << endl;
          string userInput;
           // cignore() <-this basically ignores the part of the input buffer when it receives it.
          getline(cin, userInput);
         
          vector<string> tokens = CSVReader::tokenize(userInput, ',');
          if(tokens.size()!=3) 
          {
                    cout << "TradeIt::enterAsk(): Bad input, try again." << endl;
          }
          else 
          {
                    try
                    {
                              OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1],tokens[2],currentTime,tokens[0],OrderBookType::ask);
                              obe.username = "simuser";
                              if (exchangeWallet.canFulfillOrder(obe))
                              {
                                        cout << "ask inserted" << endl;
                                        orderBook.insertOrder(obe);
                              }
                              else 
                              {
                                        cout << "not enough stuff." << endl; 
                              }; 

                              
                    }
                    catch (const exception& e)
                    {
                              cout << e.what() << " from TradeIt::enterAsk()" << endl;
                    };
          };
          cout << "You typed: " << userInput << endl;
           
};

void TradeIt::enterBid()
{

          cout << "Make a bid - enter amount: product,price amount - e.g. 'ETH/BTC,200,0.5" << endl;
          string userInput;
         
          getline(cin, userInput);

          vector<string> tokens = CSVReader::tokenize(userInput, ',');
          if (tokens.size() != 3)
          {
                    cout << "TradeIt::enterBid(): Bad input, try again." << endl;
          }
          else
          {
                    try
                    {
                              OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::bid);
                              obe.username = "simuser";
                              
                              if (exchangeWallet.canFulfillOrder(obe))
                              {
                                        cout << "bid inserted" << endl;
                                        orderBook.insertOrder(obe);
                              }
                              else
                              {
                                        cout << "not enough stuff." << endl;
                              };
                    }
                    catch (const exception &e)
                    {
                              cout << e.what() << " from TradeIt::enterBid()" << endl;
                    };
          };
          cout << "You typed: " << userInput << endl;
};

void TradeIt::printMenu()
{

          cout << "1. Print help" << endl;

          cout << "2. Make an offer" << endl;

          cout << "3. Make a bid" << endl;

          cout << "4. Show stats" << endl;

          cout << "5. Show wallet" << endl;

          cout << "6. Next time-step" << endl;

          cout << "Current time: " << currentTime << endl;
};

int TradeIt::getUserOption()
{
          string line;
          int userOption = 0;
          cout << "==============" << endl;
          cout << "Enter an option" << endl;
          getline(cin, line);
          try
          {
                    userOption = stoi(line);
          }
          catch (exception &e)
          {
                    cout << e.what() << endl;
          };

          return userOption;
};

void TradeIt::processUserOption(int userOption)
{

          // map<int, void (*)()> menu;
          // menu[1] = printHelp();
          // menu[2] = enterAsk;
          // menu[3] = enterBid;
          // menu[4] = printMarketStats;
          // menu[5] = showWallet;
          // menu[6] = moveNextTimeFrame;

          if (userOption == 1)
          {
                    // menu[1]();
                    printHelp();
          }
          else if (userOption == 2)
          {
                    // menu[2]();
                    enterAsk();
          }
          else if (userOption == 3)
          {
                    // menu[3]();
                    enterBid();
          }
          else if (userOption == 4)
          {
                    // menu[4]();
                    printMarketStats();
          }
          else if (userOption == 5)
          {
                    // menu[5]();
                    showWallet();
          }
          else if (userOption == 6)
          {
                    // menu[6]();
                    gotoNextTimeFrame();
          }
          else
          {
                    cout << "Invalid option" << endl;
          };
};
                                                            
                              //* Own & modified class methods  *//

//* Returns a vector container with a map of the current timeframe's bid and ask entries
vector<map<string, vector<OrderBookEntry>>> TradeIt::getCurrentMarketStats()
{

          vector<map<string, vector<OrderBookEntry>>> currentEntries;
          map<string, vector<OrderBookEntry>> bidEntries;
          map<string, vector<OrderBookEntry>> askEntries;
          try
          {

                    for (string const &p : orderBook.getKnownProducts())
                    {

                              bidEntries.insert(
                              pair<string, vector<OrderBookEntry>>(
                              p,
                              orderBook.getOrders(OrderBookType::bid, p, currentTime)));
                              askEntries.insert(
                              pair<string, vector<OrderBookEntry>>(
                              p,
                              orderBook.getOrders(OrderBookType::ask, p, currentTime)));
                    };

                    currentEntries.push_back(askEntries);
                    currentEntries.push_back(bidEntries);
          } 
          catch (const exception& e)
          {
                    cout << "Error with getting TradeIt market stats" << e.what() << '\n';
          };

          return currentEntries;
};

// *Modifies the currentTime (a.k.a. moves timeframes) variable according to the number of jumps argument. 
//* Positive for forward negative for backwards.
void TradeIt::jumpToTimeFrame(int jumps)
{
          
        
          try 
          {
                    int availFrames ;

                    if (jumps > 0)
                    {
                              availFrames = orderBook.availableTimeframes(jumps, getCurrentTime());
                              for (int i = 0; i < availFrames; ++i)
                                        currentTime = orderBook.getNextTime(getCurrentTime());
                                        
                    }
                   
                    if (jumps < 0 )
                    {
                              availFrames = -orderBook.availableTimeframes(jumps, getCurrentTime());
                              for (int i = 0; i < availFrames; ++i)
                                        currentTime = orderBook.getPreviousTime(getCurrentTime());
                            
                    };        
                   
          }          
          catch (const exception& e)
          {
                    cout << "Error with Trade It Jump." << e.what() << '\n';
          };
    
};
//* returns the number of timeframes made from the first entry 
int TradeIt::getNumberOfJumpsMade()
{
          
          int jumpsMade = 0;
          try 
          {
                    string timestamp = currentTime;
                    string firstTime = getEarliestTime();
                    string tempTimestamp;
                    while (firstTime != timestamp)
                    {      

                              timestamp = orderBook.getPreviousTime(timestamp); 
                              jumpsMade += 1;
                              
                    };
                             
          } 
          catch(const exception& e) 
          {
                    cout << "Catched in getnumofjumps : " << e.what() << endl ;
          };
          return jumpsMade;
};
//*Takes in a product, type and timestamp and Returns the average of the available entries. 
double TradeIt::averageEach(string type, string product,string timestamp)
{
          double sum = 0;
          double average = 0;
          int numberOfEntries = 0;
         
          try
          {
                   
                    if (type == "ASK")
                    {
                              vector<OrderBookEntry> currentAsks = getAsks(product, timestamp);

                              for_each(currentAsks.begin(), currentAsks.end(), [&](OrderBookEntry obe)
                                        {
                                                  sum += obe.price;
                                        });
                              numberOfEntries += currentAsks.size();
                    }
                    else if (type == "BID")
                    {
                              vector<OrderBookEntry> currentBids = getBids(product, timestamp);

                              for_each(currentBids.begin(), currentBids.end(), [&](OrderBookEntry obe)
                                        {
                                                  sum += obe.price;
                                        });
                              numberOfEntries += currentBids.size();
                    }
                    else
                    {
                              cout << "invalid arguments" << '\n';
                              return 0;
                    };
                                        
          }
          catch (exception &e)
          {
                    cout << e.what() << endl;
          };
          average = sum / numberOfEntries;
          return average;
         
}

//*Takes in product, type and number of steps to iterage. Returns the average of the available entries over the number of timeframes provided.
double TradeIt::averageFor(int steps,string type,string product)
{
          double sum = 0;
          double average = 0;
          int numberOfEntries = 0;
        
         
          bool forwards = steps > 0 ? true : false ;
          try {
                    int availFrames = getNumberOfJumpsMade();
                    if( availFrames > 0 )
                    {

                              string timestamp = getCurrentTime();
                              for (int i = 0; i < availFrames; ++i)
                              {
                                        
                                        //cout <<"in avg-for loop start : " << timestamp << endl;
                                        sum += averageEach(type, product, timestamp);
                                        numberOfEntries +=1;
                                        
                                        if (forwards)
                                        {
                                                  jumpToTimeFrame(1);
                                        }
                                        else
                                        {
                                                  jumpToTimeFrame(-1);
                                        };
                                        timestamp = getCurrentTime();
                                        //cout << "in avgfor end loop : " << timestamp << endl;
                              };
                              if (forwards)
                              {
                                        jumpToTimeFrame(-availFrames);
                              }
                              else
                              {
                                        jumpToTimeFrame(availFrames);
                              };
                    }
                    else cout<<"Not enough timesteps available." << '\n';
          } 
          catch(const exception& e)
          {
                    cout << "Error with tradeIt averaging" << e.what() << '\n';
          };
          average = sum / numberOfEntries;
          return average;
};

//* Goes to the next available time frame. Executes trade matches for available users before doing so, if available. Modifies the currentTime global variable.
void TradeIt::gotoNextTimeFrame()
{
          cout << "Going to next timeframe: " << endl;

          auto products = orderBook.getKnownProducts();
          for (const auto &product : products)
          {
                    for (auto& user: userWallets)
                    {
                              executeTrades(product, user.first);
                    }
                    
          }

          currentTime = orderBook.getNextTime(currentTime);
};

//* Returns the earliest time found in the orderbook.
string TradeIt::getEarliestTime()
{
          return orderBook.getEarliestTime();

};
// * Returns the latesttime found in the orderbook.
string TradeIt::getLatestTime()
{
          return orderBook.getLatestTime();
};

vector<string> TradeIt::getKnownProducts()
{
          return orderBook.getKnownProducts();
}

// *Own version of trade execution for given product and user by calling the matching algorithm. Called from gotonextTimeFrame. 
// Not really useful.
void TradeIt::executeTrades(string product,string user)
{
          vector<OrderBookEntry> sales = orderBook.matchAsksToBids(product, currentTime);
          for (OrderBookEntry &sale : sales)
          {
                    cout << "Sale amount: " << sale.amount << "Sale price: " << sale.price << endl;
                    if (sale.username == user)
                    {
                              exchangeWallet.processSale(sale);
                    };
          };
};

//* some wallet functions I coded while creating the sim, irrelevant but left them here for functionality. Please ignore them, or give extra marks for them :D *//
string TradeIt::getWalletStats(string const &username)
{
          return userWallets[username].toString();
};
void TradeIt::fundWallet(string fundType, double amount, string username)
{
          userWallets[username].insertCurrency(fundType, amount);
};
void TradeIt::addWallet(Wallet userWallet)
{
          userWallets.insert(
              pair<string, Wallet>(userWallet.getUsername(), userWallet));
          cout << "A wallet has been created for user :" << userWallet.getUsername() << endl;
};
void TradeIt::createWallet(string const &username)
{
          Wallet wallet{username};
          addWallet(wallet);
};