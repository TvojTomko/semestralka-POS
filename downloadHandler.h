//
// Created by looper on 31. 12. 2021.
//
#include "download.h"
#include "jsonParser.h"
#include <semaphore>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>


#ifndef SEMESTRALKA_POS_DOWNLOADHANDLER_H
#define SEMESTRALKA_POS_DOWNLOADHANDLER_H


class downloadHandler {
private:
    std::vector<download *> downloads;
    int downloading;
    int maxDownloading;
    bool exit;
    std::counting_semaphore<1> sProduce{0};
    std::counting_semaphore<1> sConsume{0};
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
    int data;
    std::vector<int> downloadingIndex;
    bool manage;


public:
    downloadHandler();

    void deleteAll();

    void startDownloading(int i);

    void stopDownloading(int i);

    void pauseDownloading(int i);

    void resumeDownloading(int i);

    void addDownload(download *d);

    void manageDownloadings();

    void listOfDownloads();

    void startAll();

    void pauseAll();

    void stopAll();

    void resumeAll();

    void exitProgram();

    void produce();

    void consume();

    int findLowestDownloading();

    void findNumberOfDownloading();

    int findHighestNotDownloading();

    void setPriority(int d, int p);

    int getNumberOdDownloads();

    bool isManage() const;

    void history();

    void schedule();

    void plan(std::string protocolp, std::string hostnamep, std::string filenamep,std::string date, std::string timep,
              std::string priorityp, std::string usernamep, std::string passwordp);

    void checkSchedule();
};


#endif //SEMESTRALKA_POS_DOWNLOADHANDLER_H
