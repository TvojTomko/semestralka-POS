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
#include <boost/lexical_cast.hpp>
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
    std::string username;
    std::string password;
    int priority;
    double size;
    double currentSize;
    bool pause;
    bool resume;
    bool stop;
    bool downloading;


public:
    void setPriority(int priority);

    const std::string &getUsername() const;

    const std::string &getPassword() const;

    std::string msg = "";

    bool isDownloading() const;

    bool isPause() const;

    bool isResume() const;

    bool isStop() const;

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

    download(std::string protocol, std::string filename, std::string hostname, std::string ppath, int priority,
             std::string pusername, std::string ppassword);

    const std::string &getAProtocol() const;

    void setAProtocol(const std::string &aProtocol);

    int http();

    int https();

    int ftp();

    int ftps();

    void setFilename(const std::string &filename);

    void setHostname(const std::string &hostname);

    void setSize(long long int size);

    void setCurrentSize(long long int currentSize);

    const std::string &getFilename() const;

    const std::string &getHostname() const;

    long long int getSize() const;

    long long int getCurrentSize() const;

    int getRetCode(const std::string &stringLine);

    std::string getHostAndPort(std::string &strResult);

};


#endif //SEMESTRALKA_POS_DOWNLOAD_H
