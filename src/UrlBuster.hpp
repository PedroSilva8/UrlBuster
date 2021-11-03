#ifndef _URL_BUSTER_
#define _URL_BUSTER_

#include <string>
#include <regex>
#include <shared_mutex>
#include <thread>
#include <iostream>
#include <fstream>  
#include <chrono>

#include <arpa/inet.h>
#include <curl/curl.h> 

using namespace std;

enum URL_TYPE {
    NONE,
    IP,
    URL
};

struct UrlStatus {
    string url;
    int code;
};

struct WorkerData {
    unsigned int index;
    unsigned int length;
};

class UrlBuster {
public:
    static string url;
    static string output;
    static string outputType;

    static int waitTime;

    static URL_TYPE type;

    static vector<string> dictionary;
    static vector<thread> threads;
    static vector<UrlStatus> status;

    static unsigned int threadSize;
    static long unsigned int completedJobs;

    static shared_mutex jobIncreaserMutex;
    static shared_mutex addStatusMutex;

    static void Setup();
    static void Start();
    static void Worker(WorkerData data);
    static void PrintHelp();
};

#endif