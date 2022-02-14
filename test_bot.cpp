#include "test_bot.h"
#include "Commander.h"

using namespace std;

                              // * Bot constructor.
// * Takes in a reference to a TradeInterface object to provide an interface with the exchange/trade simulator. 
TestBot::TestBot(TradeInterface& _tradingInterface):
tradingInterface(_tradingInterface){};

                    // * Bot functionalities  *//

// *Initializes the bot, and the TradeInterface. 
void TestBot::init(string _username)
{
          tradingInterface.init(); 
};

//* Runs the bot's main loop 
void TestBot::run()
{
         
          // a regex to remove unwanted characters from user input 
          const regex re(R"([\s|,|.]+)"); 
          while (true)
          {
                    string line = getUserCmd();
                    list<string> parsedTokens = Commander::tokenize(line, re);
                    queue<Commands> commands = Commander::tokensToCommands(parsedTokens);
                    queue<string> arguments = Commander::tokensToArguments(parsedTokens);
                    parseCommands(commands, arguments);
                    
          };
};

//*Returns a vector of OBE bid objects for the given product and timeframe. Uses the interface.
vector<OrderBookEntry> TestBot::getBids(string product, string timestamp)
{
          return tradingInterface.getBids(product, timestamp);
};

//*Returns a vector of OBE ask objects for the given product and timeframe.  Uses the interface.
vector<OrderBookEntry> TestBot::getAsks(string product, string timestamp)
{    
          return tradingInterface.getAsks(product, timestamp);
};

//*Gets and returns the current time.  Uses the interface.
string TestBot::getTime()
{

          return tradingInterface.getCurrentTime();
        
}

//*Helper function. Iterates over a map of key:product - value:corresponding vector of OBE objects pairs and prints the results to the console
void TestBot::unpack(map<string, vector<OrderBookEntry>> marketStats)
{
          
          try 
          {
                    map<string, vector<OrderBookEntry>>::iterator itr;
                    for (itr = marketStats.begin(); itr!= marketStats.end(); ++itr)
                    {
                              cout <<"Product :"<< itr->first
                              << " entries: "<< itr->second.size() << '\n'; 
                    };
          } 
          catch(const exception& e)
          {
                    cout << "Error in unpacking." << e.what() << '\n';
          }
};

//* Gets the current user input from the console and returns it.
string TestBot::getUserCmd() 
{
          string line;
          cout << "spit your command: " << '\n'; 
          getline(cin,line);
          return line; 
};

//* Parses the commands queue and decides which command to run, passing the arguments queue as input to the selected method. 
//* Receives the output and passes it to the outputResult method for displaying to the user, where applicable.
void TestBot::parseCommands(queue<Commands>& commands, queue<string>& arguments)
 {

          results result; 
          if (!commands.empty())
          {
                    int argSize = arguments.size();
                    try 
                    {

                              for (int i = 0; i < commands.size(); ++i)
                              {
                                        
                                        auto &cmd = commands.front();
                                        commands.pop();

                                        switch (cmd)
                                        {

                                                  case Commands::help:
                                                            result = help(commands);
                                                            outputResult(result, cmd);
                                                            break;
                                                  case Commands::avg:
                                                            if(arguments.size() == 3) 
                                                            {         
                                                                      result = avg(arguments);
                                                                      outputResult(result, cmd);
                                                                      break;
                                                            }
                                                            else 
                                                            {
                                                                      cout << "Not enough arguments." << '\n';          
                                                            };
                                                            break;
                                                  case Commands::predict:
                                                            if(arguments.size() == 3)
                                                            { 
                                                                      result = predict(arguments);
                                                                      outputResult(result, cmd);
                                                                      break;
                                                            }
                                                            else{
                                                                      cout << "Not enough arguments."<< '\n';
                                                            }
                                                            break;
                                                  case Commands::frl:
                                                            if(arguments.size()==2)
                                                            {
                                                                      result = frl(arguments);
                                                                      outputResult(result,cmd);
                                                                      break;
                                                            }
                                                            else
                                                            {
                                                                      cout << "Not enough arguments." << '\n';
                                                            }
                                                            break;
                                                  case Commands::prod:
                                                            result = prod();
                                                            outputResult(result, cmd);
                                                            break;
                                                  case Commands::min:
                                                            if(arguments.size()==2)
                                                            {
                                                                      result = min(arguments);
                                                                      outputResult(result, cmd);
                                                                      break;
                                                            }
                                                            else
                                                            {
                                                                      cout << "Invalid number of arguments." << '\n';
                                                                      break;
                                                            }
                                                            
                                                  case Commands::max:
                                                            if(arguments.size()==2)
                                                            {
                                                                      result = max(arguments);
                                                                      outputResult(result, cmd);
                                                                      break;
                                                            }
                                                            else
                                                            {
                                                                      cout << "Invalid number of arguments." << '\n';
                                                                      break;
                                                            }
                                                            
                                                  case Commands::marketTime:
                                                            time();
                                                            break;
                                                  case Commands::step:
                                                            step();
                                                            break;
                                                  default:
                                                            cout << "Invalid command." << '\n';
                                        };
                              };               
                    } 
                    catch(const exception& e)
                    {
                              cout << "Error in bot command parsing" << e.what() << '\n';
                    };
          }
          else
          {
                    cout << "Empty command." << endl;
                    return;
          }
 };

