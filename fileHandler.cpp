#include <iostream>
#include "fileHandler.h"
#include "jsonParser.h"

using namespace std;

void fileHandler::history() {
    jsonReadSchedule();
}

void fileHandler::checkSchedule(std::string &v) {
    jsonGetAllInfo(v);
}

void fileHandler::addSchedule(std::string protocolp, std::string hostnamep, std::string filenamep,std::string date, std::string timep,
                         std::string priorityp, std::string usernamep, std::string passwordp) {
    jsonWrite( protocolp, hostnamep, filenamep,date, timep, priorityp, usernamep, passwordp);
}

void fileHandler::addToHistory(std::string protocolp, std::string hostnamep, std::string filenamep, std::string priorityp, std::string usernamep, std::string passwordp) {
    addObjectToHistory(protocolp, hostnamep, filenamep, priorityp, usernamep, passwordp);
}
