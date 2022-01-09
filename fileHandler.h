#ifndef SEMESTRALKA_FILEHANDLER_H
#define SEMESTRALKA_FILEHANDLER_H

class fileHandler {
    public: void history();
            void addSchedule(std::string protocolp, std::string hostnamep, std::string filenamep, std::string timep,
                             std::string priorityp, std::string usernamep, std::string passwordp);
            void checkSchedule();
};

#endif //SEMESTRALKA_FILEHANDLER_H
