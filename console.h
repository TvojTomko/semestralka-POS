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

#ifndef SEMESTRALKA_POS_CONSOLE_H
#define SEMESTRALKA_POS_CONSOLE_H


class console {
private:
    std::vector<download*> downloads;
    std::string pathtoDownload;
public:
    void listener();
    void checkFinishedDownloads();

    const std::string &getPathtoDownload() const;

    void setPathtoDownload(const std::string &pathtoDownload);
};


#endif //SEMESTRALKA_POS_CONSOLE_H
