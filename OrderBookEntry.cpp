#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"

using namespace std;

OrderBookEntry::OrderBookEntry(double _price, double _amount, string _timestamp, string _product, OrderBookType _orderType,string _username)
          
          // class member initialization list
          :price(_price),
          amount(_amount),
          timestamp(_timestamp),
          product(_product),
          orderType(_orderType),
          username(_username)
          {};
          
OrderBookType OrderBookEntry::stringToOrderBookType(const string& str)
{
          if(str=="ask" || str=="ASK")
          {
                    return OrderBookType::ask;
          }
          else if(str=="bid" || str=="BID")
          {
                    return OrderBookType::bid;
          }
          return OrderBookType::unknown;
          
}


