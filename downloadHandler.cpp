//
// Created by looper on 31. 12. 2021.
//

#include "downloadHandler.h"

downloadHandler::downloadHandler() {
    downloading = 0;
    maxDownloading = 2;
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
/*
//Producent vzdy najde 1 download s najvyssiou prioritou ktory sa este nestahuje
//Konzument ked Producent najde download s najvyssiou prioritou skontroluje ci je vyssia ako aktualnhe stahovane ak ano tak pausne stahovanie s najnizsiou prioritou a zacne stahovat dany subor
void downloadHandler::manageDownloadings() {
    std::jthread th(&downloadHandler::produce, this);
    th.detach();
    std::jthread th1(&downloadHandler::consume, this);
    th1.detach();
}*/

/*
void downloadHandler::produce() {
    while (!exit) {
        std::unique_lock<std::mutex> ul(mtx);
        data = findHighestNotDownloading();
        ready = true;
        ul.unlock();
        cv.notify_one();
        //ul.lock();
        std::cout << "somProduce" << std::endl;
        cv.wait(ul, [this]() { return ready == false; });
    }
}

void downloadHandler::consume() {
    while (!exit) {
        std::unique_lock<std::mutex> ul(mtx);
        cv.wait(ul, [this]() { return ready; });
        int lowest = findLowestDownloading();
        findNumberOfDownloading();
        if (lowest < 0 && data != -1) {
            //nobody is downloading
            std::jthread th(&download::startDownload, downloads.at(data));
            th.detach();
        } else if (downloading < maxDownloading && data != -1) {
            //free slots
            std::jthread th(&download::startDownload, downloads.at(data));
            th.detach();
        } else if (lowest != -1 && downloads.at(lowest)->getPriority() < downloads.at(data)->getPriority() &&
                   data != -1) {
            //higher priority
            downloads.at(lowest)->pauseDownload();
            std::jthread th(&download::startDownload, downloads.at(data));
            th.detach();
        }
        data = -1;
        ready = false;
        std::cout << "somConsume" << std::endl;
        ul.unlock();
        cv.notify_one();
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
        if (downloads.at(i)->getPriority() > highest && !downloads.at(i)->isDownloading()) {
            index = i;
            highest = downloads.at(i)->getPriority();
        }
    }
    return index;
}
*/
void downloadHandler::listOfDownloads() {
    int i = 0;
    std::cout << std::setprecision(3);
    std::cout << "Number " << "Protocol " << "Hostname " << "Filename " << "LocalFileName " << "LocalPath "
              << "Finished " << "Progress " << "Size " << std::endl;
    while (i < downloads.size()) {
        std::cout << i << ". " << downloads.at(i)->getAProtocol() << " " << downloads.at(i)->getHostname() << " "
                  << downloads.at(i)->getFilename() << " " << downloads.at(i)->getLocalFilename() << " ";
        if (downloads.at(i)->getPath().empty()) {
            std::cout << "default ";
        } else {
            std::cout << downloads.at(i)->getFilepath() << " ";
        }
        std::cout << downloads.at(i)->getMsg() << " " << downloads.at(i)->progress() << "% "
                  << downloads.at(i)->getSize() << std::endl;
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
