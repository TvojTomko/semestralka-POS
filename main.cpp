#include <iostream>
#include "jsonParser.h"

using namespace std;

int main() {
    jsonWrite();
    jsonWrite();

    jsonDelete("file3");

    jsonGetInfo("file1");

    //jsonReadAll();
    readHeaders();

    exitProgram();
    return 0;
}
