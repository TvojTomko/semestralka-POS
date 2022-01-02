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
    th.detach();
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
            } else if (lowest != -1 && downloads.at(lowest)->getPriority() < downloads.at(data)->getPriority()) {
                //higher priority
                downloads.at(lowest)->pauseDownload();
                downloadingIndex.push_back(data);
                downloadingIndex.erase(std::remove(downloadingIndex.begin(), downloadingIndex.end(), lowest),
                                       downloadingIndex.end());
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
    for (auto d: downloads) {
        if (d->isDownloading()) {
            temp++;
        }
    }
    downloading = temp;
}

int downloadHandler::findLowestDownloading() {
    int lowest = INT_MAX;
    int index = -1;
    int i = 0;
    for (auto d: downloads) {
        if (d->getPriority() < lowest && d->isDownloading()) {
            index = i;
            lowest = d->getPriority();
        }
        i++;
    }
    return index;
}

int downloadHandler::findHighestNotDownloading() {
    int highest = -1;
    int index = -1;
    int i = 0;
    for (auto d: downloads) {
        if (d->getPriority() > highest &&
            !d->isDownloading() &&
            d->getMsg() != "Yes" &&
            d->getMsg() != "STOP") {
            index = i;
            highest = d->getPriority();

        }
        i++;
    }
    return index;
}

void downloadHandler::listOfDownloads() {
    int i = 0;
    std::cout << std::setprecision(3);
    std::cout << "Number " << "Protocol " << "Hostname " << "Filename " << "LocalFileName " << "LocalPath "
              << "Finished " << "Progress " << "Size " << "Priority " << std::endl;
    for (auto d: downloads) {
        std::cout << i << ". " << d->getAProtocol() << " " << d->getHostname() << " "
                  << d->getFilename() << " " << d->getLocalFilename() << " ";
        if (d->getPath().empty()) {
            std::cout << "default ";
        } else {
            std::cout << d->getFilepath() << " ";
        }
        std::cout << d->getMsg() << " " << d->progress() << "% "
                  << d->getSize() << " " << d->getPriority() << std::endl;
        i++;
    }
}

void downloadHandler::startAll() {
    for (auto d: downloads) {
        std::jthread th(&download::startDownload, d);
        th.detach();
    }
}

void downloadHandler::pauseAll() {
    for (auto d: downloads) {
        d->pauseDownload();
    }
}

void downloadHandler::exitProgram() {
    exit = true;
    deleteAll();
}

void downloadHandler::setPriority(int d, int p) {
    downloads.at(d)->setPriority(p);
}

void downloadHandler::deleteAll() {
    for (auto d: downloads) {
        delete d;
    }
    downloads.clear();
}

void downloadHandler::stopAll() {
    for (auto d: downloads) {
        d->stopDownload();
    }
}

void downloadHandler::resumeAll() {
    for (auto d: downloads) {
        if (d->getMsg() == "PAUSED") {
            d->resumeDownload();
        }
    }
}
