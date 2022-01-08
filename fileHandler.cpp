#include <iostream>
#include "fileHandler.h"
#include "jsonParser.h"

using namespace std;

void fileHandler::history() {
    jsonReadAll();
}

void fileHandler::checkSchedule() {
    jsonGetAllInfo("file1");
}

void fileHandler::addSchedule() {
    jsonWrite("hname", "fname", "lfname", "path/path", "123", "100", "http", "1", "12:00");
}
