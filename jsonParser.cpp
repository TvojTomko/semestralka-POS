#include <json-c/json.h>
#include <iostream>
#include <fstream>

#include "jsonParser.h"

using namespace std;

FILE *fp;
char buffer[1024];

struct json_object *file2;

size_t filesNum;


void jsonRead() {
    struct json_object *files;
    struct json_object *parsed_json;
    struct json_object *fileContent;
    //struct json_object *name;
    //struct json_object *age;
    //struct json_object *location;

    fp = fopen("json-test.txt", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    filesNum = json_object_array_length(files);

    for (int i = 0; i < filesNum; i++) {
        fileContent = json_object_array_get_idx(files, i);
        //json_object_object_get_ex(files, "name", &name);
        //json_object_object_get_ex(files, "age", &age);
        //json_object_object_get_ex(files, "location", &location);
        cout << "File " << (i)+1 << ": " << json_object_get_string(fileContent) << endl;
    }

}

void jsonWrite() {
    //potom to dorobit na parametre

    struct json_object *file;

    file = json_object_new_object();

    json_object_object_add(file, "hostname", json_object_new_string("www.asd.sk"));
    json_object_object_add(file, "filename", json_object_new_string("images/background.png"));
    json_object_object_add(file, "localfilename", json_object_new_string("images/background.png"));
    json_object_object_add(file, "path", json_object_new_string("cesta/asd"));
    json_object_object_add(file, "size", json_object_new_int(55555));
    json_object_object_add(file, "downloaded", json_object_new_int(12345));
    json_object_object_add(file, "protocol", json_object_new_string("http"));
    json_object_object_add(file, "priority", json_object_new_int(1));

    json_object_to_file("json-test.txt", file);

}