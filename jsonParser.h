#ifndef SEMESTRALKA_JSONPARSER_H
#define SEMESTRALKA_JSONPARSER_H

void jsonReadSchedule();
void jsonReadHistory();
void jsonWrite(std::string protocolp, std::string hostnamep, std::string filenamep, std::string timep,
               std::string priorityp, std::string usernamep, std::string passwordp);
void jsonGetAllInfo();
void addToHistory(std::string fn);
//
#endif //SEMESTRALKA_JSONPARSER_H
