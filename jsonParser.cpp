#include <json-c/json.h>
#include <iostream>

#include "jsonParser.h"

using namespace std;

FILE *fp;
char *buffer;
size_t filesNum;

struct json_object *file;

struct json_object *fileContent;


void jsonReadAll() {
    struct json_object *files;

    long sz;

    fp = fopen("json-test.json", "r");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    fread(buffer, sz, 1, fp);
    fclose(fp);


    files = json_object_new_array();
    json_object_array_add(files, json_tokener_parse(buffer));

    filesNum = json_object_array_length(files);

    for (int i = 0; i < filesNum; i++) {
        fileContent = json_object_array_get_idx(files, i);

        cout << "Reading json... " << endl << json_object_to_json_string_ext(fileContent,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;
    }

    json_object_put(files);
    free(buffer);
}

void jsonWrite() {
    struct json_object *object;
    long sz;

    fp = fopen("json-test.json", "r");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    int num = json_object_object_length(object);
    string key = "file" + to_string(num+1);

    cout << key;

    file = json_object_new_object();

    json_object_object_add(file, "hostname", json_object_new_string("www.das.sk"));
    json_object_object_add(file, "filename", json_object_new_string("images/background.png"));
    json_object_object_add(file, "localfilename", json_object_new_string("images/background.png"));
    json_object_object_add(file, "path", json_object_new_string("cesta/asd"));
    json_object_object_add(file, "size", json_object_new_string("55555"));
    json_object_object_add(file, "downloaded", json_object_new_string("12345"));
    json_object_object_add(file, "protocol", json_object_new_string("httpss"));
    json_object_object_add(file, "priority", json_object_new_string("1"));

    json_object_object_add(object, key.c_str(), file);

    json_object_to_file_ext("currentdownload.json", file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

    json_object_to_file_ext("json-test.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

    json_object_put(object);
    free(buffer);
}

void jsonDelete(string fn) {
    struct json_object *object;

    long sz;

    fp = fopen("json-test.json", "r");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    json_object_object_del(object, fn.c_str());

    json_object_to_file_ext("json-test.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

    json_object_put(object);
    free(buffer);
}

void jsonGetAllInfo(string fn) {
    struct json_object *object;

    struct json_object *hostname;
    struct json_object *filename;
    struct json_object *localfilename;
    struct json_object *path;
    struct json_object *size;
    struct json_object *downloaded;
    struct json_object *protocol;
    struct json_object *priority;

    string final;

    long sz;

    fp = fopen("json-test.json", "r");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    json_object_object_get_ex(object, fn.c_str(), &file);

    cout << json_object_to_json_string_ext(file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;

    json_object_object_get_ex(file, "hostname", &hostname);
    json_object_object_get_ex(file, "filename", &filename);
    json_object_object_get_ex(file, "localfilename", &localfilename);
    json_object_object_get_ex(file, "path", &path);
    json_object_object_get_ex(file, "size", &size);
    json_object_object_get_ex(file, "downloaded", &downloaded);
    json_object_object_get_ex(file, "protocol", &protocol);
    json_object_object_get_ex(file, "priority", &priority);

    final += json_object_get_string(hostname);
    final += " ";
    final += json_object_get_string(filename);
    final += " ";
    final += json_object_get_string(localfilename);
    final += " ";
    final += json_object_get_string(path);
    final += " ";
    final += json_object_get_string(size);
    final += " ";
    final += json_object_get_string(downloaded);
    final += " ";
    final += json_object_get_string(protocol);
    final += " ";
    final += json_object_get_string(priority);

    cout << final << endl;

    json_object_put(object);
    free(buffer);
}