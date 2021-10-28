#ifndef _DEBUG_
#define _DEBUG_

#include <iostream>
#include <string>

using namespace std;

class Debug {
public:
    static void Log(const char* Message);
    static void Log(string Message);
    static void Alert(const char* Message);
    static void Alert(string Message);
    static void Error(const char* Message);
    static void Error(string Message);
};

#endif