#ifndef SEMESTRALKA_FILEHANDLER_H
#define SEMESTRALKA_FILEHANDLER_H

class fileHandler {
    public: void history();
            void addSchedule(std::string hostnamep, std::string filenamep, std::string localfilenamep, std::string pathp, std::string sizep, std::string downloadedp, std::string protocolp, std::string priorityp, std::string timep);
            void checkSchedule();
};

#endif //SEMESTRALKA_FILEHANDLER_H
