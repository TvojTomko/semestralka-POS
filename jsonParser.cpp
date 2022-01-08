#include <json-c/json.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <time.h>

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
    if (fp == NULL) {
        cout << "Error opening file...";
        return;
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    if (fp == NULL) {
        cout << "Error opening file...";
        return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);


    files = json_object_new_array();
    json_object_array_add(files, json_tokener_parse(buffer));

    filesNum = json_object_array_length(files);

        fileContent = json_object_array_get_idx(files, 0);

        cout << "Reading json... " << endl << json_object_to_json_string_ext(fileContent,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;

    json_object_put(files);
    free(buffer);
}

void jsonWrite(string hostnamep, string filenamep, string localfilenamep, string pathp, string sizep, string downloadedp, string protocolp, string priorityp, string timep) {
    struct json_object *object;
    long sz;

    fp = fopen("json-test.json", "r");
    if (fp == NULL) {
    cout << "Error opening file...";
    return;
    }
fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    if (fp == NULL) {
    cout << "Error opening file...";
    return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    int num = json_object_object_length(object);
    string key = "file" + to_string(num+1);

    cout << "Inserting: " << key << " ..." << endl;

    file = json_object_new_object();

    json_object_object_add(file, "hostname", json_object_new_string(hostnamep.c_str()));
    json_object_object_add(file, "filename", json_object_new_string(filenamep.c_str()));
    json_object_object_add(file, "localfilename", json_object_new_string(localfilenamep.c_str()));
    json_object_object_add(file, "path", json_object_new_string(pathp.c_str()));
    json_object_object_add(file, "size", json_object_new_string(sizep.c_str()));
    json_object_object_add(file, "downloaded", json_object_new_string(downloadedp.c_str()));
    json_object_object_add(file, "protocol", json_object_new_string(protocolp.c_str()));
    json_object_object_add(file, "priority", json_object_new_string(priorityp.c_str()));

    json_object_object_add(file, "scheduled-time",json_object_new_string(timep.c_str()));

    json_object_object_add(object, key.c_str(), file);

    json_object_to_file_ext("currentdownload.json", file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);

    json_object_to_file_ext("json-test.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);

    json_object_put(object);
    free(buffer);
}

void jsonDelete(string fn) {
    struct json_object *object;

    long sz;

    fp = fopen("json-test.json", "r");
    if (fp == NULL) {
    cout << "Error opening file...";
    return;
    }
fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    if (fp == NULL) {
    cout << "Error opening file...";
    return;
    }
fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    json_object_object_del(object, fn.c_str());

    json_object_to_file_ext("json-test.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

    json_object_put(object);
    free(buffer);
}

void jsonGetAllInfo() {
    struct json_object *object;

    struct json_object *hostname;
    struct json_object *filename;
    struct json_object *localfilename;
    struct json_object *path;
    struct json_object *size;
    struct json_object *downloaded;
    struct json_object *protocol;
    struct json_object *priority;

    struct json_object *time;

    string final;

    long sz;

    fp = fopen("json-test.json", "r");

    if (fp == NULL) {
    cout << "Error opening file...";
    return;
    }

    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");

    if (fp == NULL) {
        cout << "Error opening file...";
        return;
    }

    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    int length = json_object_object_length(object);

    string name;
    int flnum = 1;

    for(int i = 0; i < length; i++) {
        name = "file" + to_string(flnum);

        if (json_object_object_get(object, name.c_str()) != nullptr) {
            cout << name << endl;

            //cout << json_object_to_json_string(json_object_object_get(object, name.c_str())) << endl;

            json_object_object_get_ex(object, name.c_str(), &file);

            //cout << json_object_to_json_string_ext(file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;

            json_object_object_get_ex(file, "hostname", &hostname);
            json_object_object_get_ex(file, "filename", &filename);
            json_object_object_get_ex(file, "localfilename", &localfilename);
            json_object_object_get_ex(file, "path", &path);
            json_object_object_get_ex(file, "size", &size);
            json_object_object_get_ex(file, "downloaded", &downloaded);
            json_object_object_get_ex(file, "protocol", &protocol);
            json_object_object_get_ex(file, "priority", &priority);
            json_object_object_get_ex(file, "scheduled-time", &time);

            string stime = json_object_get_string(time);
            std::tm schedtime = {};

            strptime(stime.c_str(), "%a %b %d %H:%M:%S %Y", &schedtime);

            std::time_t schedtm = mktime(&schedtime);

            auto current = std::chrono::system_clock::now();
            std::time_t currenttm = std::chrono::system_clock::to_time_t(current);

            cout << "scheduled time: " << asctime(&schedtime);
            cout << "current time: " << std::ctime(&currenttm) << endl;

            if(schedtm > currenttm)
            cout << "you have enough time... chill" << endl;
            if (schedtm <= currenttm) {
                cout << "start download..." << endl;

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
            }

            flnum++;
        } else {
            cout << name << " not found, searching again..." << endl;
            flnum++;
            i--;
        }

        cout << "*******************************************************************************************" << endl;
    }

    /*
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
    json_object_object_get_ex(file, "scheduled-time", &time);

    string stime = json_object_get_string(time);
    std::tm schedtime = {};

    strptime(stime.c_str(), "%a %b %d %H:%M:%S %Y", &schedtime);

    std::time_t schedtm = mktime(&schedtime);

    auto current = std::chrono::system_clock::now();
    std::time_t currenttm = std::chrono::system_clock::to_time_t(current);

    cout << "scheduled time: " << asctime(&schedtime) << endl;
    cout << "current time: " << std::ctime(&currenttm) << endl;

    if(schedtm > currenttm)
        cout << "you have enough time..." << endl;
    if (schedtm <= currenttm) {
        cout << "start download..." << endl;

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
    }
*/
    json_object_put(object);
    free(buffer);
}