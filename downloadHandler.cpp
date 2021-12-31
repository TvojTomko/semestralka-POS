//
// Created by looper on 31. 12. 2021.
//

#include "downloadHandler.h"

downloadHandler::downloadHandler() {
    downloading=0;
    maxDownloading=2;
    std::jthread th(&downloadHandler::manageDownloadings,this);
    th.detach();
}

void downloadHandler::startDownloading(int i) {
    std::jthread th(&download::startDownload,downloads.at(i));
}

void downloadHandler::pauseDownloading(int i) {
    downloads.at(i)->pauseDownload();
}

void downloadHandler::resumeDownloading(int i) {
    std::jthread th(&download::resumeDownload,downloads.at(i));
    th.detach();
}

void downloadHandler::stopDownloading(int i) {
    downloads.at(i)->stopDownload();
}

void downloadHandler::addDownload(download* d) {
    downloads.push_back(d);
}

void downloadHandler::manageDownloadings() {
//TODO priority handling

}
void downloadHandler::listOfDownloads() {
    int i=0;
    std::cout<<std::setprecision(3);
    std::cout << "Number " << "Protocol " << "Hostname " << "Filename " << "LocalFileName "<<"LocalPath "<< "Finished " <<"Progress "<<"Size "<<std::endl;
    while (i < downloads.size()) {
        std::cout << i << ". " << downloads.at(i)->getAProtocol() << " " << downloads.at(i)->getHostname() << " "
                  << downloads.at(i)->getFilename() << " "<<downloads.at(i)->getLocalFilename()<<" ";
        if(downloads.at(i)->getPath().empty()){
            std::cout<<"default ";
        }else{
            std::cout<<downloads.at(i)->getFilepath()<<" ";
        }
        std::cout << downloads.at(i)->getMsg() <<" "<<downloads.at(i)->progress()<<"% "<< downloads.at(i)->getSize()<<std::endl;
        i++;
    }
}
void downloadHandler::startAll() {
    for (int i = 0; i < downloads.size(); ++i) {
        std::jthread th(&download::startDownload,downloads.at(i));
        th.detach();
    }
}
void downloadHandler::pauseAll() {
    for (int i = 0; i < downloads.size(); ++i) {
        downloads.at(i)->pauseDownload();
    }
}
