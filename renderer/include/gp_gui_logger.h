
#ifndef GRIDPRO_DEBUG_MODULE_HPP
#define GRIDPRO_DEBUG_MODULE_HPP

#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>

#include "nlohmann/json.hpp"


namespace gridpro_debug {

namespace category  
{
    static std::string file_path;
    static std::unordered_map<std::string, bool> debug_switches;
}

static void init_flags(const std::string& file_path)
{
   try
   {
    if(file_path.empty())
    {
        throw std::runtime_error("File Path is empty");
    }

    gridpro_debug::category::file_path = file_path;   
    std::ifstream inputFile(gridpro_debug::category::file_path);    
    nlohmann::json readJson;
    inputFile >> readJson;
    inputFile.close();

    // Deserialize the JSON object to another unordered_map
    category::debug_switches = readJson.get<std::unordered_map<std::string, bool>>();

   }
   catch(const std::exception& e)
   {
       //std::cerr << "Error : " << e.what() << std::endl;
   }
}

template <typename... Args>
struct TuplePrinter;

template <typename Arg>
struct TuplePrinter<Arg>
{
  inline static void print(const Arg& arg)
    {
      std::cout << arg << " ";
    }
};

template <typename Arg, typename... Args>
struct TuplePrinter<Arg, Args...>
{
 inline static void print(const Arg& arg, const Args&... args)
    {
        std::cout << arg << " ";
        TuplePrinter<Args...>::print(args...);
    }
};


template <typename... Args>
inline void log(const std::string& log_category, const Args&... args)
{ 
     if(category::debug_switches[log_category] == true)
     {
       std::cout << "GRIDPRO_LOGGER : Category : " << log_category << " : ";
       TuplePrinter<Args...>::print(args...);
       std::cout << "\n";
     }    
}


} // namespace gridpro_debug

#endif // GRIDPRO_DEBUG_MODULE_HPP