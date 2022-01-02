#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include "console.h"
#include "download.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    //download asd;
    //asd.http(argv[1],argv[2]);
    console asd;
    asd.listener();

    return 0;
}
