#include "ArgHandler.hpp"

map<string, string> ArgHandler::arguments = map<string, string>();
map<string, string> ArgHandler::argumentConverter = map<string, string>();

bool ArgHandler::GetArgument(string Argument, string *value) {
    if (arguments.contains(Argument)) {
        if (value != nullptr)
            *value = arguments[Argument];
        return true;
    }
    return false;
}

void ArgHandler::LoadArguments(int length, char** args) {
    for (int i = 0; i < length; i++) {
        string arg = args[i];
        if (arg[0] == '-') {
            string subArg = arg.substr(arg[1] == '-' ? 2 : 1, arg.size() - arg[1] == '-' ? 2 : 1);
            arguments.insert(pair<string, string>(subArg.size() != 1 ? subArg : argumentConverter[subArg], i + 1 < length && args[i + 1][0] != '-' ? args[++i] : ""));
        }
    }
}