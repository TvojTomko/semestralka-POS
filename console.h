//
// Created by looper on 30. 12. 2021.
//
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include "download.h"
#include "downloadHandler.h"

#ifndef SEMESTRALKA_POS_CONSOLE_H
#define SEMESTRALKA_POS_CONSOLE_H


class console {
private:
    std::string pathToDownload;
    std::vector<std::string> bashCommands{"ls",
                                          "mkdir",
                                          "rmdir",
                                          "rm",
                                          "cd"
    };
    std::vector<std::string> customCommands{"download",
                                            "history",
                                            "start",
                                            "pause",
                                            "resume",
                                            "stop",
                                            "setPath",
                                            "help",
                                            "exit",
                                            "info",
                                            "startAll",
                                            "pauseAll",
                                            "stopAll",
                                            "manage",
                                            "setPriority",
                                            "plan",
                                            "schedule"
    };
    std::vector<std::string> protocols{"http",
                                       "https",
                                       "ftp",
                                       "ftps"
    };
    std::string space_delimiter = " ";
    downloadHandler *dH;
    bool exit;
public:
    void listener();

    void info();

    void exitProgram();

    int resolveCommand(std::string input);

    void makeConsoleCommand(std::string &input);

    bool checkInput(std::vector<std::string> &v);

    bool separateCommand(std::vector<std::string> &v, std::string &command);

    bool is_number(const std::string &s);
};


#endif //SEMESTRALKA_POS_CONSOLE_H
