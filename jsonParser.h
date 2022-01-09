#ifndef SEMESTRALKA_JSONPARSER_H
#define SEMESTRALKA_JSONPARSER_H

void jsonReadSchedule();
void jsonReadHistory();
void jsonWrite(std::string hostnamep, std::string filenamep, std::string localfilenamep, std::string pathp, std::string sizep, std::string downloadedp, std::string protocolp, std::string priorityp, std::string timep);
void jsonGetAllInfo();
void addToHistory(std::string fn);
//
#endif //SEMESTRALKA_JSONPARSER_H
