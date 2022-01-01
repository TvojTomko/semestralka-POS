//
// Created by looper on 31. 12. 2021.
//

#include "downloadHandler.h"

downloadHandler::downloadHandler() {
    downloading = 0;
    maxDownloading = 4;
}

void downloadHandler::startDownloading(int i) {
    std::jthread th(&download::startDownload, downloads.at(i));
}

void downloadHandler::pauseDownloading(int i) {
    downloads.at(i)->pauseDownload();
}

void downloadHandler::resumeDownloading(int i) {
    std::jthread th(&download::resumeDownload, downloads.at(i));
    th.detach();
}

void downloadHandler::stopDownloading(int i) {
    downloads.at(i)->stopDownload();
}

void downloadHandler::addDownload(download *d) {
    downloads.push_back(d);
}

//Producent vzdy najde 1 download s najvyssiou prioritou ktory sa este nestahuje
//Konzument ked Producent najde download s najvyssiou prioritou skontroluje ci je vyssia ako aktualnhe stahovane ak ano tak pausne stahovanie s najnizsiou prioritou a zacne stahovat dany subor
void downloadHandler::manageDownloadings() {
    std::jthread th(&downloadHandler::produce, this);
    th.detach();
    std::jthread th1(&downloadHandler::consume, this);
    th1.detach();
    sConsume.release();
}


void downloadHandler::produce() {
    while (!exit) {
        bool isAvailable = false;
        sConsume.acquire();
        int index;
        while (!isAvailable) {
            index = findHighestNotDownloading();
            if (index >= 0) {
                isAvailable = true;
            }
        }
        data = index;
        sProduce.release();
    }
}

void downloadHandler::consume() {
    while (!exit) {
        sProduce.acquire();
        int lowest = findLowestDownloading();
        findNumberOfDownloading();
        if (!std::binary_search(downloadingIndex.begin(), downloadingIndex.end(), data)) {
            if (downloading < maxDownloading && !downloads.at(data)->isDownloading()) {
                //free slots
                std::jthread th(&download::startDownload, downloads.at(data));
                th.detach();
                //  std::cout << "somConsume stahujem " << data << std::endl;
                downloadingIndex.push_back(data);
            } else if (lowest != -1 && downloads.at(lowest)->getPriority() < downloads.at(data)->getPriority() &&
                       downloading < maxDownloading) {
                //higher priority
                downloads.at(lowest)->pauseDownload();
                downloadingIndex.push_back(data);
                downloadingIndex.erase(std::remove(downloadingIndex.begin(), downloadingIndex.end(), lowest),
                                       downloadingIndex.end());
                std::cout << "somConsume zastavujem  " << lowest << std::endl;
                std::cout << "somConsume stahujem " << data << std::endl;
                std::jthread th(&download::startDownload, downloads.at(data));
                th.detach();
            }
        }
        data = -1;
        sConsume.release();
    }
}

void downloadHandler::findNumberOfDownloading() {
    int temp = 0;
    for (int i = 0; i < downloads.size(); i++) {
        if (downloads.at(i)->isDownloading()) {
            temp++;
        }
    }
    downloading = temp;
}

int downloadHandler::findLowestDownloading() {
    int lowest = INT_MAX;
    int index = -1;
    for (int i = 0; i < downloads.size(); i++) {
        if (downloads.at(i)->getPriority() < lowest && downloads.at(i)->isDownloading()) {
            index = i;
            lowest = downloads.at(i)->getPriority();
        }
    }
    return index;
}

int downloadHandler::findHighestNotDownloading() {
    int highest = -1;
    int index = -1;
    for (int i = 0; i < downloads.size(); i++) {
        if (downloads.at(i)->getPriority() > highest && !downloads.at(i)->isDownloading() &&
            downloads.at(i)->getMsg() != "Yes") {
            index = i;
            highest = downloads.at(i)->getPriority();
        }
    }
    return index;
}

void downloadHandler::listOfDownloads() {
    int i = 0;
    std::cout << std::setprecision(3);
    std::cout << "Number " << "Protocol " << "Hostname " << "Filename " << "LocalFileName " << "LocalPath "
              << "Finished " << "Progress " << "Size " << "Priority " << std::endl;
    while (i < downloads.size()) {
        std::cout << i << ". " << downloads.at(i)->getAProtocol() << " " << downloads.at(i)->getHostname() << " "
                  << downloads.at(i)->getFilename() << " " << downloads.at(i)->getLocalFilename() << " ";
        if (downloads.at(i)->getPath().empty()) {
            std::cout << "default ";
        } else {
            std::cout << downloads.at(i)->getFilepath() << " ";
        }
        std::cout << downloads.at(i)->getMsg() << " " << downloads.at(i)->progress() << "% "
                  << downloads.at(i)->getSize() << " " << downloads.at(i)->getPriority() << std::endl;
        i++;
    }
}

void downloadHandler::startAll() {
    for (int i = 0; i < downloads.size(); ++i) {
        std::jthread th(&download::startDownload, downloads.at(i));
        th.detach();
    }
}

void downloadHandler::pauseAll() {
    for (int i = 0; i < downloads.size(); ++i) {
        downloads.at(i)->pauseDownload();
    }
}

void downloadHandler::exitProgram() {
    exit = true;
}

void downloadHandler::setPriority(int d, int p) {
    downloads.at(d)->setPriority(p);
}
