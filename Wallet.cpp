#include "Wallet.h"
using namespace std;
//constructor
Wallet::Wallet(string _username)
{
    
    username = _username;
    
};

Wallet::Wallet()
{};
string Wallet::getUsername()
{
          return this->username; 
}
void Wallet::insertCurrency(string type, double amount)
{
          double balance;
          if (amount <0)
          {
                    throw exception{};
          };

          if(currencies.count(type)==0)
          {
                    balance = 0 ;
          }
          else 
          {
                    balance = currencies[type];
          };
          balance += amount;
          // cout << balance << endl;
          currencies[type] = balance;  
};
bool Wallet::removeCurrency(string type, double amount)
{
          double balance; 
          if(amount < 0)
          {
                    throw exception{};
          }
          if (currencies.count(type)==0)
          {
                    cout << "NO currency for " << type << endl; 
                    return false;
          }
          else{ 
                    if(containsCurrency(type,amount))
                    {
                              cout << "Removing " << type << ": " << amount << endl;

                              currencies[type] -= amount;
                              return true;
                    }
                    else{
                              return false;
                    };
                    
          };
          
};
bool Wallet::canFulfillOrder(OrderBookEntry order)
{
          vector<string> currencyPairs = CSVReader::tokenize(order.product, '/');

          
          if(order.orderType== OrderBookType::ask)
          {
                    double amount = order.amount * order.price;
                    string currency = currencyPairs[0] ;
                    return containsCurrency(currency,amount);
          };
          //checks if the bid currency exists
          if(order.orderType==OrderBookType::bid)
          {
                    double amount = order.amount;
                    string currency = currencyPairs[1];
                    return containsCurrency(currency,amount);
          }
          return false;
};
bool Wallet::containsCurrency(string type, double amount)
{
          if(currencies.count(type)==0)
          
                    return false;
          
          else           
                    return currencies[type] >= amount;
};
string Wallet::toString()
{
          string message; 
          for(pair<string,double> pair : currencies)
          {
                    string currency = pair.first;
                    double amount = pair.second;
                    message+=currency + " : " + to_string(amount) + "\n";

          };
          return message;
};
ostream &operator<<(ostream &os, Wallet &wallet)
{
          os << wallet.toString();
          return os;
};
void Wallet::processSale(OrderBookEntry& sale)
{
          vector<string> currencyPairs = CSVReader::tokenize(sale.product, '/');
         


          if (sale.orderType == OrderBookType::asksale)
          {
                    double outgoingAmount = sale.amount;
                    string outgoingCurrency = currencyPairs[0];
                    double incomingAmount = sale.amount * sale.price;
                    string incomingCurrency = currencyPairs[1];

                    currencies[incomingCurrency] += incomingAmount;
                    currencies[outgoingCurrency] -= outgoingAmount;
          };
          
          if (sale.orderType == OrderBookType::bidsale)
          {

                    double incomingAmount = sale.amount;
                    string incomingCurrency = currencyPairs[0];
                    double outgoingAmount = sale.amount * sale.price;
                    string outgoingCurrency = currencyPairs[1];

                    currencies[incomingCurrency] += incomingAmount;
                    currencies[outgoingCurrency] -= outgoingAmount;
          }
          
};