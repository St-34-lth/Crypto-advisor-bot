#include <iostream>
#include <vector>
#include <map>
#include "CSVReader.h"
#include "OrderBookEntry.h"
#pragma once
using namespace std;

class Wallet
{
          public:
                    Wallet(string _username);
                    Wallet();
                    void insertCurrency(string type,double amount);
                    bool containsCurrency(string type,double amount);
                    bool removeCurrency(string type,double amount);
                    bool canFulfillOrder(OrderBookEntry order);
                    string getUsername();
                    
                    //friend ostream& operator<<(ostream& os, Wallet& wallet);
                    /** update the contents of the wallet
                     * assumes order was made by the owner of the wallet
                      */
                    void processSale(OrderBookEntry& sale);
                    /**
                     * @brief 
                     * returns a string representation of the wallet contents
                     * @return string 
                     */
                    string toString();
          private:
                    /**
                     * @brief holds a key value pair representing the amount held per currency
                     * 
                     */
                    string username;  
                    map<string,double> currencies;
                   
                    


};