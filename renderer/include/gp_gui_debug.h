#ifndef _GRIDPRO_API_DEBUG_FUNCTIONS_HPP_
#define _GRIDPRO_API_DEBUG_FUNCTIONS_HPP_

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <atomic>

//#define GRIDPRO_API_DEBUG_PROFILE_ENABLE

namespace gridpro_gui
{
namespace _DEBUG_  
{
    static bool hlm_debug_switch = true;
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


#ifdef _MSVC_2015_FIX_

template <typename... Args>
inline void DEBUG_PRINT(const Args&... args)
{ 
  #ifdef GRIDPRO_API_DEBUG_PROFILE_ENABLE 
     if(_DEBUG_::hlm_debug_switch)
     {
     std::cout << "HELIUM_API DEBUG_PRINT : ";
     TuplePrinter<Args...>::print(args...);
     std::cout << "\n";
     }
  #endif      
}

template <typename... Args>
inline bool MILD_WARNING(const bool condition, const Args&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 5
  if(!condition) {
      std::cout << "HELIUM_API MILD_WARNING : " ; 
      TuplePrinter<Args...>::print(args...);
      std::cout << "\n";
      return false; }
  #endif
  #endif
  return true;
}


template <typename... Args>
inline bool MEDIUM_WARNING(const bool condition, const Args&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 4
  if(!condition) {
      std::cout << "HELIUM_API MEDIUM_WARNING : " ;
      TuplePrinter<Args...>::print(args...);
      std::cout << "\n";
      return false; }
  #endif
  #endif
  return true;
}


template <typename... Args>
inline bool STRICT_WARNING(const bool condition, const Args&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 3  
  if(!condition) { 
      std::cout << "HELIUM_API STRICT_WARNING : " ;
      TuplePrinter<Args...>::print(args...);
      std::cout << "\n";
      return false; }
  #endif
  #endif
  return true;
}


template <typename... Args>
inline bool MILD_ASSERT(const bool condition, const Args&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 2  
    if(!condition)
    { 
    std::cerr << "HELIUM_API MILD_ASSERT : ";
    TuplePrinter<Args...>::print(args...);
    std::cerr << "\n";
    printf("FAILED \n");
    }
    assert(condition);
  #endif
  #endif
  return true;
}


template <typename... Args>
inline bool MEDIUM_ASSERT(const bool condition, const Args&... args)
{ 
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 1  
    if(!condition)
    { 
     std::cerr << "HELIUM_API MEDIUM_ASSERT : ";
     //TuplePrinter<Args...>::print(args...);
     std::cerr << "\n";
     printf("FAILED \n");
    } 
    assert(condition);
  #endif
  #endif
  return true;
}



template <typename... Args>
inline bool STRICT_ASSERT(const bool condition, const Args&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED   // will be always shown if GRIDPRO_API_HAZARD_CHECK_LEVEL macro is defined
    if(!condition)
    { 
    std::cerr << "HELIUM_API STRICT_ASSERT : " ;
//    TuplePrinter<Args...>::print(args...);
    std::cerr << "\n";
    printf("FAILED \n");
    }
    assert(condition);
  #endif
  return true;
}

#else

// Helper function for expanding parameter pack
template<typename T>
void expand_args(T&& arg) {
    std::cout << std::forward<T>(arg) << " ";
}
/*
template <typename... Args>
constexpr inline bool DEBUG_PRINT(const std::string& message = "", Args&&... args) 
{
  #ifdef GRIDPRO_API_DEBUG_PROFILE_ENABLE 
      std::cout << "HELIUM_API DEBUG_PRINT : " << message  << " : ";
     (expand_args(std::forward<Args>(args)), ...);
      std::cout << "\n";
  #endif
  return true;
}
*/
template <typename... Args>
void DEBUG_PRINT(const Args&... args)
{ 
  #ifdef GRIDPRO_API_DEBUG_PROFILE_ENABLE 
    if(_DEBUG_::hlm_debug_switch)
     {
     std::cout << "GRIDPRO_API DEBUG_PRINT : ";
     TuplePrinter<Args...>::print(args...);
     std::cout << "\n";
     }
  #endif      
}

template <typename... Args>
constexpr inline bool MILD_WARNING(const bool condition, const std::string& message = "", Args&&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 5
  if(!condition) {
      std::cout << "HELIUM_API MILD_WARNING : " << message  << " : ";
     (expand_args(std::forward<Args>(args)), ...);
      std::cout << "\n";
      return false; }
  #endif
  #endif
  return true;
}

template <typename... Args>
constexpr inline bool MEDIUM_WARNING(const bool condition, const std::string& message = "" , Args&&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 4
  if(!condition) { 
      std::cout << "HELIUM_API MEDIUM_WARNING : " << message  << " : ";
      (expand_args(std::forward<Args>(args)), ...);
      std::cout << "\n"; 
      return false; }
  #endif
  #endif
  return true;
}

template <typename... Args>
constexpr inline bool STRICT_WARNING(const bool condition, const std::string& message = "" , Args&&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 3  
  if(!condition) { 
      std::cout << "HELIUM_API STRICT_WARNING : " << message  << " : ";
      (expand_args(std::forward<Args>(args)), ...);
      std::cout << "\n";
      return false; }
  #endif
  #endif
  return true;
}

template <typename... Args>
constexpr inline bool MILD_ASSERT(const bool condition, const std::string& message = "", Args&&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 2  
    if(!condition)
    { 
    std::cerr << "HELIUM_API MILD_ASSERT : " << message  << " : ";
   (expand_args(std::forward<Args>(args)), ...);
    std::cerr << "\n";
    printf("FAILED \n");
    }
    assert(condition);
  #endif
  #endif
  return true;
}

template <typename... Args>
constexpr inline bool MEDIUM_ASSERT(const bool condition, const std::string& message = "",  Args&&... args)
{ 
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED
  #if GRIDPRO_API_HAZARD_CHECK_LEVEL > 1  
    if(!condition)
    { 
    std::cerr << "HELIUM_API MEDIUM_ASSERT : " << message  << " : ";
    (expand_args(std::forward<Args>(args)), ...);
    std::cerr << "\n";
    printf("FAILED \n");
    } 
    assert(condition);
  #endif
  #endif
  return true;
}


template <typename... Args>
constexpr inline bool STRICT_ASSERT(const bool condition, const std::string& message = "",  Args&&... args)
{
  #ifdef GRIDPRO_API_HAZARD_CHECK_ENABLED   // will be always shown if GRIDPRO_API_HAZARD_CHECK_LEVEL macro is defined
    if(!condition)
    { 
    std::cerr << "HELIUM_API STRICT_ASSERT : " << message  << " : ";
    (expand_args(std::forward<Args>(args)), ...);
    std::cerr << "\n";
    printf("FAILED \n");
    }
    assert(condition);
  #endif
  return true;
}

#endif
} // namespace HELIUM_API
#endif // #define _GRIDPRO_API_DEBUG_FUNCTIONS_HPP_

