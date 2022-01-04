#include <json-c/json.h>
#include <iostream>

#include "jsonParser.h"

/*
 * TO DO:
 *      classu parser
 *      novy json subor planovac + jeho funkcie
 */

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

        cout << "Reading json... " << endl << json_object_to_json_string_ext(fileContent,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;
    }

}

void jsonWrite() {
    struct json_object *file;
    struct json_object *parsedjson;

    fp = fopen("json-test.json", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsedjson = json_tokener_parse(buffer);

    file = json_object_new_object();

    json_object_object_add(file, "hostname", json_object_new_string("www.das.sk"));
    //json_object_object_add(file, "filename", json_object_new_string("images/background.png"));
    //json_object_object_add(file, "localfilename", json_object_new_string("images/background.png"));
    //json_object_object_add(file, "path", json_object_new_string("cesta/asd"));
    //json_object_object_add(file, "size", json_object_new_int(55555));
    //json_object_object_add(file, "downloaded", json_object_new_int(12345));
    json_object_object_add(file, "protocol", json_object_new_string("httpss"));
    json_object_object_add(file, "priority", json_object_new_int(1));

    json_object_object_add(parsedjson, "file", file);

    json_object_to_file_ext("currentdownload.json", file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

    json_object_to_file_ext("json-test.json", parsedjson, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
}

void jsonDelete() {
    struct json_object *parsedjson;
    struct json_object *key;

    fp = fopen("json-test.json", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsedjson = json_tokener_parse(buffer);

    json_object_object_del(parsedjson, "file");

    json_object_to_file_ext("json-test.json", parsedjson, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

}

static struct json_object * parsedJson() {
    struct json_object *pj;

    fp = fopen("json-test.json", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    pj = json_tokener_parse(buffer);

    return pj;
}

void jsonGetInfo() {
    struct json_object *parsedjson;
    struct json_object *file;

    struct json_object *hostname;
    struct json_object *filename;
    struct json_object *localfilename;
    struct json_object *path;
    struct json_object *size;
    struct json_object *downloaded;
    struct json_object *protocol;
    struct json_object *priority;

    fp = fopen("json-test.json", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsedjson = json_tokener_parse(buffer);

    json_object_object_get_ex(parsedjson, "file1", &file);

    cout << json_object_to_json_string_ext(file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;

    json_object_object_get_ex(file, "localfilename", &localfilename);
    //json_object_object_get_ex(file, "path", &path);
    //json_object_object_get_ex(file, "size", &size);
    //json_object_object_get_ex(file, "downloaded", &downloaded);
    json_object_object_get_ex(file, "protocol", &protocol);
    json_object_object_get_ex(file, "priority", &priority);

    cout << json_object_get_string(protocol) << endl;
}