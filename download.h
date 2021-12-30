//
// Created by looper on 30. 12. 2021.
//
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>

#ifndef SEMESTRALKA_POS_DOWNLOAD_H
#define SEMESTRALKA_POS_DOWNLOAD_H

using boost::asio::ip::tcp;

class download {
public:
   int http(std::string hostname,std::string filename);


};


#endif //SEMESTRALKA_POS_DOWNLOAD_H
