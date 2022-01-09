#include <iostream>
#include "fileHandler.h"
#include "jsonParser.h"

using namespace std;

void fileHandler::history() {
    jsonReadSchedule();
}

void fileHandler::checkSchedule() {
    jsonGetAllInfo();
}

void
fileHandler::addSchedule(std::string protocolp, std::string hostnamep, std::string filenamep, std::string timep,
                         std::string priorityp, std::string usernamep, std::string passwordp) {
    jsonWrite( protocolp, hostnamep, filenamep, timep, priorityp, usernamep, passwordp);
}
