#include <string>
#pragma once

using namespace std;
enum class OrderBookType
{
          bid,
          ask,
          unknown,
          sale,
          asksale,bidsale
};
//specification of the class OrderBookEntry - essentially a promise for an implementation of the class OrderBookEntry
class OrderBookEntry
{
          public:
                    OrderBookEntry(double _price, double _amount, string _timeStamp,string _product, OrderBookType _orderType,string username = "dataset");
                    static OrderBookType stringToOrderBookType(const string &str);
                              
                    static bool timestampCompare(OrderBookEntry& e1, OrderBookEntry& e2)
                    {
                              
                                        return e1.timestamp < e2.timestamp;
                              
                    };
                    static bool compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2)
                    {
                              return e1.price < e2.price;
                    };
                    static bool compareByPriceDesc(OrderBookEntry &e1, OrderBookEntry &e2)
                    {
                              return e1.price > e2.price;
                    };
                    double price;
                    double amount;
                    string timestamp;
                    string username;
                    string product;
                    OrderBookType orderType;
                    
         
};
