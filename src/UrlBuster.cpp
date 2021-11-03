#include "UrlBuster.hpp"
#include "Debug.hpp"

string UrlBuster::url = "";
string UrlBuster::output;
string UrlBuster::outputType;
URL_TYPE UrlBuster::type = NONE;

int UrlBuster::waitTime;

unsigned int UrlBuster::threadSize;
long unsigned int UrlBuster::completedJobs;

vector<string> UrlBuster::dictionary;
vector<thread> UrlBuster::threads;
vector<UrlStatus> UrlBuster::status;

shared_mutex UrlBuster::jobIncreaserMutex;
shared_mutex UrlBuster::addStatusMutex;

void UrlBuster::Setup() {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, url.data(), &(sa.sin_addr)) == 1)
        type = IP;
    else if (regex_match(url, regex("^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*\\/?$")))
        type = URL;

    if (type == NONE)
        Debug::Error("Invalid URL");
}

void UrlBuster::Start() {
    cout << "Starting Check" << endl;

    unsigned int Length = dictionary.size() / threadSize;

    for (unsigned int i = 0; i < threadSize; i++)
        threads.emplace_back(thread(Worker, (WorkerData){ index: i * Length, length: (i + 1 == threadSize ? (unsigned int)dictionary.size() - (i * Length) : Length) }));

    while (completedJobs != dictionary.size())
        Debug::SafePrintFlush("\r\33[0m[" + to_string(completedJobs) + "/" + to_string(dictionary.size()) + "]");

    //Double check
    Debug::SafePrintFlush("\r\33[0m[" + to_string(completedJobs) + "/" + to_string(dictionary.size()) + "]\n");

    for (unsigned int i = 0; i < threadSize; i++)
        threads[i].join();

    //Save output
    if (output != "") {
        //This can probably be optimized
        if (outputType == "1") {
            //Get All Status Codes
            vector<int> codes;
            for (UrlStatus stat : status)
                codes.push_back(stat.code);

            for (int code : codes) {
                ofstream outfile(output + to_string(code) + ".log");

                for (UrlStatus stat : status)
                    if (code == stat.code)
                        outfile << stat.url << std::endl;
        
                outfile.close();
            }
        }
        else {
            ofstream outfile(output + "output.log");

            for (UrlStatus stat : status)
                outfile << to_string(stat.code) + " - " + stat.url << std::endl;
        
            outfile.close();
        }
    }
}

void UrlBuster::Worker(WorkerData data) {
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

    int responseCode;
    
    //Make Request Checks
    for (size_t i = data.index; i < data.index + data.length; i++) {
        //Set URL (NOTE: For some reasson setting url directly from const char* (when encondig) causes malformed URL, check later why and try to fix this)
        string encondedURL = finalUrl + "/" + curl_easy_escape(c, dictionary[i].data(), dictionary[i].length());
        curl_easy_setopt(c, CURLOPT_URL, encondedURL.data());
        
        if (UrlBuster::waitTime != 0)
            this_thread::sleep_for(chrono::milliseconds(UrlBuster::waitTime));
        //Setting curl_easy_perform directly on if causes all response codes to be 404... why?
        CURLcode res = curl_easy_perform(c);
        
        //Check Response
        if (res == CURLE_OK) {
            curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &responseCode); 

            if (responseCode != 404) {
                if (responseCode == 200)
                    Debug::SafePrint("\33[2K\33[32m\r" + to_string(responseCode) + " - " + dictionary[i]);
                else
                    Debug::SafePrint("\33[2K\33[33m\r" + to_string(responseCode) + " - " + dictionary[i]);
                
                addStatusMutex.lock();
                status.push_back({ url: dictionary[i],  code: responseCode });
                addStatusMutex.unlock();
            }
        }
        else
            Debug::SafePrint("\33[2K\rError: Stopped Awensering");

        jobIncreaserMutex.lock();
        completedJobs++;
        jobIncreaserMutex.unlock();
    }    
}

void UrlBuster::PrintHelp() {
    printf("Usage:\n");
    printf("\tUrlBuster [options] -u <argument> -d <argument>\tcheck url using dictionary\n");
    printf("Options:\n");
    printf("\t-h, --help\t\t\t\t\tdisplay this help\n");
    printf("\t-u, --url\t\t\t\t\ttarget url\n");
    printf("\t-d, --dictionary\t\t\t\tdictionary used by the program\n");
    printf("\t-t, --thread\t\t\t\t\tdefine how many threads the program uses\n");
    printf("\t-o, --output\t\t\t\t\tdefine where to output log\n");
    printf("\t-ot, --output-type\t\t\t\tdefine output save type\n");
    printf("\t\t-ot 0\t\t\t\t\tsave all into single output file\n");
    printf("\t\t-ot 1\t\t\t\t\tsave all into multiple output files named by status code\n");
    printf("\t-s, --slow-mode <argument>\t\t\twait x miliseconds before next request\n");
}