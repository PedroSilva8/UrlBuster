#ifndef _DEBUG_
#define _DEBUG_

#include <iostream>
#include <string>
#include <shared_mutex>

using namespace std;

class Debug {
public:
    static shared_mutex lockWrite;

    static void SafePrintFlush(const char* Message);
    static void SafePrintFlush(string Message);
    static void SafePrint(const char* Message);
    static void SafePrint(string Message);
    static void Log(const char* Message);
    static void Log(string Message);
    static void Alert(const char* Message);
    static void Alert(string Message);
    static void Error(const char* Message);
    static void Error(string Message);
};

#endif