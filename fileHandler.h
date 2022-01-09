#ifndef SEMESTRALKA_FILEHANDLER_H
#define SEMESTRALKA_FILEHANDLER_H
#include <iostream>

#include "jsonParser.h"

class fileHandler {
    public: void history();
            void addSchedule(std::string protocolp, std::string hostnamep, std::string filenamep,std::string date, std::string timep,
                             std::string priorityp, std::string usernamep, std::string passwordp);
            void checkSchedule(std::string &v);
            void addToHistory(std::string protocolp, std::string hostnamep, std::string filenamep, std::string priorityp, std::string usernamep, std::string passwordp);
};

#endif //SEMESTRALKA_FILEHANDLER_H
