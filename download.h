//
// Created by looper on 30. 12. 2021.
//
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>
#include <stdlib.h>
#include <filesystem>



#ifndef SEMESTRALKA_POS_DOWNLOAD_H
#define SEMESTRALKA_POS_DOWNLOAD_H

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
typedef ssl::stream<tcp::socket> ssl_socket;


class download {
private:
    std::string protocol;
    std::string filename;
    std::string localFilename;
    std::string hostname;
    std::string path;
    std::string filepath;
    int priority;
    double size;
    double currentSize;
    bool pause;
    bool resume;
    bool stop;



public:
    std::string msg="";

    const std::string &getFilepath() const;

    const std::string &getLocalFilename() const;

    const std::string &getPath() const;

    int getPriority() const;

    void startDownload();

    void pauseDownload();

    void resumeDownload();

    void stopDownload();

    double progress();

    const std::string &getPpath() const;

    void setPpath(const std::string &ppath);

    const std::string &getMsg() const;

    void setMsg(const std::string &msg);

    void checkfile(std::string &filename);

    download(std::string protocol,std::string filename,std::string hostname,std::string ppath);

    const std::string &getAProtocol() const;

    void setAProtocol(const std::string &aProtocol);

    int http();

    int https();

    void setFilename(const std::string &filename);

    void setHostname(const std::string &hostname);

    void setSize(long long int size);

    void setCurrentSize(long long int currentSize);

    const std::string &getFilename() const;

    const std::string &getHostname() const;

    long long int getSize() const;

    long long int getCurrentSize() const;

};


#endif //SEMESTRALKA_POS_DOWNLOAD_H
