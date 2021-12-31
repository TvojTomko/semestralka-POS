//
// Created by looper on 31. 12. 2021.
//
#include "download.h"
#ifndef SEMESTRALKA_POS_DOWNLOADHANDLER_H
#define SEMESTRALKA_POS_DOWNLOADHANDLER_H


class downloadHandler {
private:
    std::vector<download*>downloads;
    int downloading;
    int maxDownloading;

public:
    downloadHandler();
    void startDownloading(int i);
    void stopDownloading(int i);
    void pauseDownloading(int i);
    void resumeDownloading(int i);
    void addDownload(download* d);
    void manageDownloadings();
    void listOfDownloads();
    void startAll();
    void pauseAll();
};


#endif //SEMESTRALKA_POS_DOWNLOADHANDLER_H
