#include "ArgHandler.hpp"

map<string, string> ArgHandler::arguments = map<string, string>();
map<string, string> ArgHandler::argumentConverter = map<string, string>();

bool ArgHandler::GetArgument(string Argument, string &value) {
    if (arguments.contains(Argument)) {
        value = arguments[Argument];
        return true;
    }
    return false;
}

void ArgHandler::LoadArguments(int length, char** args) {
    for (int i = 0; i < length; i++) {
        string arg = args[i];
        if (arg.rfind("--") == 0 && i + 1 < length)
            arguments.insert(pair<string, string>(arg.substr(2, arg.size() - 2), args[++i]));
        else if (arg.rfind("-") == 0 && i + 1 < length && argumentConverter.contains(arg.substr(1, arg.size() - 1)))
            arguments.insert(pair<string, string>(argumentConverter[arg.substr(1, arg.size() - 1)], args[++i]));
    }
}