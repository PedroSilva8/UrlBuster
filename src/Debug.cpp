#include "Debug.hpp"

shared_mutex Debug::lockWrite;

void Debug::SafePrintFlush(const char* Message) {
    lockWrite.lock();
    cout << Message << flush;
    lockWrite.unlock();
}

void Debug::SafePrintFlush(string Message) {
    SafePrintFlush(Message.data());
}

void Debug::SafePrint(const char* Message) {
    lockWrite.lock();
    cout << Message << endl;
    lockWrite.unlock();
}

void Debug::SafePrint(string Message) {
    SafePrint(Message.data());
}

void Debug::Log(const char* Message) {
    cout << "\033[32m[Log] \033[0m" << Message << endl;
}

void Debug::Log(string Message) {
    Log(Message.c_str());
}

void Debug::Alert(const char* Message) {

    cout << "\033[33m[Alert] \033[0m" << Message << endl;
}

void Debug::Alert(string Message) {
    Alert(Message.c_str());
}

void Debug::Error(const char* Message) {

    cout << "\033[31m[Error] \033[0m" << Message << endl;
    throw runtime_error("\n\033[31m[Error] \033[0m" + (string)Message);
}

void Debug::Error(string Message) {
    Error(Message.c_str());
}