//* Allows the bot to go to a timeframe according to input.
//* Used to go to the next timeframe (or previous where needed)
void TestBot::jumpToTimeFrame(int jumps)
{
                  
          try
          {
                    tradingInterface.jumpToTimeFrame(jumps);
          } 
          catch (const exception& e)
          {
                    cout << "Error with changing timeframe" << e.what() << '\n';
          };
}

//* Helper that takes in a queue by reference, returns the first element, then pops the queue.
//* Used within the bot commands for handling the arguments queue
string TestBot::retrieveAndPop(queue<string> &arguments)
{
          string argument;
          try 
          {
                    argument = arguments.front();
                    arguments.pop();
          }
          catch(const exception& e) 
          {
                    cout << "Error with retrieving arguments" << e.what() << '\n';
          };
          return argument;
};

//* Helper to check whether an orderType is valid
bool TestBot::validType(const string& orderType)
{
          try 
          {
                    OrderBookType obt = OrderBookEntry::stringToOrderBookType(orderType);
                    if (obt != OrderBookType::unknown ) return true;
          }
          catch(const exception& e)
          {
                    cout << "Error with validating ordertype" << e.what() << '\n';
          };
          return false;
};

//* Helper to check whether a peak passed as an argument to a command is valid
bool TestBot::validPeak(const string& peak)
{
          if (peak == "MAX"|| peak =="MIN")
          {
                    return true ;
          };
          return false;
};

//* Helper to check whether a product passed as an argument to a command is valid
bool TestBot::validProduct(const string& product)
{
        
          try
          {
                    vector<string> availProducts = tradingInterface.getKnownProducts();
                    for (const string& prod : availProducts)
                   
                              if(prod==product) return true;
          }
          catch(const exception& e)
          {
                    cout << "Error with validating product" << e.what() << '\n';
          };
          return false;
          
};
          

                              //* Bot commands *//

//* Predicts price momentum. Calculates the rate of price change between the current time and the number of steps taken since the start.
// * usage: predict max/min ETH/BTC bid/ask
//* info at -> // https: // www.investopedia.com/terms/r/rateofchange.asp
TestBot::results TestBot::predict(queue<string> &arguments)
{
  
          results result;
          double rCp;
          double nrCp;
 
          // ROC = (Cp - Cp-n) / (Cp-n) * 100
          // Closing price of recent period= Cp
          // Closing price n periods before most recent =Cp-n
          
      
          int distanceFromStart = tradingInterface.getNumberOfJumpsMade();
         
          if(distanceFromStart>0)
          {
          
                    const string peak = retrieveAndPop(arguments);
                    const string product = retrieveAndPop(arguments);        
                    const string type = retrieveAndPop(arguments);
                    try {

                              if(validPeak(peak))
                                        {
                                                  if(peak=="MAX")
                                                  {
                                                            if (validType(type) && validProduct(product))
                                                            {
                                                                      // 2. get closing price of most recent period
                                                                      rCp = tradingInterface.currentHigh(product, type, getTime());
                                                                      jumpToTimeFrame(-distanceFromStart);
                                                                      
                                                                      // 3. get closing price of n periods before  rCp
                                                                      nrCp = tradingInterface.currentHigh(product, type, getTime());
                                                                     
                                                                      jumpToTimeFrame(distanceFromStart);
                                                            }
                                                  
                                                            else 
                                                            {
                                                                      result.text = "Please select appropriate type: bid/ask";
                                                                      
                                                            };
                                                  }
                                                  if (peak == "MIN")
                                                  {
                                                            if (validType(type) && validProduct(product))
                                                            {
                                                                      rCp = tradingInterface.currentLow(product, type, getTime());
                                                                      jumpToTimeFrame(-distanceFromStart); //moves back in time to find the price

                                                                      nrCp = tradingInterface.currentLow(product, type, getTime());
                                                                      jumpToTimeFrame(distanceFromStart);// returns back to the currentTime.
                                                            }
                                                  
                                                            else 
                                                            {
                                                                      result.text ="Please select appropriate type: bid/ask";
                                                            };
                                                  }
                                      

                                                  result.number = ((rCp - nrCp) / nrCp) * 100;

                                                  if (result.number > 0) result.text = "Price might be moving upwards with a rate of change : " +to_string(result.number);
                                                  else if (result.number < 0) result.text = "Price might be moving downwards with a rate of change : " + to_string(result.number);
                                                  else result.text ="Price is going nowhere. Good luck.";
                                        }
                                        else result.text = "Enter appropriate peak";
                    }catch (const exception& e)
                    {
                              cout << "catched in predict: "<< e.what() << '\n';
                    };
          }
          else
          {
                    result.text = "Not enough timesteps available";
          };

          return result;
};

 //*Goes to next time frame. Modifies the trade sim's currentTime variable.
