#ifndef _ARG_HADNDLER_
#define _ARG_HADNDLER_

#include <string>
#include <map>

using namespace std;

class ArgHandler {
public:
    //Maybe Converter is not a good name
    static map<string, string> argumentConverter;
    static map<string, string> arguments;
    
    static bool GetArgument(string Argument, string *value = nullptr);
    static void LoadArguments(int length, char** args);
};

#endif