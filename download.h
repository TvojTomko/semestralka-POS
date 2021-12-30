//
// Created by looper on 30. 12. 2021.
//
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <thread>
#include <stdlib.h>


#ifndef SEMESTRALKA_POS_DOWNLOAD_H
#define SEMESTRALKA_POS_DOWNLOAD_H

using boost::asio::ip::tcp;

class download {
private:
    std::string protocol;
    std::string filename;
    std::string hostname;
    std::string path;
    int priority;
    long long size;
    long long currentSize;
    std::jthread t;


public:
    std::string msg="";

    const std::string &getPpath() const;

    void setPpath(const std::string &ppath);

    const std::string &getMsg() const;

    void setMsg(const std::string &msg);

    void stop();

    download(std::string protocol,std::string filename,std::string hostname,std::string ppath);

    const std::string &getAProtocol() const;

    void setAProtocol(const std::string &aProtocol);

    static int http(std::string hostname, std::string filename,download* instance);

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
