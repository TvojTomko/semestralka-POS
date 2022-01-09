#ifndef SEMESTRALKA_JSONPARSER_H
#define SEMESTRALKA_JSONPARSER_H
#include <json-c/json.h>
#include <iostream>
#include <chrono>
#include <time.h>
#include <sstream>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string
#include <vector>



void jsonReadSchedule();
void jsonReadHistory();
void jsonWrite(std::string protocolp, std::string hostnamep, std::string filenamep,std::string date, std::string timep,
               std::string priorityp, std::string usernamep, std::string passwordp);
void jsonGetAllInfo(std::string &v);
void addToHistory(std::string fn);

void addObjectToHistory(std::string protocolp, std::string hostnamep, std::string filenamep, std::string priorityp, std::string usernamep, std::string passwordp);

//
#endif //SEMESTRALKA_JSONPARSER_H
