#include <iostream>
#include "OrderBook.h"
#include <vector>
#include <string>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
using namespace std;

OrderBook::OrderBook(string filename)
{
          cout << "OrderBook constructed" << endl;
          orders = CSVReader::readCSV(filename);
          

};

vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,string product,string timestamp)
{
          vector <OrderBookEntry> orders_sub; 
          
          for(OrderBookEntry& e : orders)
          {
                    if(e.orderType == type && e.product== product && e.timestamp==timestamp)
                    {
                              orders_sub.push_back(e);
                    };
          };


          
          return orders_sub;
};

vector<string> OrderBook::getKnownProducts()
{
          vector<string> products;
          map<string,bool> prodMap;
         

          for (OrderBookEntry& e : orders)
          {
                    prodMap[e.product] =true; 
          };

          for (auto const& e : prodMap) 
          {
                    products.push_back(e.first); 
          };
          return products;
};

double OrderBook::getHighPrice(vector<OrderBookEntry>& orders)
{
          double max = orders[0].price;

          for(OrderBookEntry& o : orders)
          {
                    if(o.price > max)
                    {
                              max = o.price;
                    };

          };
          return max; 
};

double OrderBook::getLowPrice(vector<OrderBookEntry>& orders)
{
          double lowest = orders[0].price;

          for (OrderBookEntry& o : orders)
          {
                    if (o.price < lowest)
                    {
                              lowest = o.price;
                    };
          };
          return lowest;
};

void OrderBook::insertOrder(OrderBookEntry& order)
{
          orders.push_back(order);

          sort(orders.begin(), orders.end(), OrderBookEntry::timestampCompare);
};

string OrderBook::getEarliestTime()
{
          return orders[0].timestamp;
};


string OrderBook::getNextTime(string timestamp)
{
          string next_timestamp = timestamp;

          for(OrderBookEntry& e : orders)
          {
                    if (e.timestamp > next_timestamp)
                    {
                              next_timestamp = e.timestamp;
                              break;
                    };
          };
          if(next_timestamp=="")
          {
                    next_timestamp = orders[0].timestamp;
          };
         
          return next_timestamp;

};


vector<OrderBookEntry> OrderBook::matchAsksToBids(string product, string timestamp)
{
          vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
          vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
          vector<OrderBookEntry> sales;

          sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
          sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceDesc);

          for (OrderBookEntry &ask : asks)
          {
                    for (OrderBookEntry &bid : bids)
                    {
                              if (bid.price >= ask.price)
                              {
                                        OrderBookType type = OrderBookType::asksale;
                                        OrderBookEntry sale{ask.price, 0, ask.timestamp, ask.product, OrderBookType::asksale};
                                        if (bid.username == "simuser")
                                        {

                                                  sale.username = "simuser";
                                                  sale.orderType = OrderBookType::bidsale;
                                        };
                                        if (ask.username == "simuser")
                                        {

                                                  sale.username = "simuser";
                                                  sale.orderType = OrderBookType::asksale;
                                        };

                                        if (bid.amount == ask.amount)
                                        {
                                                  sale.amount = ask.amount;
                                                  sales.push_back(sale);
                                                  bid.amount = 0.0;
                                                  break;
                                        };
                                        if (bid.amount > ask.amount)
                                        {
                                                  sale.amount = ask.amount;
                                                  sales.push_back(sale);
                                                  bid.amount = bid.amount - ask.amount;
                                                  break;
                                        };
                                        if (bid.amount < ask.amount)
                                        {
                                                  if (bid.amount < ask.amount && bid.amount > 0)
                                                  {
                                                            sale.amount = bid.amount;
                                                            sales.push_back(sale);
                                                            ask.amount = ask.amount - bid.amount;
                                                            bid.amount = 0;
                                                            continue;
                                                  };
                                        };
                              };
                    };
          };
          return sales;
};
                            
                            
                              //* Own & Modified class methods *//

// *checks whether the timeframe steps are available in the orderbook. Input Positive for forward checking. Negative for backwards checking. Returns 0 if not enough timeframes available otherwise the available number of timeframes.
int OrderBook::availableTimeframes(int timeframes, string timestamp)
{

          
          int forwardFramesAvailable = 0;
          int backwardFramesAvailable = 0;
          
          if (timeframes > 0)
          {
                    try{
                              string earliestTimeframe = getEarliestTime();
                              string latestTimeFrame = getLatestTime();

                              while(timestamp!=latestTimeFrame)
                              {
                                        forwardFramesAvailable += 1;
                                        if(timeframes <= forwardFramesAvailable) return timeframes;
                                        timestamp = getNextTime(timestamp);
                              };
                              
                              
                    }
                    catch(const exception& e)
                    {
                              cout << "Error in getting available timeframes" << e.what() << '\n';
                    };
          };
          if (timeframes < 0)
          {
                    try
                    {
                              string earliestTimeframe = getEarliestTime();
                              string latestTimeFrame = getLatestTime();

                              while (timestamp != earliestTimeframe)
                              {
                                        backwardFramesAvailable += 1;
                                        if (timeframes <= backwardFramesAvailable)return timeframes;
                                        timestamp = getPreviousTime(timestamp);
                                        
                              };
                             
                    }
                    catch (const exception &e)
                    {
                              cout << "Error in getting available timeframes" << e.what() << '\n';
                    };
          };
          return 0;
};

//* Helper function to check previous available timeframes in the CSV. Takes in a timestamp and returns its immediate previous. 
string OrderBook::getPreviousTime(string timestamp)
{
          string prev_timestamp = timestamp;
       
          const string &firstTimestamp = getEarliestTime();
          for (int i = orders.size() - 1; i > 0; --i)
          {
                    
                    auto &obe = orders.at(i);
                    
                    if (prev_timestamp == firstTimestamp)
                    {

                              break;
                    };

                    if (obe.timestamp > prev_timestamp)
                    {
                              continue;
                    }
                    
                    if (obe.timestamp < prev_timestamp)
                    {
                              prev_timestamp = obe.timestamp;
                              break;
                    }
                    prev_timestamp = obe.timestamp;
          };

          return prev_timestamp;
}

//* Returns the latest timeframe available from the CSV
string OrderBook::getLatestTime()
{
          return orders[orders.size() - 1].timestamp;
}
