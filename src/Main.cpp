#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <curl/curl.h> 
#include <arpa/inet.h>

#include "Debug.hpp"

using namespace std;

enum URL_TYPE {
    NONE,
    IP,
    URL
};

//Global Variables
vector<string> dictionary;
string url = "";
URL_TYPE type = NONE;
int completedJobs;
shared_mutex jobIncreaserMutex;
shared_mutex lockWrite;

void worker(int startIndex, int length) {
    //Setup Curl
    CURL *c = curl_easy_init(); 

    if (!c)
        Debug::Error("Failed To Initialize CURL");

    struct curl_slist *headers = curl_slist_append(NULL, "Accept: *");
    string finalUrl;

    if (type == IP) {
        curl_easy_setopt(c, CURLOPT_RESOLVE, curl_slist_append(NULL, ("example.com:80:" + url).data()));
        finalUrl = "http://example.com";
    }
    else
        finalUrl = url;
    
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(c, CURLOPT_TIMEOUT, 4 * 1000);
    curl_easy_setopt(c, CURLOPT_NOBODY, 1);

    //Do Shit
    for (size_t i = startIndex; i < startIndex + length; i++) {
        //Set URL (NOTE: For some reasson setting url directly from const char* (when encondig) causes malformed URL, check later why and try to fix this)
        string encondedURL = finalUrl + "/" + curl_easy_escape(c, dictionary[i].data(), dictionary[i].length());
        curl_easy_setopt(c, CURLOPT_URL, encondedURL.data());
        
        //Perform Request
        CURLcode res = curl_easy_perform(c);

        //Check Response
        if (res == CURLE_OK) {
            int responseCode;
            curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &responseCode); 
            if (responseCode != 404) {
                lockWrite.lock();
                cout << "\33[2K\r" + dictionary[i] + " - " + to_string(responseCode) << endl;
                lockWrite.unlock();
            }
        }
        else {
            lockWrite.lock();
            cout << "\33[2K\rError: Stopped Awensering" << endl;
            lockWrite.unlock();
        }

        jobIncreaserMutex.lock();
        completedJobs++;
        jobIncreaserMutex.unlock();
    }    
}

int main(int ac, char *av[]) {

    //Get URL
    for(int i = 0; i < ac; i++) {
        if ((string)av[i] == "--url" && i + 1 < ac) {
            url = av[++i];
        }
    }

    if (url == "")
        Debug::Error("Missing URL Argument");
    
    //Verify URL
    //TODO: Above
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, url.data(), &(sa.sin_addr)) == 1)
        type = IP;
    else if (regex_match(url, regex("^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$")))
        type = URL;

    if (type == NONE)
        Debug::Error("Invalid URL");

    //Get Dic
    string l;
    
    ifstream file("/home/autotechnocract/Code/Scripts/UrlBuster/build/UrlBuster.data");
    
    if(!file.is_open())
        Debug::Error(strerror(errno));

    while (getline(file, l))
        dictionary.push_back(l);

    //Do Jobs
    cout << "Starting Check" << endl;

    int Cores = 16;
    int Length = dictionary.size() / Cores;

    vector<thread> ThreadVector;
    for (int i = 0; i < Cores; i++)
        ThreadVector.emplace_back(thread(worker, i * Length, Length));

    while (completedJobs != dictionary.size()) {
        lockWrite.lock();
        cout << "\r[" + to_string(completedJobs) + "/" + to_string(dictionary.size()) + "]" << flush;
        lockWrite.unlock();
    }
    return 0;
}