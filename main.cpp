#include <iostream>
#include "fileHandler.h"

using namespace std;

int main() {

    fileHandler fh;

    //fh.history();

    fh.addSchedule("hname", "fname", "lfname", "path/path", "123", "100", "http", "1", "Sat Jan 8 14:23:51 2022");

    fh.addSchedule("hname", "fname", "lfname", "path/path", "123", "100", "http", "1", "Sat Jan 15 14:23:51 2022");

    fh.checkSchedule();


    return 0;
}
