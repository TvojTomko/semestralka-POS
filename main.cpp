#include <boost/asio.hpp>
#include <iostream>
#include "console.h"
#include "download.h"
#include "fileHandler.h"

using boost::asio::ip::tcp;



int main() {
    console asd;
    asd.listener();
    return 0;
}
