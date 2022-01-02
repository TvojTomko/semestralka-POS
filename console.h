//
// Created by looper on 30. 12. 2021.
//
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "download.h"
#include "downloadHandler.h"

#ifndef SEMESTRALKA_POS_CONSOLE_H
#define SEMESTRALKA_POS_CONSOLE_H


class console {
private:
    std::string pathtoDownload;
    std::vector<std::string> bashCommands{"ls",
                                          "mkdir",
                                          "rmdir",
                                          "rm",
                                          "cd"
    };
    std::vector<std::string> customCommands{"download",
                                            "history",
                                            "pause",
                                            "resume",
                                            "stop",
                                            "setpath",
                                            "help",
                                            "exit",
                                            "info",
                                            "startAll",
                                            "pauseAll",
                                            "stopAll",
                                            "manage",
                                            "setPriority"
    };
    std::string space_delimiter = " ";
public:
    void listener();
    //void checkFinishedDownloads();

    const std::string &getPathtoDownload() const;

    void setPathtoDownload(const std::string &pathtoDownload);

    void info();

    void exitt(bool &exit);

    int resolveCommand(std::string input);

    void makeConsoleCommand(std::string &command, std::string &input);
};


#endif //SEMESTRALKA_POS_CONSOLE_H