void TestBot::step()
 {
        
          try 
          {
                    jumpToTimeFrame(1);
                    time();
          }
          catch (const exception &e)
          {
                    cout << "Error with max." << e.what() << '\n';
          };
};

//*prints to console the current timeframe
void TestBot::time()
{
          try 
          {
                    cout << "This is the bot speaking current time is :" << getTime() << '\n';
          }
          catch (const exception &e)
          {
                    cout << "Error with getting time." << e.what() << '\n';
          }
};

//* gets the minimum price for a given product at the current timeframe
// * usage: min product ordertype
TestBot::results TestBot::min(queue<string> &arguments)
{
          //get the min ask/bid price for the current timeframe
          //command - > min ETH/BTC ask
          results result ; 
   
          try{
                    const string product = retrieveAndPop(arguments);

                    const string type = retrieveAndPop(arguments);

                    if(validProduct(product))
                    { 
                              if(validType(type))
                              {
                                        result.number = tradingInterface.currentLow(product, type, getTime());

                                        result.text = "Min price for current timestep: " + getTime() + " is " + to_string(result.number);
                              }
                              else result.text = "Enter appropriate type.";
                                        
                    }
                    else result.text = "Enter appropriate product.";
                    
          }
          catch (const exception &e)
          {
                    cout << "Error with max." << e.what() << '\n';
          };

          return result;

};

//* gets the maximum price for a given product at the current timeframe
// * usage: max product ordertype
TestBot::results TestBot::max(queue<string> &arguments)
{
          
          results result; 

          try
          {
                    const string product = retrieveAndPop(arguments);

                    const string type = retrieveAndPop(arguments);
          
                    if (validProduct(product))
                    {
                              if (validType(type))
                              {
                                        result.number = tradingInterface.currentHigh(product, type, getTime());

                                        result.text = "Max price for current timestep: " + getTime() + " is " + to_string(result.number);
                              }
                              else
                                        result.text = "Enter appropriate type.";
                    }
                    else result.text = "Enter appropriate product.";
          }

          catch (const exception &e)
          {
                   cout << "Error with max." << e.what() << '\n' ;
          };
     
          return result; 
};

//* gives help information on available commands
// * usage: help OR help cmd
TestBot::results TestBot::help(queue<Commands> &commands)
{
          TestBot::results result;
          if (!commands.empty())
          {
                    Commands option = commands.front();
                    commands.pop();

                    switch(option)
                    {
                              case Commands::avg:
                                        result.text = "Gets the average price for a given product pair and a given interval of previous timesteps - usage: avg ETH/USDT ask 10";
                                        break;
                              case Commands::predict:
                                        result.text = "Possibly predicts the next price movement by calculating the rate of change. Rates above 0 mean price is going up and vice versa. - usage: predict max/min ETH/USDT ask/bid";
                                        break;
                              case Commands::prod:
                                        result.text = "Displays all listed product pairs available at the current market time. - usage: prod" ;
                                        break;
                              case Commands::min:
                                        result.text = "Gets the minimum price for a given product and ordertype at the current timeframe - usage: min ETH/USDT ask";
                                        break;
                              case Commands::max:
                                        result.text = "Gets the maximum price for a given product and ordertype at the current timeframe - usage: max ETH/USDT ask";
                                        break;
                              case Commands::marketTime:
                                        result.text = "Displays the current market time. - usage: time";
                                        break;
                              case Commands::step:
                                        result.text = "Moves the market time a step forward. Each step is a 5 second interval. - usage: step";
                                        break;
                              case Commands::frl:
                                        result.text = "Displays the fibonacci retracement levels from the earliest available timeframe to the current one. - usage: frl ETH/USDT ask/bid";
                                        break;
                              case Commands::invalid:
                                        result.text = "Invalid command, try again";
                                        break;
                    };
          }
          else
          {
                    result.text = "Available commands : avg, time, predict, prod, step, min, max, frl \n  help <cmd> - for more info." ;
          
          };
          return result; 

}

