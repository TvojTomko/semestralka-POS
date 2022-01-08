#include <iostream>
#include "jsonParser.h"

using namespace std;

int main() {
    jsonWrite();
    //jsonWrite();

    //jsonDelete("file3");

    jsonGetAllInfo("file1");
    jsonGetAllInfo("file2");
    jsonGetAllInfo("file3");

    jsonReadAll();

    return 0;
}
