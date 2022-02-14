#include <iostream>
#include <map>
#include <string>
#include <vector>


#include "test_bot.h"
#include "TradeInterface.h"
#include "CSVReader.h"
#include "tradeit-io.h"
#include "Wallet.h"

using namespace std;

TradeIt app_ {};
TradeInterface _tradingInterface(app_); 
TestBot bot(_tradingInterface);  
int main()
{
         
          bot.init("bot");
          bot.run();
          return 0;
};
