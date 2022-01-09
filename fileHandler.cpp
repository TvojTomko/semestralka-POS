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
fileHandler::addSchedule(std::string hostnamep, std::string filenamep, std::string localfilenamep, std::string pathp,
                         std::string sizep, std::string downloadedp, std::string protocolp, std::string priorityp,
                         std::string timep) {
    jsonWrite(hostnamep, filenamep, localfilenamep, pathp, sizep, downloadedp, protocolp, priorityp, timep);
}
