#ifndef _URL_BUSTER_
#define _URL_BUSTER_

#include <string>
#include <regex>
#include <shared_mutex>
#include <thread>

#include <arpa/inet.h>
#include <curl/curl.h> 

using namespace std;

enum URL_TYPE {
    NONE,
    IP,
    URL
};

class UrlBuster {
public:
    static string url;
    static URL_TYPE type;
    static vector<string> dictionary;
    static vector<thread> threads;
    static unsigned int threadSize;
    
    static int completedJobs;
    static shared_mutex jobIncreaserMutex;

    static void Setup();
    static void Start();
    static void Worker(int startIndex, int length);
    static void PrintHelp();
};

#endif