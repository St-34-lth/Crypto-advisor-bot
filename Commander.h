using namespace std;
#include "test_bot.h"
#include <boost/any.hpp>


//* A console input parser class with static members. To be used with the advisor bot.
//* Each of its members is modular and can be used individually. The parser uses C++ queue and list containers.  
class Commander
{

          public:
                    Commander();
                    
                    //* tokenizes user input by using a regular expression and returns the tokenized line
                    static list<string> tokenize(
                        const string str,
                        const regex re)
                        {
                                  sregex_token_iterator it{
                                      str.begin(),
                                      str.end(), re, -1};
                                  list<string> tokenized{it, {}};
                                  try
                                  {
                                            
                                            //  removes empty strings
                                            tokenized.erase(
                                                remove_if(tokenized.begin(), tokenized.end(),
                                                          [](string const &s)
                                                          {
                                                                    return s.size() == 0;
                                                          }),
                                                tokenized.end());
                                                 
                                            return tokenized;
                                  }
                                  catch (exception &e)
                                  {
                                            cout << "command tokenizer error : "
                                                 << e.what() << '\n';
                                  };
                                  return tokenized;
                        }
                    //* Helper that checks if a string contains a number. Returns false if not, true otherwise.
                    static bool isNumber(const string& str)
                    {
                              try 
                              {
                                        return str.find_first_not_of("0123456789")== string::npos;
                              } catch(exception & e)
                              {
                                        cout << "Error with string to number conversion" << e.what() << '\n';
                              }
                    }
                    // *Helper that converts string characters to upper case then returns it
                    static string toUpperCase(string line)
                    {
                              try 
                              {
                                        transform(line.begin(), line.end(), line.begin(),
                                                  [](unsigned char c) -> unsigned char
                                                  { return std::toupper(c); });
                                        return line;
                              }
                              catch (exception& e)
                              {
                                        cout << "Error with uppercase conversion" << e.what() << '\n';
                              };
                    }
                    //* translates strings to the Command enum class 
                    static Commands stringToCommand(string &str)
                    {
     
                              try {
                                        str = toUpperCase(str);
                                        if (str == "HELP")
                                        {
                                                  
                                                  return Commands::help;
                                        };
                                        if (str == "AVG")
                                        {

                                                  return Commands::avg;
                                        }
                                        if (str == "PROD")
                                        {
                                                  return Commands::prod;
                                        };
                                        if (str=="FRL")
                                        {
                                                  return Commands::frl;
                                        }
                                        if (str == "MIN")
                                        {

                                                  return Commands::min;
                                        }
                                        if (str == "MAX")
                                        {

                                                  return Commands::max;
                                        };
                                        if (str == "PREDICT")
                                        {
                                                  return Commands::predict;
                                        }
                                        if (str == "TIME")
                                        {
                                                  return Commands::marketTime;
                                        };
                                        if (str == "STEP")
                                        {
                                                  return Commands::step;
                                        };
                                       
                              } 
                              catch (exception& e)
                              {
                                        cout << "Error with converting the string to a command." << e.what() << '\n';
                              };
                              return Commands::invalid;
                    }
                    
                    //* takes in a referenced list of strings with of parsed tokens and returns a valid commands queue if possible
                    static queue<Commands> tokensToCommands(list<string>& parsedTokens)
                    {
                             
                              queue<Commands> commandQ;
                              if(parsedTokens.size()< 10)
                              {
                                        try{
                                                  if (!parsedTokens.empty())
                                                  {
                                                        
                                                            string firstToken = parsedTokens.front();
                                                            Commands firstCommand = stringToCommand(firstToken);
                                                            commandQ.push(firstCommand);
                                                            parsedTokens.pop_front();
                                                            list<string> _parsedTokens ;
                                                            copy(parsedTokens.begin(),parsedTokens.end(),back_inserter(_parsedTokens));
                                                            // 2. if the first command is valid continue the loop
                                                            if (firstCommand != Commands::invalid && !_parsedTokens.empty())
                                                            {

                                                                      for (auto &token : _parsedTokens)
                                                                      {         
                                                                                 
                                                                      
                                                                                          // 3. If the next token is a number or invalid,  ignore it
                                                                                         
                                                                                          Commands command = stringToCommand(token);
                                                                                          if (!isNumber(token) && command != Commands::invalid)
                                                                                          {
                                                                                                    
                                                                                                    // 4. check whether it is the help or predict command
                                                                                                    if (firstCommand == Commands::help)
                                                                                                    {
                                                                                                              // 5. if it is the help command, there could be another valid command next in line
                                                                                                              commandQ.push(command);
                                                                                                              parsedTokens.pop_front();
                                                                                                    };

                                                                                                    if (firstCommand == Commands::predict)
                                                                                                    {
                                                                                                              break;
                                                                                                    };
                                                                                                    
                                                                                          }
                                                                                          else
                                                                                          {
                                                                                                    continue;
                                                                                          };
                                                                      };
                                                                                
                                                            };
                                                            
                                                  }; 
                                        } catch(exception& e)
                                        {
                                                  cout <<"Error with converting the token to command." << e.what() << '\n';
                                        };
                              };

                              return commandQ;
                             
                           
                    };

                     //* takes in a referenced list of strings of parsed tokens and returns a queue of arguments if possible
                    static queue<string> tokensToArguments(list<string> &parsedTokens)
                    {
                              
                              queue<string> argumentsQ;
                              
                              
                              if (!parsedTokens.empty()&&parsedTokens.size()<10)
                              {
                                        
                                        try
                                        {
                                                
                                                  if (!parsedTokens.empty())
                                                  {
                                                            for (auto &token : parsedTokens)

                                                            {
                                                                      
                                                                      token = toUpperCase(token);
                                                                     
                                                                      argumentsQ.push(token);
                                                                      
                                                            };
                                                  };
                                        }
                                        catch (exception &e)
                                        {
                                                  cout <<"Error with converting the tokens to argument."<< e.what() << '\n';
                                        };
                              };
                              return argumentsQ;
                    };

};