//* Gets the average price for a given product for a given  interval of timesteps.
//* Checks whether sufficient steps are available.
//* usage avg ETH/BTC ask 10
TestBot::results TestBot::avg(queue<string> &arguments)
{

          results result;
         
          try
          {
                    
                    const string product = retrieveAndPop(arguments);

                    const string type = retrieveAndPop(arguments);

                    const int prevSteps = stoi(retrieveAndPop(arguments));
                    int distanceFromStart = tradingInterface.getNumberOfJumpsMade();
                    if(distanceFromStart>=prevSteps)
                    {
                              if (validProduct(product))
                              {
                                        if (validType(type))
                                        {
                                                  result.number = tradingInterface.averageFor(-prevSteps, type, product);
                                                  result.text = "The average is : " + to_string(result.number);
                                        }
                                        else
                                                  result.text = "Enter appropriate type.";
                              }
                              else
                                        result.text = "Enter appropriate product.";
                    }
                    else result.text = "Not enough timesteps available." ;
                    
                    
                    
          
          }
          catch (const exception &e)
          {
                    cout <<  e.what() << '\n';
          };
          
          return result;
};


//* Gets market stats for the current timeframe. Returns a result struct.
TestBot::results TestBot::prod()
{
          results result;
          try {
                    vector<map<string, vector<OrderBookEntry>>> currentMarketEntries = tradingInterface.getCurrentMarketStats();

                    result.totalAskEntries = currentMarketEntries[0];
                    result.totalBidEntries = currentMarketEntries[1];

                    result.text = " the market has entries for: " + to_string(result.totalAskEntries.size()) + " asking " + "and " + to_string(result.totalBidEntries.size()) + " bid, products." + '\n';
          }
          catch (const exception &e)
          {
                    cout << "Error with products" << e.what() << '\n';
          };
          return result;
};

// * Fibonacci retracement level
// * usage: FRL product bid/ask
TestBot::results TestBot::frl(queue<string> &arguments)
{
          // 23.6%, 38.2%, 50%, 61.8%, and 78.6%.
          // For 23.6% - ($15 - ($5 x 0.236) = $13.82).
          // for  50%  ($15 - ($5 x 0.5) = $12.50).
          results result;
          const string product = retrieveAndPop(arguments);
          const string type = retrieveAndPop(arguments);
          int distanceFromStart = tradingInterface.getNumberOfJumpsMade();
          if (distanceFromStart>0)
          {
                    try
                    {
                              if(validProduct(product))
                              {       
                                        if(validType(type))
                                        {
                                                  // 2. get the earliest time price point for the product chosen
                                                  const double earliestLowestPrice = tradingInterface.currentLow(product, type, tradingInterface.getEarliestTime());
                                                  const double currentHighestPrice = tradingInterface.currentHigh(product, type, getTime());
                                                  const double differenceBetween = currentHighestPrice - earliestLowestPrice;

                                                  const double fib_23 = currentHighestPrice - (differenceBetween * 0.236);
                                                  const double fib_38 = currentHighestPrice - (differenceBetween * 0.382);
                                                  const double fib_50 = currentHighestPrice - (differenceBetween * 0.5);
                                                  const double fib_61 = currentHighestPrice - (differenceBetween * 0.618);
                                                  const double fib_78 = currentHighestPrice - (differenceBetween * 0.786);

                                                  result.text = "Fibonnaci retracement levels are : \n 23.6% : " + to_string(fib_23) + "\n 38.2% : " +  to_string(fib_38) + "\n  50% : " +  to_string(fib_50)  + "\n 61.8% :" + to_string(fib_61) + "\n 78.6% :" + to_string(fib_78)  + "\n";
                                        }
                                        else result.text = "Enter appropriate type" ;
                              } else result.text = "Enter appropriate product";
                              
                    } catch(const exception& e)
                    {
                              cout << "Error with frl command" << e.what() << '\n';
                    };
          } 
          else result.text = "Not enough timesteps available";
          return result;
};

//* Outputs command results to the console.
void TestBot::outputResult(TestBot::results& result, Commands &callerCmd)
{

          switch (callerCmd)
          {
                    case Commands::help:
                              cout << result.text << '\n';
                              break;
                    case Commands::avg:
                              cout << result.text << '\n';
                              break;
                    case Commands::predict:
                              cout << result.text << '\n';
                              break;
                    case Commands::prod:
                              cout 
                              << result.text 
                              << "Ask entries & products : "
                              << '\n';
                              unpack(result.totalAskEntries);
                              cout << "Bid entries & products : " 
                              << '\n';
                              unpack(result.totalBidEntries);
                              break;
                    case Commands::min:
                              cout << result.text << '\n';
                              break;
                    case Commands::max:
                              cout << result.text << '\n';
                              break;
                    case Commands::frl:         
                              cout << result.text << '\n';
                              break;
                    default:
                              cout << "invalid" << '\n';
                              break;
          };
};