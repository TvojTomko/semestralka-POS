#include <json-c/json.h>
#include <iostream>
#include <fstream>

#include "jsonParser.h"

using namespace std;

FILE *fp;
char buffer[1024];

size_t filesNum;

void jsonRead() {
    struct json_object *files;
    struct json_object *parsed_json;
    struct json_object *fileContent;

    fp = fopen("json-test.json", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    files = json_object_new_array();
    json_object_array_add(files, parsed_json);

    filesNum = json_object_array_length(files);

    for (int i = 0; i < filesNum; i++) {
        fileContent = json_object_array_get_idx(files, i);
        //json_object_ob
        cout << "Object " << (i)+1 << ": " << json_object_get_string(fileContent) << endl;

        cout << "Object " << (i)+1 << ": " << json_object_to_json_string_ext(fileContent,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;
    }

}

void jsonWrite() {
    //potom to dorobit na parametre
    struct json_object *file;
    struct json_object *parsedjson;
    struct json_object *array;

    struct json_object *current;


    array = json_object_new_array();

    fp = fopen("json-test.json", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsedjson = json_tokener_parse(buffer);

    file = json_object_new_object();

    json_object_object_add(file, "hostname", json_object_new_string("www.asd.sk"));
    //json_object_object_add(file, "filename", json_object_new_string("images/background.png"));
    //json_object_object_add(file, "localfilename", json_object_new_string("images/background.png"));
    //json_object_object_add(file, "path", json_object_new_string("cesta/asd"));
    //json_object_object_add(file, "size", json_object_new_int(55555));
    //json_object_object_add(file, "downloaded", json_object_new_int(12345));
    json_object_object_add(file, "protocol", json_object_new_string("https"));
    json_object_object_add(file, "priority", json_object_new_int(1));

    json_object_array_add(array, parsedjson);
    json_object_array_add(array, file);

    //json_object_object_add(array,"file", file);

    json_object_object_add(parsedjson, "file", file);

    json_object_to_file_ext("currentdownload.json", file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

    json_object_to_file_ext("json-test.json", parsedjson, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
}

void jsonDelete() {
    //treba dokoncit
    struct json_object *parsedjson;

    fp = fopen("json-test.txt", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsedjson = json_tokener_parse(buffer);
}

static json_object *currentJson() {
    struct json_object *current;
    struct json_object *parsedjson;

    fp = fopen("json-test.txt", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsedjson = json_tokener_parse(buffer);

    current = json_object_new_object();

    json_object_object_add(current, "history", parsedjson);

    return current;
}