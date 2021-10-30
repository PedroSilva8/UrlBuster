#include <iostream>
#include <fstream>

#include "Debug.hpp"
#include "Misc.hpp"
#include "ArgHandler.hpp"
#include "UrlBuster.hpp"

using namespace std;

int main(int ac, char **av) {

    //Setup ArgHandler
    string dictionaryPath = "";

    ArgHandler::argumentConverter = {
        { "u", "url" },
        { "d", "dictionary" },
        { "h", "help" },
        { "t", "thread" },
        { "o", "output" },
        { "ot", "output-type"},
        { "s", "slow-mode"}
    };

    ArgHandler::LoadArguments(ac, av);

    if (ArgHandler::GetArgument("help")) {
        UrlBuster::PrintHelp();
        return 0;
    }

    ArgHandler::GetArgument("output", &UrlBuster::output);
    ArgHandler::GetArgument("output-type", &UrlBuster::outputType);

    string slowMode;
    if (ArgHandler::GetArgument("slow-mode", &slowMode) && Misc::isNumber(slowMode))
        UrlBuster::waitTime = stoi(slowMode);

    //Get URL
    if (!ArgHandler::GetArgument("url", &UrlBuster::url))
        Debug::Error("Missing URL Argument");
    
    if (!ArgHandler::GetArgument("dictionary", &dictionaryPath))
        Debug::Error("Dictionary Missing");
    

    string threads;
    if (ArgHandler::GetArgument("thread", &threads) && Misc::isNumber(threads))
        UrlBuster::threadSize = stoi(threads);
    else
        UrlBuster::threadSize = thread::hardware_concurrency();

    UrlBuster::Setup();

    //Get Dic
    ifstream file(dictionaryPath);
    
    if(!file.is_open())
        Debug::Error(strerror(errno));
    
    string line;
    while (getline(file, line))
        UrlBuster::dictionary.push_back(line);

    //Do Jobs
    UrlBuster::Start();
    
    return 0;
}